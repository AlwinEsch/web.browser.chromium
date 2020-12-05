/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "LauncherPosix.h"

#include "utils/Log.h"
#include "utils/Utils.h"

#include <sys/wait.h>
#include <unistd.h>

#if defined(TARGET_LINUX)
#include <sys/prctl.h>
#endif

namespace kodi
{
namespace sandbox
{
namespace parent
{

// Set the calling thread's signal mask to new_sigmask and return
// the previous signal mask.
sigset_t SetSignalMask(const sigset_t& new_sigmask)
{
  sigset_t old_sigmask;
#if defined(OS_ANDROID)
  // POSIX says pthread_sigmask() must be used in multi-threaded processes,
  // but Android's pthread_sigmask() was broken until 4.1:
  // https://code.google.com/p/android/issues/detail?id=15337
  // http://stackoverflow.com/questions/13777109/pthread-sigmask-on-android-not-working
  RAW_CHECK(sigprocmask(SIG_SETMASK, &new_sigmask, &old_sigmask) == 0);
#else
  RAW_CHECK(pthread_sigmask(SIG_SETMASK, &new_sigmask, &old_sigmask) == 0);
#endif
  return old_sigmask;
}

void ResetChildSignalHandlersToDefaults()
{
  // The previous signal handlers are likely to be meaningless in the child's
  // context so we reset them to the defaults for now. http://crbug.com/44953
  // These signal handlers are set up at least in browser_main_posix.cc:
  // BrowserMainPartsPosix::PreEarlyInitialization and stack_trace_posix.cc:
  // EnableInProcessStackDumping.
  signal(SIGHUP, SIG_DFL);
  signal(SIGINT, SIG_DFL);
  signal(SIGILL, SIG_DFL);
  signal(SIGABRT, SIG_DFL);
  signal(SIGFPE, SIG_DFL);
  signal(SIGBUS, SIG_DFL);
  signal(SIGSEGV, SIG_DFL);
  signal(SIGSYS, SIG_DFL);
  signal(SIGTERM, SIG_DFL);
}

CChildLauncherPosix::CChildLauncherPosix()
{
}

CChildLauncherPosix::~CChildLauncherPosix()
{
  Kill(true);
}

bool CChildLauncherPosix::Launch(const std::vector<std::string>& argv, bool waitForExit)
{
  using namespace std::chrono;

  std::vector<char*> argv_cstr;
  argv_cstr.reserve(argv.size() + 2);
  for (const auto& arg : argv)
    argv_cstr.push_back(const_cast<char*>(arg.c_str()));
  const std::string mainSharedId = "--main-shared=" + GetIdentifier();
  argv_cstr.push_back(const_cast<char*>(mainSharedId.c_str()));
  argv_cstr.push_back(nullptr);

  if (!m_mainThreadReceive)
  {
    kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR, "CChildLauncherPosix::%s: Main receiver not present",
                             __func__);
    return false;
  }

  if (!m_mainThreadReceive->Create(true, true))
  {
    kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR, "CChildLauncherPosix::%s: Failed to create receiver",
                             __func__);
    return false;
  }

  if (!m_mainThreadTransmit->Create(true))
  {
    kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                             "CChildLauncherPosix::%s: Failed to create transmitter", __func__);
    return false;
  }

  sigset_t full_sigset;
  sigfillset(&full_sigset);
  const sigset_t orig_sigmask = SetSignalMask(full_sigset);

  const auto before_fork = high_resolution_clock::now();
  pid_t pid = fork();

  // Always restore the original signal mask in the parent.
  if (pid != 0)
  {
    const auto after_fork = high_resolution_clock::now();
    SetSignalMask(orig_sigmask);

    duration<double> fork_time = duration_cast<duration<double>>(after_fork - before_fork);
    kodi::utils::LOG_MESSAGE(ADDON_LOG_DEBUG, "CChildLauncherPosix::%s: Child fork time %f",
                             __func__, fork_time.count());
  }

  if (pid == -1)
  {
    kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                             "CChildLauncherPosix::%s: Failed to create child process");
    return false;
  }

  if (pid == 0)
  {
    ResetChildSignalHandlersToDefaults();
    SetSignalMask(orig_sigmask);

#if defined(TARGET_LINUX)
    if (prctl(PR_SET_PDEATHSIG, SIGKILL) != 0)
    {
      fprintf(stderr, "prctl(PR_SET_PDEATHSIG) failed");
      _exit(127);
    }
#endif

    execvp(argv_cstr[0], argv_cstr.data());

    _exit(0);
  }

  if (!StartObserver())
  {
    kodi::utils::LOG_MESSAGE(ADDON_LOG_INFO,
                             "CChildLauncherPosix::%s: Failed to start sandbox process observer",
                             __func__);
    Kill(true);
    return false;
  }

  kodi::utils::LOG_MESSAGE(
      ADDON_LOG_INFO,
      "CChildLauncherPosix::%s: Started child process for webbrowser addon (pid %i) in wait %s",
      __func__, pid, waitForExit ? "yes" : "no");
  m_pid = pid;

  if (waitForExit)
  {
    int status = 0;
    pid_t ret = HANDLE_EINTR(waitpid(pid, &status, 0));
    if (ret <= 0)
    {
      ProcessStatus(status);
      m_pid = pid;
    }
  }

  return true;
}

ChildStatus CChildLauncherPosix::ProcessActive()
{
  if (m_pid <= 0)
    return ChildStatus::NotStarted;

  int status = 0;
  int ret = waitpid(m_pid, &status, WNOHANG);
  if (ret == 0)
  {
    m_lastStatus = ChildStatus::Running;
  }
  else if (ret < 0)
  {
    kodi::utils::LOG_MESSAGE(
        ADDON_LOG_ERROR, "CChildLauncherPosix::%s: Asked sandbox process pid %i no more present",
        __func__, m_pid);
    if (m_lastStatus == ChildStatus::Running)
      m_lastStatus = ChildStatus::StoppedByUnknown;
    return m_lastStatus;
  }
  else
  {
    ProcessStatus(status);
  }

  return m_lastStatus;
}

void CChildLauncherPosix::ProcessStatus(int status)
{
  if (WIFEXITED(status))
  {
    m_exitCode = WEXITSTATUS(status);
    m_lastStatus = ChildStatus::ExitedNormal;
    kodi::utils::LOG_MESSAGE(
        ADDON_LOG_DEBUG,
        "CChildLauncherPosix::%s: Sandbox process pid %i, stopped normal with exit code %i",
        __func__, m_pid, m_exitCode);
  }
  else if (WIFSIGNALED(status))
  {
    m_exitCode = WTERMSIG(status);
    if (m_exitCode == SIGSEGV)
    {
      m_lastStatus = ChildStatus::SeqmentionFault;
      kodi::utils::LOG_MESSAGE(
          ADDON_LOG_FATAL, "CChildLauncherPosix::%s: Sandbox process pid %i with seqmention fault",
          __func__, m_pid, m_exitCode);
    }
    else
    {
      m_lastStatus = ChildStatus::StoppedBySignal;
      kodi::utils::LOG_MESSAGE(ADDON_LOG_FATAL,
                               "CChildLauncherPosix::%s: Sandbox process pid %i signaled with %i",
                               __func__, m_pid, m_exitCode);
    }
  }
  else if (WIFSTOPPED(status))
  {
    m_exitCode = WSTOPSIG(status);
    m_lastStatus = ChildStatus::StoppedBySignal;
    kodi::utils::LOG_MESSAGE(
        ADDON_LOG_FATAL,
        "CChildLauncherPosix::%s: Sandbox process pid %i stopped from outside with %i", __func__,
        m_pid, m_exitCode);
  }
  else
  {
    m_lastStatus = ChildStatus::StoppedByUnknown;
    kodi::utils::LOG_MESSAGE(
        ADDON_LOG_FATAL,
        "CChildLauncherPosix::%s: Sandbox process pid %i stopped with unknown status %i", __func__,
        m_pid, status);
  }
}

bool CChildLauncherPosix::Kill(bool wait)
{
  if (m_pid)
  {
    if (!StopObserver())
    {
      kodi::utils::LOG_MESSAGE(ADDON_LOG_INFO,
                               "CChildLauncherPosix::%s: Failed to stop sandbox process observer",
                               __func__);
    }

    if (m_mainThreadReceive)
      m_mainThreadReceive->Destroy();
    for (const auto& entry : m_childThreadReceive)
      entry->Destroy();
    m_childThreadReceive.clear();

    if (m_lastStatus == ChildStatus::Running)
    {
      bool did_terminate = kill(m_pid, SIGTERM) == 0;

      if (wait && did_terminate)
      {
        int cnt = 50;
        pid_t ret_pid = 0;
        while (ret_pid <= 0 && cnt-- > 0)
        {
          std::this_thread::sleep_for(std::chrono::milliseconds(50));
          ret_pid = HANDLE_EINTR(waitpid(m_pid, nullptr, WNOHANG));
        }
        if (ret_pid > 0)
          return true;

        did_terminate = kill(m_pid, SIGKILL) == 0;
        if (did_terminate)
        {
          ret_pid = HANDLE_EINTR(waitpid(m_pid, nullptr, 0));
          if (ret_pid > 0)
            return true;
        }
      }

      if (!did_terminate)
      {
        kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                                 "CChildLauncherPosix::%s: Unable to terminate process %i",
                                 __func__, m_pid);
        return false;
      }
    }
  }

  return true;
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */

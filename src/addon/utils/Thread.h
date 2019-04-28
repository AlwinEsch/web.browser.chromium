/*
 *      Copyright (C) 2015-2019 Team KODI
 *      http:/kodi.tv
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <thread>
#include <condition_variable>

class CThread
{
public:
  CThread(void) = default;

  virtual ~CThread(void)
  {
    StopThread(0);
  }

  static void ThreadHandler(CThread *thread)
  {
    if (thread)
    {
      {
        std::unique_lock<std::mutex> lock(thread->m_threadMutex);
        thread->m_running = true;
        thread->m_stopped = false;
        thread->m_threadCondition.notify_all();
      }

      thread->Process();

      {
        std::unique_lock<std::mutex> lock(thread->m_threadMutex);
        thread->m_running = false;
        thread->m_stopped = true;
        thread->m_threadCondition.notify_all();
      }
    }
  }

  virtual bool IsRunning(void)
  {
    std::unique_lock<std::mutex> lock(m_threadMutex);
    return m_running;
  }

  virtual bool IsStopped(void)
  {
    std::unique_lock<std::mutex> lock(m_threadMutex);
    return m_stop;
  }

  virtual bool CreateThread(bool wait = true)
  {
    bool bReturn(false);
    std::unique_lock<std::mutex> lock(m_threadMutex);
    if (!IsRunning())
    {
      m_stop = false;
      m_thread = std::thread(CThread::ThreadHandler, this);
      if (m_thread.joinable())
      {
        if (wait)
        {
          m_threadCondition.wait(lock, [&]() {return m_running; });
        }
        bReturn = true;
      }
    }
    return bReturn;
  }

  /*!
   * @brief Stop the thread
   * @param waitMs negative = don't wait, 0 = infinite, or the amount of ms to wait
   */
  virtual bool StopThread(uint32_t waitMs = 5000)
  {
    bool bReturn(true);
    bool bRunning(false);
    {
      std::unique_lock<std::mutex> lock(m_threadMutex);
      bRunning = IsRunning();
      m_stop = true;
    }

    if (bRunning && waitMs >= 0)
    {
      std::unique_lock<std::mutex> lock(m_threadMutex);
      bReturn = m_threadCondition.wait_for(lock, std::chrono::milliseconds(waitMs), [&]() {return m_running; });
    }
    else
    {
      m_thread.detach(); // detach from main thread
      bReturn = true;
    }

    return bReturn;
  }

  virtual bool Sleep(uint32_t timeout)
  {
    std::unique_lock<std::mutex> lock(m_threadMutex);
    return m_stop ? false : m_threadCondition.wait_for(lock, std::chrono::milliseconds(timeout), [&]() {return m_running; });
  }

  virtual void Process(void) = 0;

protected:
  void SetRunning(bool bSetTo);

private:
  bool m_stop = false;
  bool m_running = false;
  bool m_stopped = false;
  std::condition_variable m_threadCondition;
  std::mutex m_threadMutex;
  std::thread m_thread;
};


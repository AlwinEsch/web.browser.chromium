/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "V8Handler.h"

// Own
#include "../../MessageIds.h"
#include "../AppRenderer.h"
#include "kodi/General.h"
#include "kodi/gui/dialogs/OK.h"
#include "kodi/gui/dialogs/YesNo.h"

// Dev kit
#include "../../../../lib/kodi-dev-kit/include/kodi/Filesystem.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/General.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/tools/StringUtils.h"

// Global
#include <atomic>

using ::kodi::tools::StringUtils;

namespace chromium
{
namespace app
{
namespace renderer
{
namespace v8
{

namespace
{
static std::atomic_int m_ctorcount{0}; // For debug purposes and to see destructs done
}

CV8Handler::CV8Handler(CWebAppRenderer& renderer) : m_renderer(renderer)
{
  fprintf(stderr, "CV8Handler START (%p) count open %i\n", this, ++m_ctorcount);
}

CV8Handler::~CV8Handler()
{
  fprintf(stderr, "CV8Handler STOP (%p) count open %i\n", this, --m_ctorcount);
}

bool CV8Handler::InitKodiAPI(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             CefRefPtr<CefV8Context> context)
{
  CefRefPtr<CefV8Value> object = context->GetGlobal();
  CefRefPtr<CefV8Value> objKodi = CefV8Value::CreateObject(nullptr, nullptr);
  CefRefPtr<CefV8Value> objKodiGUI = CefV8Value::CreateObject(nullptr, nullptr);
  objKodi->SetValue("gui", objKodiGUI, V8_PROPERTY_ATTRIBUTE_NONE);

  CefRefPtr<CefV8Value> objKodiGUIDialogs = CefV8Value::CreateObject(nullptr, nullptr);
  objKodiGUI->SetValue("dialogs", objKodiGUIDialogs, V8_PROPERTY_ATTRIBUTE_NONE);

  m_subParts.emplace_back(std::make_shared<v8::kodi::CGeneral>(objKodi, object, this));
  m_subParts.emplace_back(std::make_shared<v8::kodi::gui::dialogs::COK>(
      CefV8Value::CreateObject(nullptr, nullptr), objKodiGUIDialogs, this));
  m_subParts.emplace_back(std::make_shared<v8::kodi::gui::dialogs::CYesNo>(
      CefV8Value::CreateObject(nullptr, nullptr), objKodiGUIDialogs, this));

  ::kodi::vfs::CFile file;
  const std::string helperFile = ::kodi::GetAddonPath("resources/js/kodi.js");
  if (!file.OpenFile(helperFile))
  {
    ::kodi::Log(ADDON_LOG_ERROR, "Failed to open JavaScript helper file '%s'", helperFile.c_str());
    return false;
  }

  std::string helperCode;
  helperCode.reserve(file.GetLength());
  char buffer[1024];
  while (int bytesRead = file.Read(buffer, 1024))
    helperCode.append(buffer, bytesRead);

  CefRefPtr<CefV8Value> retval;
  CefRefPtr<CefV8Exception> exception;
  context->Eval(helperCode, "", 0, retval, exception);

  return true;
}

bool CV8Handler::Execute(const CefString& name,
                         CefRefPtr<CefV8Value> object,
                         const CefV8ValueList& arguments,
                         CefRefPtr<CefV8Value>& retval,
                         CefString& exception)
{
  if (!m_renderer.GetBrowser() || !m_renderer.CurrentSiteInterfaceAllowed())
    return false;

  CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
  std::string url = context->GetFrame()->GetURL().ToString();

  for (const auto& part : m_subParts)
  {
    if (StringUtils::StartsWith(name, part->GetIdentfier()))
      return part->Execute(name, object, arguments, retval, exception);
  }

  return false;
}

} /* namespace v8 */
} /* namespace renderer */
} /* namespace app */
} /* namespace chromium */

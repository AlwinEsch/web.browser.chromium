/*
 *  Copyright (C) 2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "CertificateError.h"

// Own
#include "URICheckHandler.h"

// Dev-kit
#include "../../../../lib/kodi-dev-kit/include/kodi/General.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/YesNo.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/tools/StringUtils.h"

// CEF
#include "include/wrapper/cef_helpers.h"

// Global
#include <thread>

/*
Mostly Final

TODO:
- Check addon settings and set of CefSettings.ignore_certificate_errors
*/

using kodi::tools::StringUtils;

namespace chromium
{
namespace app
{
namespace main
{
namespace certificate
{

bool OnCertificateError(CefRefPtr<CefBrowser> browser,
                        ErrorCode cert_error,
                        const CefString& request_url,
                        CefRefPtr<CefSSLInfo> ssl_info,
                        CefRefPtr<CefRequestCallback> callback)
{
  CEF_REQUIRE_UI_THREAD();

  CefRefPtr<CefX509Certificate> cert = ssl_info->GetX509Certificate();
  if (cert.get())
  {
    std::string subject = cert->GetSubject()->GetDisplayName().ToString();
    std::string certStatusString = CURICheck::GetCertStatusString(ssl_info->GetCertStatus());
    std::string text = StringUtils::Format(kodi::GetLocalizedString(31001).c_str(), subject.c_str(),
                                           certStatusString.c_str(), subject.c_str());
    std::thread([text, callback, browser, request_url, cert_error, cert,
                                      ssl_info] {
      bool canceled = false;
      bool ret = kodi::gui::dialogs::YesNo::ShowAndGetInput(
          kodi::GetLocalizedString(31000), text, canceled, kodi::GetLocalizedString(31003),
          kodi::GetLocalizedString(31002));
      // If user has canceled, show him a load fail page.
      if (canceled)
      {
        kodi::Log(ADDON_LOG_WARNING,
                  "User not confirmed correct on uncertified website, showing load fail page now");

        //Load the error page.
        CURICheck::LoadErrorPage(
            browser->GetMainFrame(), request_url, cert_error,
            CURICheck::GetCertificateInformation(cert, ssl_info->GetCertStatus()));
      }

      callback->Continue(ret);
    }).detach();

    return true;
  }

  return false; // Cancel the request.
}

} /* namespace certificate */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */

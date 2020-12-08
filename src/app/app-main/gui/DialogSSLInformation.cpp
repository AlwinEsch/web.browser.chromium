/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "DialogSSLInformation.h"

// Own
#include "../certificate/URICheckHandler.h"

// CEF
#include "include/cef_parser.h"

// Dev-kit
#include "../../../../lib/kodi-dev-kit/include/kodi/General.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/TextViewer.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/tools/StringUtils.h"

// Global
#include <thread>

/*
TODO:
Seems final here.

WARNING:
ShowSSLInformation need to call from main thread, related to CefBrowser!
*/

using kodi::tools::StringUtils;

namespace chromium
{
namespace app
{
namespace main
{
namespace gui
{

namespace
{

// Return HTML string with information about a certificate.
std::string GetCertificateInformation(CefRefPtr<CefX509Certificate> cert,
                                      cef_cert_status_t certstatus)
{
  using namespace certificate;

  CefRefPtr<CefX509CertPrincipal> subject = cert->GetSubject();
  CefRefPtr<CefX509CertPrincipal> issuer = cert->GetIssuer();

  // Build a table showing certificate information. Various types of invalid
  // certificates can be tested using https://badssl.com/.
  std::stringstream ss;
  ss << kodi::GetLocalizedString(31006) << "[CR]";

  if (certstatus != CERT_STATUS_NONE)
  {
    ss << StringUtils::Format(kodi::GetLocalizedString(31007).c_str(),
                              CURICheck::GetCertStatusString(certstatus).c_str())
       << "[CR]";
  }

  ss << StringUtils::Format(kodi::GetLocalizedString(31008).c_str(),
                            subject.get() ? subject->GetDisplayName().ToString().c_str() : "")
     << "[CR]"
     << StringUtils::Format(kodi::GetLocalizedString(31009).c_str(),
                            issuer.get() ? issuer->GetDisplayName().ToString().c_str() : "")
     << "[CR]"
     << StringUtils::Format(kodi::GetLocalizedString(31010).c_str(),
                            CURICheck::GetBinaryString(cert->GetSerialNumber()).c_str())
     << "[CR]"
     << StringUtils::Format(kodi::GetLocalizedString(31011).c_str(),
                            CURICheck::GetTimeString(cert->GetValidStart()).c_str())
     << "[CR]"
     << StringUtils::Format(kodi::GetLocalizedString(31012).c_str(),
                            CURICheck::GetTimeString(cert->GetValidExpiry()).c_str())
     << "[CR]";

  CefX509Certificate::IssuerChainBinaryList der_chain_list;
  CefX509Certificate::IssuerChainBinaryList pem_chain_list;
  cert->GetDEREncodedIssuerChain(der_chain_list);
  cert->GetPEMEncodedIssuerChain(pem_chain_list);
  DCHECK_EQ(der_chain_list.size(), pem_chain_list.size());

  der_chain_list.insert(der_chain_list.begin(), cert->GetDEREncoded());
  pem_chain_list.insert(pem_chain_list.begin(), cert->GetPEMEncoded());

  for (size_t i = 0U; i < der_chain_list.size(); ++i)
  {
    ss << StringUtils::Format(kodi::GetLocalizedString(31013).c_str(),
                              CURICheck::GetBinaryString(der_chain_list[i]).c_str())
       << "[CR]"
       << StringUtils::Format(kodi::GetLocalizedString(31014).c_str(),
                              CURICheck::GetBinaryString(pem_chain_list[i]).c_str())
       << "[CR]";
  }

  ss << kodi::GetLocalizedString(31015);
  return ss.str();
}

} /* namespace */

void ShowSSLInformation(CefRefPtr<CefBrowser> browser)
{
  using namespace certificate;

  std::stringstream ss;
  CefRefPtr<CefNavigationEntry> nav = browser->GetHost()->GetVisibleNavigationEntry();
  if (!nav)
    return;

  CefRefPtr<CefSSLStatus> ssl = nav->GetSSLStatus();
  if (!ssl)
    return;

  ss << kodi::GetLocalizedString(31016) << "[CR]";

  CefURLParts urlparts;
  if (CefParseURL(nav->GetURL(), urlparts))
  {
    CefString port(&urlparts.port);
    ss << StringUtils::Format(kodi::GetLocalizedString(31017).c_str(),
                              std::string(CefString(&urlparts.host).ToString() +
                                          (!port.empty() ? ":" + port.ToString() : ""))
                                  .c_str())
       << "[CR]";
  }

  ss << StringUtils::Format(kodi::GetLocalizedString(31018).c_str(),
                            CURICheck::GetSSLVersionString(ssl->GetSSLVersion()).c_str())
     << "[CR]";
  ss << StringUtils::Format(kodi::GetLocalizedString(31019).c_str(),
                            CURICheck::GetContentStatusString(ssl->GetContentStatus()).c_str())
     << "[CR]";

  CefRefPtr<CefX509Certificate> cert = ssl->GetX509Certificate();
  if (cert.get())
    ss << "[CR]" << GetCertificateInformation(cert, ssl->GetCertStatus());

  std::string str = ss.str();
  std::thread([str] {
    kodi::gui::dialogs::TextViewer::Show(kodi::GetLocalizedString(31005), str);
  }).detach();
}

} /* namespace gui */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */

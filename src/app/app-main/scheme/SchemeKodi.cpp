/*
 *  Copyright (C) 2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "SchemeKodi.h"

// Dev-kit
#include "../../../../lib/kodi-dev-kit/include/kodi/Filesystem.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/General.h"

// Global
#include <algorithm>
#include <string>

/*
TODO:
- Currently only support show of minimal content and not usable for something
  - Make usable to something about addon (history, common links, kodi parts...)
- Make them as default homepage and include also e.g. google for search?
*/

namespace chromium
{
namespace app
{
namespace main
{
namespace scheme
{

/*
 * Scheme handler for: kodi://home/index.html
 */

bool CSchemeKodiHandler::Open(CefRefPtr<CefRequest> request,
                              bool& handle_request,
                              CefRefPtr<CefCallback> callback)
{
  DCHECK(!CefCurrentlyOn(TID_UI) && !CefCurrentlyOn(TID_IO));

  bool handled = false;

  std::string url = request->GetURL();
  if (strstr(url.c_str(), "index.html") != nullptr)
  {
    // Build the response html
    m_data = "<html>"
             "<head>"
             "<title>Kodi</title>"
             "<link rel=\"shortcut icon\" "
             "href=\"https://kodi.tv/sites/default/themes/kodi/favicon.png\" type=\"image/png\">"
             "<style>"
             "p.sansserif {"
             "  font-family: Arial, Helvetica, sans-serif;"
             "}"
             "</style>"
             "</head>"
             "<body bgcolor=\"white\">"
             "<div align=\"center\">"
             "<img src=\"kodi://home/icon.png\"><br/>"
             "<header>"
             "<h1><p class=\"sansserif\">Kodi's scheme homepage</p></h1>"
             "</header>"
             "</div>"
             "</body>"
             "</html>";

    handled = true;

    // Set the resulting mime type
    m_mime_type = "text/html";
  }
  else if (strstr(url.c_str(), "icon.png") != nullptr)
  {
    // Load the response image
    kodi::vfs::CFile file;
    if (file.OpenFile(kodi::GetAddonPath("resources/icon.png")))
    {
      char buf[1 << 16];
      ssize_t len;
      while ((len = file.Read(buf, sizeof(buf))) > 0)
        m_data.append(buf, len);
      handled = true;

      // Set the resulting mime type
      m_mime_type = "image/png";
    }
  }

  if (handled)
  {
    // Indicate the headers are available.
    callback->Continue();
    return true;
  }

  return false;
}

void CSchemeKodiHandler::GetResponseHeaders(CefRefPtr<CefResponse> response,
                                            int64& response_length,
                                            CefString& redirectUrl)
{
  CEF_REQUIRE_IO_THREAD();

  DCHECK(!m_data.empty());

  response->SetMimeType(m_mime_type);
  response->SetStatus(200);

  // Set the resulting response length
  response_length = m_data.length();
}

bool CSchemeKodiHandler::Read(void* data_out,
                              int bytes_to_read,
                              int& bytes_read,
                              CefRefPtr<CefResourceReadCallback> callback)
{
  DCHECK(!CefCurrentlyOn(TID_UI) && !CefCurrentlyOn(TID_IO));

  bool has_data = false;
  bytes_read = 0;

  if (m_offset < m_data.length())
  {
    // Copy the next block of data into the buffer.
    int transfer_size = std::min(bytes_to_read, static_cast<int>(m_data.length() - m_offset));
    memcpy(data_out, m_data.c_str() + m_offset, transfer_size);
    m_offset += transfer_size;

    bytes_read = transfer_size;
    has_data = true;
  }

  return has_data;
}

} /* namespace scheme */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */

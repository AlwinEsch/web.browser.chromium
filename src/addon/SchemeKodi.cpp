/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "SchemeKodi.h"

#include <algorithm>
#include <kodi/Filesystem.h>
#include <kodi/General.h>
#include <string>

/*
 * Scheme handler for: kodi://home/index.html
 */

bool CSchemeKodiHandler::ProcessRequest(CefRefPtr<CefRequest> request,
                                        CefRefPtr<CefCallback> callback)
{
  CEF_REQUIRE_IO_THREAD();

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

bool CSchemeKodiHandler::ReadResponse(void* m_dataout,
                                      int bytes_to_read,
                                      int& bytes_read,
                                      CefRefPtr<CefCallback> callback)
{
  CEF_REQUIRE_IO_THREAD();

  bool has_data = false;
  bytes_read = 0;

  if (m_offset < m_data.length())
  {
    // Copy the next block of data into the buffer.
    int transfer_size = std::min(bytes_to_read, static_cast<int>(m_data.length() - m_offset));
    memcpy(m_dataout, m_data.c_str() + m_offset, transfer_size);
    m_offset += transfer_size;

    bytes_read = transfer_size;
    has_data = true;
  }

  return has_data;
}

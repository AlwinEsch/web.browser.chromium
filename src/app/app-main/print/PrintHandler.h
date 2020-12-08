/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// CEF
#include "include/cef_print_handler.h"

namespace chromium
{
namespace app
{
namespace main
{
namespace print
{

class CPrintHandler : public CefPrintHandler
{
public:
  CPrintHandler();
  ~CPrintHandler() override;

  void OnPrintStart(CefRefPtr<CefBrowser> browser) override;
  void OnPrintSettings(CefRefPtr<CefBrowser> browser,
                       CefRefPtr<CefPrintSettings> settings,
                       bool get_defaults) override;
  bool OnPrintDialog(CefRefPtr<CefBrowser> browser,
                     bool has_selection,
                     CefRefPtr<CefPrintDialogCallback> callback) override;
  bool OnPrintJob(CefRefPtr<CefBrowser> browser,
                  const CefString& document_name,
                  const CefString& pdf_file_path,
                  CefRefPtr<CefPrintJobCallback> callback) override;
  void OnPrintReset(CefRefPtr<CefBrowser> browser) override;
  CefSize GetPdfPaperSize(int device_units_per_inch) override;

  static CefRefPtr<CefPrintHandler> CreatePrintHandler();

private:
  IMPLEMENT_REFCOUNTING(CPrintHandler);
  DISALLOW_COPY_AND_ASSIGN(CPrintHandler);

};

} /* namespace print */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */

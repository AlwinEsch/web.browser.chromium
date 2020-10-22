/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "PrintHandler.h"

// This class is added, but currently not used

CefRefPtr<CefPrintHandler> CPrintHandler::CreatePrintHandler()
{
  return nullptr;
}

void CPrintHandler::OnPrintStart(CefRefPtr<CefBrowser> browser)
{
}

void CPrintHandler::OnPrintSettings(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefPrintSettings> settings,
                                    bool get_defaults)
{
}

bool CPrintHandler::OnPrintDialog(CefRefPtr<CefBrowser> browser,
                                  bool has_selection,
                                  CefRefPtr<CefPrintDialogCallback> callback)
{
  return false;
}

bool CPrintHandler::OnPrintJob(CefRefPtr<CefBrowser> browser,
                               const CefString& document_name,
                               const CefString& pdf_file_path,
                               CefRefPtr<CefPrintJobCallback> callback)
{
  return false;
}

void CPrintHandler::OnPrintReset(CefRefPtr<CefBrowser> browser)
{
}

CefSize CPrintHandler::GetPdfPaperSize(int device_units_per_inch)
{
  return CefSize();
}

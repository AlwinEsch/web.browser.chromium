/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "PrintHandler.h"

// Dev-kit
#include "../../../../lib/kodi-dev-kit/include/kodi/General.h"

/*
TODO:
This class is added, but currently not used
*/

namespace chromium
{
namespace app
{
namespace main
{
namespace print
{

typedef enum PrintSize
{
  PrintSizeA3 = 0,
  PrintSizeA4,
  PrintSizeA5,
  PrintSizeA6,
  PrintSizeJB4,
  PrintSizeTabloid,
  PrintSizeLetter,
  PrintSizeLegal,
  PrintSizeFolio,
  PrintSizeExecutive,
  PrintSizeJIS_B5,
  PrintSize4x6in,
  PrintSize5x8in,
  PrintSize3_5x5in,
  PrintSize5x7in,
  PrintSizePostcard1,
  PrintSizePostcard2_Double,
  PrintSizeC5Envelope,
  PrintSizeDLEnvelope,
  PrintSizeCom10,
  PrintSizeMonarch,
  PrintSizeC6Envelope,
  PrintSizeY4Envelope,
  PrintSizeMax
} PrintSize;

typedef struct
{
  PrintSize size;
  float metric_width;
  float metric_height;
  float inch_width;
  float inch_height;
} PrintSizeList;

static const PrintSizeList sizeList[] = {
    {PrintSizeA3, 297.0f, 420.0f, 11.693f, 16.535f},
    {PrintSizeA4, 210.0f, 297.0f, 8.268f, 11.693f},
    {PrintSizeA5, 148.0f, 210.0f, 5.827f, 8.268f},
    {PrintSizeA6, 105.0f, 148.0f, 4.134f, 5.827f},
    {PrintSizeJB4, 257.0f, 364.0f, 10.1f, 14.3f},
    {PrintSizeTabloid, 279.0f, 432.0f, 11.0f, 17.0f},
    {PrintSizeLetter, 216.0f, 279.0f, 8.5f, 11.0f},
    {PrintSizeLegal, 216.0f, 356.0f, 8.5f, 14.0f},
    {PrintSizeFolio, 304.8f, 482.6f, 12.0f, 19.0f},
    {PrintSizeExecutive, 184.2f, 266.7f, 7.0f, 10.0f},
    {PrintSizeJIS_B5, 182.0f, 257.0f, 7.2f, 10.1f},
    {PrintSize4x6in, 101.6f, 152.4f, 4.0f, 6.0f},
    {PrintSize5x8in, 127.0f, 203.2f, 5.0f, 8.0f},
    {PrintSize3_5x5in, 88.9, 127.0f, 3.5f, 5.0f},
    {PrintSize5x7in, 127.0f, 177.8f, 5.0f, 7.0f},
    {PrintSizePostcard1, 394.0f, 489.0f, 15.5f, 19.3f},
    {PrintSizePostcard2_Double, 483.0f, 762.0f, 19.0f, 30.0f},
    {PrintSizeC5Envelope, 162.0f, 229.0f, 6.4f, 9.0f},
    {PrintSizeDLEnvelope, 220.0f, 110.0f, 8.66f, 4.33f},
    {PrintSizeCom10, 104.8f, 241.3f, 4.125f, 9.5f},
    {PrintSizeMonarch, 98.4f, 190.5f, 3.875f, 7.5f},
    {PrintSizeC6Envelope, 162.0f, 114.0f, 6.38f, 4.49f},
    {PrintSizeY4Envelope, 235.0f, 105.0f, 9.254f, 4.134f},
};

CPrintHandler::CPrintHandler()
{
}

CPrintHandler::~CPrintHandler()

{
}

CefRefPtr<CefPrintHandler> CPrintHandler::CreatePrintHandler()
{
  return new CPrintHandler;
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
  PrintSize size = kodi::GetSettingEnum<PrintSize>("other.printsize", PrintSizeA4);

  float width = 0.0f;
  float height = 0.0f;
  for (unsigned int i; i < PrintSizeMax; ++i)
  {
    if (sizeList[i].size == size)
    {
      width = sizeList[i].inch_width;
      height = sizeList[i].inch_height;
      break;
    }
  }

  return CefSize(width * device_units_per_inch, height * device_units_per_inch);
}

} /* namespace print */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */

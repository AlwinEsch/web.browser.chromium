/*
 *      Copyright (C) 2015-2017 Team KODI
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

#include "DOMVisitor.h"
#include "MessageIds.h"

#include <kodi/General.h>

CDOMVisitor::CDOMVisitor(CefRefPtr<CefBrowser> browser)
  : m_browser(browser)
{

}

void CDOMVisitor::Visit(CefRefPtr<CefDOMDocument> document)
{
  fprintf(stderr, "-------------------------<> %s\n", __PRETTY_FUNCTION__);
  CefRefPtr<CefDOMNode> node = document->GetFocusedNode();
  if (node == nullptr)
    return;
fprintf(stderr, "-222------------------------<> %s\n", __PRETTY_FUNCTION__);
  std::string elementType = node->GetFormControlElementType();
  std::string elementName = node->GetName();
  if (elementName == "SELECT")
  {
    fprintf(stderr, "---3333----------------------<> %s\n", __PRETTY_FUNCTION__);
    CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(RendererMessage::ShowSelect);
    message->GetArgumentList()->SetString(0, elementType);
    message->GetArgumentList()->SetString(1, node->GetElementAttribute("title"));
    message->GetArgumentList()->SetString(2, node->GetValue());
    message->GetArgumentList()->SetString(3, node->GetElementAttribute("id"));
    message->GetArgumentList()->SetString(4, node->GetElementAttribute("name"));
    message->GetArgumentList()->SetString(5, node->GetAsMarkup());

    int cnt = 0;
    for (CefRefPtr<CefDOMNode> entry = node->GetFirstChild(); entry; entry = entry->GetNextSibling())
    {
      if (entry->GetType() != DOM_NODE_TYPE_ELEMENT)
        continue;
      message->GetArgumentList()->SetString(cnt*3+0+7, entry->GetElementAttribute("value"));
      message->GetArgumentList()->SetString(cnt*3+1+7, entry->GetElementInnerText());
      message->GetArgumentList()->SetBool(cnt*3+2+7, !entry->GetElementAttribute("selected").empty());
      cnt++;
    }

    message->GetArgumentList()->SetInt(6, cnt);
    m_browser->SendProcessMessage(PID_BROWSER, message);
  }
  else if (elementName == "INPUT" && node->IsEditable())
  {
    fprintf(stderr, "--3333-----------------------<> %s\n", __PRETTY_FUNCTION__);
    CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(RendererMessage::ShowKeyboard);
    message->GetArgumentList()->SetString(0, elementType);
    message->GetArgumentList()->SetString(1, node->GetElementAttribute("title"));
    message->GetArgumentList()->SetString(2, node->GetValue());
    message->GetArgumentList()->SetString(3, node->GetElementAttribute("id"));
    message->GetArgumentList()->SetString(4, node->GetElementAttribute("name"));
    message->GetArgumentList()->SetString(5, node->GetAsMarkup());
    m_browser->SendProcessMessage(PID_BROWSER, message);
  }
  else if (elementName == "DIV" && node->IsEditable())
  {

  }

    fprintf(stderr, "---1: ----GetType %i\n", node->GetType());
    fprintf(stderr, "---2: ----GetName %s\n", node->GetName().ToString().c_str());
    fprintf(stderr, "---3: ----IsElement %i\n", node->IsElement());
    fprintf(stderr, "---4: ----IsEditable %i\n", node->IsEditable());
    fprintf(stderr, "---5: ----IsFormControlElement %i\n", node->IsFormControlElement());
    fprintf(stderr, "---6: ----GetFormControlElementType %s\n", node->GetFormControlElementType().ToString().c_str());
    fprintf(stderr, "---6: ----GetAsMarkup %s\n", node->GetAsMarkup().ToString().c_str());
    fprintf(stderr, "---7: ----GetValue %s\n", node->GetValue().ToString().c_str());
    fprintf(stderr, "---8: ----GetElementTagName %s\n", node->GetElementTagName().ToString().c_str());
    fprintf(stderr, "---9: ----HasElementAttributes %i\n", node->HasElementAttributes());
    fprintf(stderr, "---10: ----GetElementInnerText %s\n", node->GetElementInnerText().ToString().c_str());
    fprintf(stderr, "---11: ----HasChildren %i\n", node->HasChildren());
}

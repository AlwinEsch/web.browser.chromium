/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "input/Action.h"

#include <memory>
#include <string>
#include <vector>

namespace kodi
{
namespace gui
{

class CListItem;

class CWindow
{
public:
  CWindow(const std::string& xmlFilename,
          const std::string& defaultSkin,
          bool asDialog,
          bool isMedia = false);
  virtual ~CWindow();

  bool Show();
  void Close();
  void DoModal();
  bool SetFocusId(int controlId);
  int GetFocusId();
  void SetControlLabel(int controlId, const std::string& label);
  void SetControlVisible(int controlId, bool visible);
  void SetControlSelected(int controlId, bool selected);
  void SetProperty(const std::string& key, const std::string& value);
  std::string GetProperty(const std::string& key) const;
  void SetPropertyInt(const std::string& key, int value);
  int GetPropertyInt(const std::string& key) const;
  void SetPropertyBool(const std::string& key, bool value);
  bool GetPropertyBool(const std::string& key) const;
  void SetPropertyDouble(const std::string& key, double value);
  double GetPropertyDouble(const std::string& key) const;
  void ClearProperties();
  void ClearProperty(const std::string& key);
  void ClearList();
  void AddListItem(std::shared_ptr<CListItem> item, int itemPosition = -1);
  void AddListItem(const std::string item, int itemPosition = -1);
  void RemoveListItem(int itemPosition);
  void RemoveListItem(std::shared_ptr<CListItem> item);
  std::shared_ptr<CListItem> GetListItem(int listPos);
  void SetCurrentListPosition(int listPos);
  int GetCurrentListPosition() const;
  int GetListSize() const;
  void SetContainerProperty(const std::string& key, const std::string& value);
  void SetContainerContent(const std::string& value);
  int GetCurrentContainerId() const;
  void MarkDirtyRegion();

  virtual bool OnInit() { return false; };
  virtual bool OnFocus(int controlId) { return false; };
  virtual bool OnClick(int controlId) { return false; };
  virtual bool OnAction(const ::kodi::gui::input::CAction& action)
  {
    switch (action.GetID())
    {
      case ADDON_ACTION_PREVIOUS_MENU:
      case ADDON_ACTION_NAV_BACK:
        Close();
        return true;
      default:
        break;
    }
    return false;
  }
  virtual void GetContextButtons(int itemNumber,
                                 std::vector<std::pair<unsigned int, std::string>>& buttons)
  {
  }
  virtual bool OnContextButton(int itemNumber, unsigned int button) { return false; }

  uint64_t GetControlHandle() const { return m_handle; }

protected:
  uint64_t m_handle;
};

} /* namespace gui */
} /* namespace kodi */

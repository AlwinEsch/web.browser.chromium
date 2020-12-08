
/*
 *  Copyright (C) 2005-2020 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <kodi/AddonBase.h>
#include <kodi/c-api/addon-instance/web.h>
#include <kodi/gui/input/Action.h>

#ifdef __cplusplus

namespace kodi
{
namespace addon
{

class CInstanceWeb;

class ATTRIBUTE_HIDDEN CWebControlOwn
{
public:
  CWebControlOwn(WEB_CONTROL_HANDLE& handle, int dataIdentifier)
    : m_handle(handle)
  {
    m_handle.controlHdlAddonParent = this;
    m_handle.dataIdentifier = dataIdentifier;
  }

  virtual ~CWebControlOwn() = default;

  virtual bool RenderInit(int width, int height) { return false; }
  virtual void Render() {}
  virtual bool Dirty() { return false; }
  virtual bool OnInit() { return false; }
  virtual bool OnAction(const gui::input::CAction& action, int& nextItem)
  {
    return false;
  }
  virtual bool OnMouseEvent(int id, double x, double y, double offsetX, double offsetY, int state)
  {
    return false;
  }
  virtual bool OpenWebsite(const std::string& url) { return false; }
  virtual bool GetHistory(std::vector<std::string>& historyWebsiteNames, bool behindCurrent)
  {
    return false;
  }
  virtual void SearchText(const std::string& text, bool forward, bool matchCase, bool findNext) {}
  virtual void StopSearch(bool clearSelection) {}
  virtual void Reload() {}
  virtual void StopLoad() {}
  virtual void GoBack() {}
  virtual void GoForward() {}
  virtual void OpenOwnContextMenu() {}
  virtual void ScreenSizeChange(float x, float y, float width, float height, bool fullscreen) {}

  inline void SetControlReady(bool ready)
  {
    const AddonInstance_WebAddon* data = m_handle.instanceData;
    data->toKodi->control_set_control_ready(data->toKodi->kodiInstance, m_handle.props->pControlIdent,
                                           ready);
  }

  inline void SetOpenedAddress(const std::string& title)
  {
    const AddonInstance_WebAddon* data = m_handle.instanceData;
    data->toKodi->control_set_opened_address(data->toKodi->kodiInstance,
                                            m_handle.props->pControlIdent, title.c_str());
  }

  inline void SetOpenedTitle(const std::string& title)
  {
    const AddonInstance_WebAddon* data = m_handle.instanceData;
    data->toKodi->control_set_opened_title(data->toKodi->kodiInstance, m_handle.props->pControlIdent,
                                          title.c_str());
  }

  inline void SetIconURL(const std::string& icon)
  {
    const AddonInstance_WebAddon* data = m_handle.instanceData;
    data->toKodi->control_set_icon_url(data->toKodi->kodiInstance, m_handle.props->pControlIdent,
                                      icon.c_str());
  }

  inline void SetFullscreen(bool fullscreen)
  {
    const AddonInstance_WebAddon* data = m_handle.instanceData;
    data->toKodi->control_set_fullscreen(data->toKodi->kodiInstance, m_handle.props->pControlIdent,
                                        fullscreen);
  }

  inline void SetLoadingState(bool isLoading, bool canGoBack, bool canGoForward)
  {
    const AddonInstance_WebAddon* data = m_handle.instanceData;
    data->toKodi->control_set_loading_state(data->toKodi->kodiInstance, m_handle.props->pControlIdent,
                                           isLoading, canGoBack, canGoForward);
  }

  inline void SetTooltip(const std::string& tooltip)
  {
    const AddonInstance_WebAddon* data = m_handle.instanceData;
    data->toKodi->control_set_tooltip(data->toKodi->kodiInstance, m_handle.props->pControlIdent,
                                     tooltip.c_str());
  }

  inline void SetStatusMessage(const std::string& status)
  {
    const AddonInstance_WebAddon* data = m_handle.instanceData;
    data->toKodi->control_set_status_message(data->toKodi->kodiInstance,
                                            m_handle.props->pControlIdent, status.c_str());
  }

  inline void RequestOpenSiteInNewTab(const std::string& url)
  {
    const AddonInstance_WebAddon* data = m_handle.instanceData;
    data->toKodi->control_request_open_site_in_new_tab(data->toKodi->kodiInstance,
                                                      m_handle.props->pControlIdent, url.c_str());
  }

  int GetDataIdentifier() const { return m_handle.dataIdentifier; }
  std::string GetName() const { return m_handle.props->strName; }
  virtual void* GetDevice() const { return m_handle.props->pDevice; }
  virtual float GetXPos() const { return m_handle.props->fXPos; }
  virtual float GetYPos() const { return m_handle.props->fYPos; }
  virtual float GetWidth() const { return m_handle.props->fWidth; }
  virtual float GetHeight() const { return m_handle.props->fHeight; }
  virtual float GetPixelRatio() const { return m_handle.props->fPixelRatio; }
  virtual float GetFPS() const { return m_handle.props->fFPS; }
  virtual float GetSkinXPos() const { return m_handle.props->fSkinXPos; }
  virtual float GetSkinYPos() const { return m_handle.props->fSkinYPos; }
  virtual float GetSkinWidth() const { return m_handle.props->fSkinWidth; }
  virtual float GetSkinHeight() const { return m_handle.props->fSkinHeight; }
  virtual bool UseTransparentBackground() const
  {
    return m_handle.props->bUseTransparentBackground;
  }
  virtual uint32_t GetBackgroundColorARGB() const { return m_handle.props->iBackgroundColorARGB; }
  int GetGUIItemLeft() const { return m_handle.props->iGUIItemLeft; }
  int GetGUIItemRight() const { return m_handle.props->iGUIItemRight; }
  int GetGUIItemTop() const { return m_handle.props->iGUIItemTop; }
  int GetGUIItemBottom() const { return m_handle.props->iGUIItemBottom; }
  int GetGUIItemBack() const { return m_handle.props->iGUIItemBack; }

private:
  friend class CInstanceWeb;

  WEB_CONTROL_HANDLE& m_handle;
};

class ATTRIBUTE_HIDDEN CInstanceWebOwn : public IAddonInstance
{
public:
  //==========================================================================
  ///
  /// @ingroup cpp_kodi_addon_web
  /// @brief Web addon class constructor
  ///
  /// Used by an add-on that only supports web browser.
  ///
  CInstanceWebOwn() : IAddonInstance(ADDON_INSTANCE_WEB, GetKodiTypeVersion(ADDON_INSTANCE_WEB))
  {
    if (CAddonBase::m_interface->globalSingleInstance != nullptr)
      throw std::logic_error("kodi::addon::CInstanceWebOwn: Creation of more as one in single "
                             "instance way is not allowed!");

    SetAddonStruct(CAddonBase::m_interface->firstKodiInstance);
    CAddonBase::m_interface->globalSingleInstance = this;
  }
  //--------------------------------------------------------------------------

  CInstanceWebOwn(KODI_HANDLE instance, const std::string& kodiVersion = "")
    : IAddonInstance(ADDON_INSTANCE_WEB,
                     !kodiVersion.empty() ? kodiVersion : GetKodiTypeVersion(ADDON_INSTANCE_WEB))
  {
    if (CAddonBase::m_interface->globalSingleInstance != nullptr)
      throw std::logic_error(
          "kodi::addon::CInstanceWebOwn: Creation of multiple together with single "
          "instance way is not allowed!");

    SetAddonStruct(instance);
  }

  virtual ~CInstanceWebOwn() = default;

  virtual WEB_ADDON_ERROR StartInstance() { return WEB_ADDON_ERROR_NO_ERROR; }
  virtual void StopInstance() {}

  virtual bool MainInitialize() { return false; }

  virtual void MainLoop() {}

  virtual void MainShutdown() {}

  virtual void StartDone(uint64_t childHandle) {}

  virtual void SetMute(bool mute) {}
  virtual bool SetLanguage(const std::string& language) { return false; }

  virtual uint64_t CreateControl(const std::string& sourceName,
                                 const std::string& startURL,
                                 WEB_CONTROL_HANDLE& handle,
                                 int& dataIdentifier)
  {
    return 0;
  }
  virtual bool DestroyControl(uint64_t control, bool complete) { return false; }

  inline void InhibitShutdown(bool inhibit)
  {
    m_instanceData->toKodi->inhibit_shutdown(m_instanceData->toKodi->kodiInstance, inhibit);
  }

  inline void InhibitScreensaver(bool inhibit)
  {
    m_instanceData->toKodi->inhibit_screensaver(m_instanceData->toKodi->kodiInstance, inhibit);
  }

  inline std::string UserPath() { return m_instanceData->props->strUserPath; }

  inline std::string AddonLibPath() { return m_instanceData->props->strAddonLibPath; }

  inline std::string AddonSharePath() { return m_instanceData->props->strAddonSharePath; }

  inline bool IsMuted() const
  {
    return m_instanceData->toKodi->is_muted(m_instanceData->toKodi->kodiInstance);
  }

private:
  void SetAddonStruct(KODI_HANDLE instance)
  {
    if (instance == nullptr)
      throw std::logic_error(
          "kodi::addon::CInstanceWebOwn: Creation with empty addon structure not "
          "allowed, table must be given from Kodi!");

    m_instanceData = static_cast<AddonInstance_WebAddon*>(instance);
    m_instanceData->toAddon->addonInstance = this;
    m_instanceData->toAddon->main_initialize = main_initialize;
    m_instanceData->toAddon->main_loop = main_loop;
    m_instanceData->toAddon->main_shutdown = main_shutdown;
    m_instanceData->toAddon->start_instance = start_instance;
    m_instanceData->toAddon->stop_instance = stop_instance;
    m_instanceData->toAddon->set_mute = set_mute;
    m_instanceData->toAddon->set_language = set_language;
    m_instanceData->toAddon->create_control = create_control;
    m_instanceData->toAddon->destroy_control = destroy_control;
    m_instanceData->toAddon->control_render_init = control_render_init;
    m_instanceData->toAddon->control_render = control_render;
    m_instanceData->toAddon->control_dirty = control_dirty;
    m_instanceData->toAddon->control_on_init = control_on_init;
    m_instanceData->toAddon->control_on_action = control_on_action;
    m_instanceData->toAddon->control_on_mouse_event = control_on_mouse_event;
    m_instanceData->toAddon->control_open_website = control_open_website;
    m_instanceData->toAddon->control_get_history = control_get_history;
    m_instanceData->toAddon->control_get_history_clear = control_get_history_clear;
    m_instanceData->toAddon->control_search_text = control_search_text;
    m_instanceData->toAddon->control_stop_search = control_stop_search;
    m_instanceData->toAddon->control_web_cmd_reload = control_web_cmd_reload;
    m_instanceData->toAddon->control_web_cmd_stop_load = control_web_cmd_stop_load;
    m_instanceData->toAddon->control_web_cmd_nav_back = control_web_cmd_nav_back;
    m_instanceData->toAddon->control_web_cmd_nav_forward = control_web_cmd_nav_forward;
    m_instanceData->toAddon->control_web_open_own_context_menu = control_web_open_own_context_menu;
    m_instanceData->toAddon->control_screen_size_change = control_screen_size_change;
  }

  inline static WEB_ADDON_ERROR start_instance(const AddonInstance_WebAddon* instance)
  {
    return static_cast<CInstanceWebOwn*>(instance->toAddon->addonInstance)->StartInstance();
  }

  inline static void stop_instance(const AddonInstance_WebAddon* instance)
  {
    static_cast<CInstanceWebOwn*>(instance->toAddon->addonInstance)->StopInstance();
  }

  inline static bool main_initialize(const AddonInstance_WebAddon* instance)
  {
    return static_cast<CInstanceWebOwn*>(instance->toAddon->addonInstance)->MainInitialize();
  }

  inline static void main_loop(const AddonInstance_WebAddon* instance)
  {
    static_cast<CInstanceWebOwn*>(instance->toAddon->addonInstance)->MainLoop();
  }

  inline static void main_shutdown(const AddonInstance_WebAddon* instance)
  {
    static_cast<CInstanceWebOwn*>(instance->toAddon->addonInstance)->MainShutdown();
  }

  inline static void set_mute(const AddonInstance_WebAddon* instance, bool mute)
  {
    static_cast<CInstanceWebOwn*>(instance->toAddon->addonInstance)->SetMute(mute);
  }

  inline static bool set_language(const AddonInstance_WebAddon* instance, const char* language)
  {
    return static_cast<CInstanceWebOwn*>(instance->toAddon->addonInstance)->SetLanguage(language);
  }

  inline static WEB_ADDON_ERROR create_control(const AddonInstance_WebAddon* instance,
                                               const WEB_ADDON_GUI_PROPS* props,
                                               const char* start_url,
                                               WEB_CONTROL_HANDLE* handle)
  {
    handle->instanceData = instance;
    int dataIdentifier;
    uint64_t control =
        static_cast<CInstanceWebOwn*>(instance->toAddon->addonInstance)
            ->CreateControl(props->strName, start_url, *handle, dataIdentifier);
    if (!control)
    {
      kodi::Log(ADDON_LOG_ERROR, "Failed to create web addon control instance");
      return WEB_ADDON_ERROR_FAILED;
    }

    return WEB_ADDON_ERROR_NO_ERROR;
  }

  inline static bool destroy_control(const AddonInstance_WebAddon* instance,
                                     const WEB_CONTROL_HANDLE* handle,
                                     bool complete)
  {
    return static_cast<CInstanceWebOwn*>(instance->toAddon->addonInstance)
                   ->DestroyControl(handle->controlHdlAddonChild, complete);
  }

  //============================================================================
  // Control

  inline static bool control_render_init(const struct AddonInstance_WebAddon* instance,
                                         const WEB_CONTROL_HANDLE* handle,
                                         float width,
                                         float height)
  {
    return static_cast<CWebControlOwn*>(handle->controlHdlAddonParent)->RenderInit(width, height);
  }

  inline static void control_render(const AddonInstance_WebAddon* instance,
                                    const WEB_CONTROL_HANDLE* handle)
  {
    static_cast<CWebControlOwn*>(handle->controlHdlAddonParent)->Render();
  }

  inline static bool control_dirty(const AddonInstance_WebAddon* instance,
                                   const WEB_CONTROL_HANDLE* handle)
  {
    return static_cast<CWebControlOwn*>(handle->controlHdlAddonParent)->Dirty();
  }

  inline static bool control_on_init(const AddonInstance_WebAddon* instance,
                                     const WEB_CONTROL_HANDLE* handle)
  {
    return static_cast<CWebControlOwn*>(handle->controlHdlAddonParent)->OnInit();
  }

  inline static bool control_on_action(const AddonInstance_WebAddon* instance,
                                       const WEB_CONTROL_HANDLE* handle,
                                       const addon_action_data* action,
                                       int* nextItem)
  {
    return static_cast<CWebControlOwn*>(handle->controlHdlAddonParent)
        ->OnAction(action, *nextItem);
  }

  inline static bool control_on_mouse_event(const AddonInstance_WebAddon* instance,
                                            const WEB_CONTROL_HANDLE* handle,
                                            int id,
                                            double x,
                                            double y,
                                            double offsetX,
                                            double offsetY,
                                            int state)
  {
    return static_cast<CWebControlOwn*>(handle->controlHdlAddonParent)
        ->OnMouseEvent(id, x, y, offsetX, offsetY, state);
  }

  inline static bool control_open_website(const AddonInstance_WebAddon* instance,
                                          const WEB_CONTROL_HANDLE* handle,
                                          const char* url)
  {
    return static_cast<CWebControlOwn*>(handle->controlHdlAddonParent)->OpenWebsite(url);
  }

  inline static bool control_get_history(const AddonInstance_WebAddon* instance,
                                         const WEB_CONTROL_HANDLE* handle,
                                         char*** list,
                                         unsigned int* entries,
                                         bool behind_current)
  {
    std::vector<std::string> historyWebsiteNames;
    bool ret = static_cast<CWebControlOwn*>(handle->controlHdlAddonParent)
                   ->GetHistory(historyWebsiteNames, behind_current);
    if (ret)
    {
      *list = static_cast<char**>(malloc(historyWebsiteNames.size() * sizeof(char*)));
      *entries = static_cast<unsigned int>(historyWebsiteNames.size());
      for (size_t i = 0; i < historyWebsiteNames.size(); ++i)
#ifdef WIN32 // To prevent warning C4996
        (*list)[i] = _strdup(historyWebsiteNames[i].c_str());
#else
        (*list)[i] = strdup(historyWebsiteNames[i].c_str());
#endif
    }
    return ret;
  }

  inline static void control_get_history_clear(const AddonInstance_WebAddon* instance,
                                               const WEB_CONTROL_HANDLE* handle,
                                               char** list,
                                               unsigned int entries)
  {
    for (unsigned int i = 0; i < entries; ++i)
      free(list[i]);
    free(list);
  }

  inline static void control_search_text(const AddonInstance_WebAddon* instance,
                                         const WEB_CONTROL_HANDLE* handle,
                                         const char* text,
                                         bool forward,
                                         bool matchCase,
                                         bool findNext)
  {
    static_cast<CWebControlOwn*>(handle->controlHdlAddonParent)
        ->SearchText(text, forward, matchCase, findNext);
  }

  inline static void control_stop_search(const AddonInstance_WebAddon* instance,
                                         const WEB_CONTROL_HANDLE* handle,
                                         bool clearSelection)
  {
    static_cast<CWebControlOwn*>(handle->controlHdlAddonParent)->StopSearch(clearSelection);
  }

  inline static void control_web_cmd_reload(const AddonInstance_WebAddon* instance,
                                            const WEB_CONTROL_HANDLE* handle)
  {
    static_cast<CWebControlOwn*>(handle->controlHdlAddonParent)->Reload();
  }

  inline static void control_web_cmd_stop_load(const AddonInstance_WebAddon* instance,
                                               const WEB_CONTROL_HANDLE* handle)
  {
    static_cast<CWebControlOwn*>(handle->controlHdlAddonParent)->StopLoad();
  }

  inline static void control_web_cmd_nav_back(const AddonInstance_WebAddon* instance,
                                              const WEB_CONTROL_HANDLE* handle)
  {
    static_cast<CWebControlOwn*>(handle->controlHdlAddonParent)->GoBack();
  }

  inline static void control_web_cmd_nav_forward(const AddonInstance_WebAddon* instance,
                                                 const WEB_CONTROL_HANDLE* handle)
  {
    static_cast<CWebControlOwn*>(handle->controlHdlAddonParent)->GoForward();
  }

  inline static void control_web_open_own_context_menu(const AddonInstance_WebAddon* instance,
                                                       const WEB_CONTROL_HANDLE* handle)
  {
    static_cast<CWebControlOwn*>(handle->controlHdlAddonParent)->OpenOwnContextMenu();
  }

  inline static void control_screen_size_change(const AddonInstance_WebAddon* instance,
                                                const WEB_CONTROL_HANDLE* handle,
                                                float x,
                                                float y,
                                                float width,
                                                float height,
                                                bool fullscreen)
  {
    static_cast<CWebControlOwn*>(handle->controlHdlAddonParent)
        ->ScreenSizeChange(x, y, width, height, fullscreen);
  }

  AddonInstance_WebAddon* m_instanceData;
};

} /* namespace addon */
} /* namespace kodi */

#endif /* __cplusplus */

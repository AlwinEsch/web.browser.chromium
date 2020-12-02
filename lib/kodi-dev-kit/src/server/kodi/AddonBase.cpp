/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "AddonBase.h"

#include "../../sandbox/kodi/AddonBase.h"
#include "../utils/Log.h"

#include <kodi/c-api/addon_base.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

class ATTRIBUTE_HIDDEN CAddonBaseHdl
{
public:
  CAddonBaseHdl()
  {
    m_interface->toAddon->destroy = ADDONBASE_Destroy;
    m_interface->toAddon->create_instance = ADDONBASE_CreateInstance;
    m_interface->toAddon->destroy_instance = ADDONBASE_DestroyInstance;
    m_interface->toAddon->set_setting = ADDONBASE_SetSetting;
  }
  virtual ~CAddonBaseHdl() = default;

  virtual ADDON_STATUS Create() { return ADDON_STATUS_OK; }
  virtual ADDON_STATUS SetSetting(const std::string& settingName,
                                  const kodi::CSettingValue& settingValue)
  {
    return ADDON_STATUS_UNKNOWN;
  }
  virtual ADDON_STATUS CreateInstance(int instanceType,
                                      const std::string& instanceID,
                                      KODI_HANDLE instance,
                                      const std::string& version,
                                      KODI_HANDLE& addonInstance)
  {
    return ADDON_STATUS_NOT_IMPLEMENTED;
  }
  virtual void DestroyInstance(int instanceType,
                               const std::string& instanceID,
                               KODI_HANDLE addonInstance)
  {
  }

  /* Global variables of class */
  static AddonGlobalInterface*
      m_interface; // Interface function table to hold addresses on add-on and from kodi

private:
  static inline void ADDONBASE_Destroy()
  {
    delete static_cast<CAddonBaseHdl*>(m_interface->addonBase);
    m_interface->addonBase = nullptr;
  }

  static inline ADDON_STATUS ADDONBASE_SetSetting(const char* settingName, const void* settingValue)
  {
    return static_cast<CAddonBaseHdl*>(m_interface->addonBase)
        ->SetSetting(settingName, CSettingValue(settingValue));
  }

  static inline ADDON_STATUS ADDONBASE_CreateInstance(int instanceType,
                                                      const char* instanceID,
                                                      KODI_HANDLE instance,
                                                      const char* version,
                                                      KODI_HANDLE* addonInstance,
                                                      KODI_HANDLE parent)
  {
    CAddonBaseHdl* base = static_cast<CAddonBaseHdl*>(m_interface->addonBase);

    ADDON_STATUS status = ADDON_STATUS_NOT_IMPLEMENTED;

    /* Check about single instance usage:
     * 1. The kodi side instance pointer must be equal to first one
     * 2. The addon side instance pointer must be set
     * 3. And the requested type must be equal with used add-on class
     */
    if (m_interface->firstKodiInstance == instance && m_interface->globalSingleInstance &&
        static_cast<IAddonInstance*>(m_interface->globalSingleInstance)->m_type == instanceType)
    {
      /* The handling here is intended for the case of the add-on only one
       * instance and this is integrated in the add-on base class.
       */
      *addonInstance = m_interface->globalSingleInstance;
      status = ADDON_STATUS_OK;
    }
    else
    {
      /* Here it should use the CreateInstance instance function to allow
       * creation of several on one addon.
       */

      /* Check first a parent is defined about (e.g. Codec within inputstream) */
      if (parent != nullptr)
        status = static_cast<IAddonInstance*>(parent)->CreateInstance(
            instanceType, instanceID, instance, version, *addonInstance);

      /* if no parent call the main instance creation function to get it */
      if (status == ADDON_STATUS_NOT_IMPLEMENTED)
      {
        status = base->CreateInstance(instanceType, instanceID, instance, version, *addonInstance);
      }
    }

    if (*addonInstance == nullptr)
    {
      if (status == ADDON_STATUS_OK)
      {
        m_interface->toKodi->addon_log_msg(m_interface->toKodi->kodiBase, ADDON_LOG_FATAL,
                                           "kodi::addon::CAddonBaseHdl CreateInstance returned an "
                                           "empty instance pointer, but reported OK!");
        return ADDON_STATUS_PERMANENT_FAILURE;
      }
      else
      {
        return status;
      }
    }

    if (static_cast<IAddonInstance*>(*addonInstance)->m_type != instanceType)
    {
      m_interface->toKodi->addon_log_msg(
          m_interface->toKodi->kodiBase, ADDON_LOG_FATAL,
          "kodi::addon::CAddonBaseHdl CreateInstance difference between given and returned");
      delete static_cast<IAddonInstance*>(*addonInstance);
      *addonInstance = nullptr;
      return ADDON_STATUS_PERMANENT_FAILURE;
    }

    // Store the used ID inside instance, to have on destroy calls by addon to identify
    static_cast<IAddonInstance*>(*addonInstance)->m_id = instanceID;

    return status;
  }

  static inline void ADDONBASE_DestroyInstance(int instanceType, KODI_HANDLE instance)
  {
    CAddonBaseHdl* base = static_cast<CAddonBaseHdl*>(m_interface->addonBase);

    if (m_interface->globalSingleInstance == nullptr && instance != base)
    {
      base->DestroyInstance(instanceType, static_cast<IAddonInstance*>(instance)->m_id, instance);
      delete static_cast<IAddonInstance*>(instance);
    }
  }
};

AddonGlobalInterface* CAddonBaseHdl::m_interface = nullptr;

bool C_AddonBase_h::HandleMessage(int funcGroup,
                               int func,
                               const msgpack::unpacked& in,
                               msgpack::sbuffer& out,
                               const char* data,
                               std::size_t size,
                               std::size_t offset)
{
  if (funcGroup != funcGroup_AddonBase_h)
    return false;

  switch (func)
  {
    case funcParent_AddonBase_h::kodi_CAddonBase_constructor:
    {
      break;
    }
    case funcParent_AddonBase_h::kodi_CAddonBase_destructor:
    {
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(
          ADDON_LOG_ERROR,
          "C_AddonBase_h::%s: addon called with unknown function id '%i' on group 'kodi'", __func__,
          func);
      return false;
  }

  return true;
}

} // namespace parent
} // namespace sandbox
} /* namespace kodi */

extern "C" ATTRIBUTE_DLL_EXPORT ADDON_STATUS ADDON_Create(KODI_HANDLE addonInterface, const char* /*globalApiVersion*/, void* /*unused*/)
{
  kodi::sandbox::parent::CAddonBaseHdl::m_interface = static_cast<AddonGlobalInterface*>(addonInterface);
  kodi::sandbox::parent::CAddonBaseHdl::m_interface->addonBase = new kodi::sandbox::parent::CAddonBaseHdl;
  return static_cast<kodi::sandbox::parent::CAddonBaseHdl*>(kodi::sandbox::parent::CAddonBaseHdl::m_interface->addonBase)->Create();
}

extern "C" ATTRIBUTE_DLL_EXPORT const char* ADDON_GetTypeVersion(int type)
{
  return kodi::addon::GetTypeVersion(type);
}

extern "C" ATTRIBUTE_DLL_EXPORT const char* ADDON_GetTypeMinVersion(int type) \
{
  return kodi::addon::GetTypeMinVersion(type);
}


/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "Filesystem.h"

#include "../../sandbox/kodi/Filesystem.h"
#include "../utils/Log.h"

#include <kodi/Filesystem.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool CFilesystem_h::HandleMessage(int funcGroup,
                                  int func,
                                  const msgpack::unpacked& in,
                                  msgpack::sbuffer& out,
                                  const char* data,
                                  std::size_t size,
                                  std::size_t offset)
{
  if (funcGroup != funcGroup_Filesystem_h)
    return false;

  switch (func)
  {
    case funcParent_Filesystem_h::kodi_vfs_CreateDirectory:
    {
      msgParent__IN_kodi_vfs_CreateDirectory t = in.get().as<decltype(t)>();
      bool ret = kodi::vfs::CreateDirectory(std::get<0>(t));
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CreateDirectory(ret));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_DirectoryExists:
    {
      msgParent__IN_kodi_vfs_DirectoryExists t = in.get().as<decltype(t)>();
      bool ret = kodi::vfs::DirectoryExists(std::get<0>(t));
      msgpack::pack(out, msgParent_OUT_kodi_vfs_DirectoryExists(ret));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_RemoveDirectory:
    {
      msgParent__IN_kodi_vfs_RemoveDirectory t = in.get().as<decltype(t)>();
      bool ret = kodi::vfs::RemoveDirectory(std::get<0>(t), std::get<1>(t));
      msgpack::pack(out, msgParent_OUT_kodi_vfs_RemoveDirectory(ret));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_GetDirectory:
    {
      msgParent__IN_kodi_vfs_GetDirectory t = in.get().as<decltype(t)>();
      std::vector<kodi::vfs::CDirEntry> items;
      bool ret = kodi::vfs::GetDirectory(std::get<0>(t), std::get<1>(t), items);
      std::vector<std::tuple<std::string, std::string, std::string,
                             std::map<std::string, std::string>, bool, int64_t, time_t>>
          values;
      values.reserve(items.size());
      for (auto& item : items)
      {
        values.emplace_back(item.Label(), item.Title(), item.Path(), item.GetProperties(),
                            item.IsFolder(), item.Size(), item.DateTime());
      }

      msgpack::pack(out, msgParent_OUT_kodi_vfs_GetDirectory(ret, values));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_FileExists:
    {
      msgParent__IN_kodi_vfs_FileExists t = in.get().as<decltype(t)>();
      bool ret = kodi::vfs::FileExists(std::get<0>(t), std::get<1>(t));
      msgpack::pack(out, msgParent_OUT_kodi_vfs_FileExists(ret));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_StatFile:
    {
      msgParent__IN_kodi_vfs_StatFile t = in.get().as<decltype(t)>();
      const std::string& filename(std::get<0>(t));

      kodi::vfs::FileStatus buffer;
      bool ret = kodi::vfs::StatFile(filename, buffer);
      msgpack::pack(out, msgParent_OUT_kodi_vfs_StatFile(
                             ret, buffer.GetDeviceId(), buffer.GetSize(), buffer.GetAccessTime(),
                             buffer.GetModificationTime(), buffer.GetStatusTime(),
                             buffer.GetIsDirectory(), buffer.GetIsSymLink(), buffer.GetIsBlock(),
                             buffer.GetIsCharacter(), buffer.GetIsFifo(), buffer.GetIsRegular(),
                             buffer.GetIsSocket(), buffer.GetFileSerialNumber()));
    }
    case funcParent_Filesystem_h::kodi_vfs_DeleteFile:
    {
      msgParent__IN_kodi_vfs_DeleteFile t = in.get().as<decltype(t)>();
      bool ret = kodi::vfs::FileExists(std::get<0>(t));
      msgpack::pack(out, msgParent_OUT_kodi_vfs_DeleteFile(ret));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_RenameFile:
    {
      msgParent__IN_kodi_vfs_RenameFile t = in.get().as<decltype(t)>();
      bool ret = kodi::vfs::RenameFile(std::get<0>(t), std::get<1>(t));
      msgpack::pack(out, msgParent_OUT_kodi_vfs_RenameFile(ret));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CopyFile:
    {
      msgParent__IN_kodi_vfs_CopyFile t = in.get().as<decltype(t)>();
      bool ret = kodi::vfs::CopyFile(std::get<0>(t), std::get<1>(t));
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CopyFile(ret));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_GetFileMD5:
    {
      msgParent__IN_kodi_vfs_GetFileMD5 t = in.get().as<decltype(t)>();
      std::string ret = kodi::vfs::GetFileMD5(std::get<0>(t));
      msgpack::pack(out, msgParent_OUT_kodi_vfs_GetFileMD5(ret));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_GetCacheThumbName:
    {
      msgParent__IN_kodi_vfs_GetCacheThumbName t = in.get().as<decltype(t)>();
      std::string ret = kodi::vfs::GetCacheThumbName(std::get<0>(t));
      msgpack::pack(out, msgParent_OUT_kodi_vfs_GetCacheThumbName(ret));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_MakeLegalFileName:
    {
      msgParent__IN_kodi_vfs_MakeLegalFileName t = in.get().as<decltype(t)>();
      std::string ret = kodi::vfs::MakeLegalFileName(std::get<0>(t));
      msgpack::pack(out, msgParent_OUT_kodi_vfs_MakeLegalFileName(ret));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_MakeLegalPath:
    {
      msgParent__IN_kodi_vfs_MakeLegalPath t = in.get().as<decltype(t)>();
      std::string ret = kodi::vfs::MakeLegalPath(std::get<0>(t));
      msgpack::pack(out, msgParent_OUT_kodi_vfs_MakeLegalPath(ret));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_TranslateSpecialProtocol:
    {
      msgParent__IN_kodi_vfs_TranslateSpecialProtocol t = in.get().as<decltype(t)>();
      std::string ret = kodi::vfs::TranslateSpecialProtocol(std::get<0>(t));
      msgpack::pack(out, msgParent_OUT_kodi_vfs_TranslateSpecialProtocol(ret));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_GetDiskSpace:
    {
      msgParent__IN_kodi_vfs_GetDiskSpace t = in.get().as<decltype(t)>();
      const std::string& path = std::get<0>(t);
      uint64_t capacity = std::get<1>(t);
      uint64_t free = std::get<2>(t);
      uint64_t available = std::get<3>(t);
      bool ret = kodi::vfs::GetDiskSpace(path, capacity, free, available);
      msgpack::pack(out, msgParent_OUT_kodi_vfs_GetDiskSpace(ret, capacity, free, available));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_IsInternetStream:
    {
      msgParent__IN_kodi_vfs_IsInternetStream t = in.get().as<decltype(t)>();
      bool ret = kodi::vfs::IsInternetStream(std::get<0>(t));
      msgpack::pack(out, msgParent_OUT_kodi_vfs_IsInternetStream(ret));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_IsOnLAN:
    {
      msgParent__IN_kodi_vfs_IsOnLAN t = in.get().as<decltype(t)>();
      bool ret = kodi::vfs::IsOnLAN(std::get<0>(t));
      msgpack::pack(out, msgParent_OUT_kodi_vfs_IsOnLAN(ret));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_IsRemote:
    {
      msgParent__IN_kodi_vfs_IsRemote t = in.get().as<decltype(t)>();
      bool ret = kodi::vfs::IsRemote(std::get<0>(t));
      msgpack::pack(out, msgParent_OUT_kodi_vfs_IsRemote(ret));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_IsLocal:
    {
      msgParent__IN_kodi_vfs_IsLocal t = in.get().as<decltype(t)>();
      bool ret = kodi::vfs::IsLocal(std::get<0>(t));
      msgpack::pack(out, msgParent_OUT_kodi_vfs_IsLocal(ret));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_IsURL:
    {
      msgParent__IN_kodi_vfs_IsURL t = in.get().as<decltype(t)>();
      bool ret = kodi::vfs::IsURL(std::get<0>(t));
      msgpack::pack(out, msgParent_OUT_kodi_vfs_IsURL(ret));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_GetHttpHeader:
    {
      msgParent__IN_kodi_vfs_GetHttpHeader t = in.get().as<decltype(t)>();
      kodi::vfs::HttpHeader* header = reinterpret_cast<kodi::vfs::HttpHeader*>(std::get<1>(t));

      bool ret = kodi::vfs::GetHttpHeader(std::get<0>(t), *header);
      msgpack::pack(out, msgParent_OUT_kodi_vfs_GetHttpHeader(ret, uint64_t(header)));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_GetMimeType:
    {
      msgParent__IN_kodi_vfs_GetMimeType t = in.get().as<decltype(t)>();
      std::string retStr = std::get<1>(t);
      bool ret = kodi::vfs::GetMimeType(std::get<0>(t), retStr, std::get<2>(t));
      msgpack::pack(out, msgParent_OUT_kodi_vfs_GetMimeType(ret, retStr));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_GetContentType:
    {
      msgParent__IN_kodi_vfs_GetContentType t = in.get().as<decltype(t)>();
      std::string retStr = std::get<1>(t);
      bool ret = kodi::vfs::GetContentType(std::get<0>(t), retStr, std::get<2>(t));
      msgpack::pack(out, msgParent_OUT_kodi_vfs_GetContentType(ret, retStr));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_GetCookies:
    {
      msgParent__IN_kodi_vfs_GetCookies t = in.get().as<decltype(t)>();
      std::string retStr = std::get<1>(t);
      bool ret = kodi::vfs::GetCookies(std::get<0>(t), retStr);
      msgpack::pack(out, msgParent_OUT_kodi_vfs_GetCookies(ret, retStr));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_HttpHeader_construct:
    {
      msgParent__IN_kodi_vfs_HttpHeader_construct t = in.get().as<decltype(t)>();
      kodi::vfs::HttpHeader* header = new kodi::vfs::HttpHeader;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_HttpHeader_construct(uint64_t(header)));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_HttpHeader_destruct:
    {
      msgParent__IN_kodi_vfs_HttpHeader_destruct t = in.get().as<decltype(t)>();
      kodi::vfs::HttpHeader* header = reinterpret_cast<kodi::vfs::HttpHeader*>(std::get<0>(t));
      if (header == nullptr)
        break;
      delete header;
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_HttpHeader_GetValue:
    {
      msgParent__IN_kodi_vfs_HttpHeader_GetValue t = in.get().as<decltype(t)>();
      kodi::vfs::HttpHeader* header = reinterpret_cast<kodi::vfs::HttpHeader*>(std::get<0>(t));
      if (header == nullptr)
        break;
      msgpack::pack(out,
                    msgParent_OUT_kodi_vfs_HttpHeader_GetValue(header->GetValue(std::get<1>(t))));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_HttpHeader_GetValues:
    {
      msgParent__IN_kodi_vfs_HttpHeader_GetValues t = in.get().as<decltype(t)>();
      kodi::vfs::HttpHeader* header = reinterpret_cast<kodi::vfs::HttpHeader*>(std::get<0>(t));
      if (header == nullptr)
        break;
      msgpack::pack(out,
                    msgParent_OUT_kodi_vfs_HttpHeader_GetValues(header->GetValues(std::get<1>(t))));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_HttpHeader_GetHeader:
    {
      msgParent__IN_kodi_vfs_HttpHeader_GetHeader t = in.get().as<decltype(t)>();
      kodi::vfs::HttpHeader* header = reinterpret_cast<kodi::vfs::HttpHeader*>(std::get<0>(t));
      if (header == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_HttpHeader_GetHeader(header->GetHeader()));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_HttpHeader_GetMimeType:
    {
      msgParent__IN_kodi_vfs_HttpHeader_GetMimeType t = in.get().as<decltype(t)>();
      kodi::vfs::HttpHeader* header = reinterpret_cast<kodi::vfs::HttpHeader*>(std::get<0>(t));
      if (header == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_HttpHeader_GetMimeType(header->GetMimeType()));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_HttpHeader_GetCharset:
    {
      msgParent__IN_kodi_vfs_HttpHeader_GetCharset t = in.get().as<decltype(t)>();
      kodi::vfs::HttpHeader* header = reinterpret_cast<kodi::vfs::HttpHeader*>(std::get<0>(t));
      if (header == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_HttpHeader_GetCharset(header->GetCharset()));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_HttpHeader_GetProtoLine:
    {
      msgParent__IN_kodi_vfs_HttpHeader_GetProtoLine t = in.get().as<decltype(t)>();
      kodi::vfs::HttpHeader* header = reinterpret_cast<kodi::vfs::HttpHeader*>(std::get<0>(t));
      if (header == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_HttpHeader_GetProtoLine(header->GetProtoLine()));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_construct:
    {
      msgParent__IN_kodi_vfs_CFile_construct t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = new kodi::vfs::CFile;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CFile_construct(uint64_t(file)));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_destruct:
    {
      msgParent__IN_kodi_vfs_CFile_destruct t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      delete file;
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_OpenFile:
    {
      msgParent__IN_kodi_vfs_CFile_OpenFile t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CFile_OpenFile(
                             file->OpenFile(std::get<1>(t), std::get<2>(t))));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_OpenFileForWrite:
    {
      msgParent__IN_kodi_vfs_CFile_OpenFileForWrite t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CFile_OpenFileForWrite(
                             file->OpenFileForWrite(std::get<1>(t), std::get<2>(t))));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_Close:
    {
      msgParent__IN_kodi_vfs_CFile_Close t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      file->Close();
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_CURLCreate:
    {
      msgParent__IN_kodi_vfs_CFile_CURLCreate t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CFile_CURLCreate(file->CURLCreate(std::get<1>(t))));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_CURLAddOption:
    {
      msgParent__IN_kodi_vfs_CFile_CURLAddOption t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CFile_CURLAddOption(file->CURLAddOption(
                             CURLOptiontype(std::get<1>(t)), std::get<2>(t), std::get<2>(t))));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_CURLOpen:
    {
      msgParent__IN_kodi_vfs_CFile_CURLOpen t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CFile_CURLOpen(file->CURLOpen(std::get<1>(t))));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_Read:
    {
      msgParent__IN_kodi_vfs_CFile_Read t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;

      std::vector<int8_t> data;
      data.resize(std::get<1>(t));
      size_t size = file->Read(data.data(), data.size());
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CFile_Read(size, data));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_ReadLine:
    {
      msgParent__IN_kodi_vfs_CFile_ReadLine t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;

      std::string line = std::get<1>(t);
      bool ret = file->ReadLine(line);
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CFile_ReadLine(ret, line));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_Write:
    {
      msgParent__IN_kodi_vfs_CFile_Write t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;

      const std::vector<int8_t>& data = std::get<1>(t);
      size_t size = file->Write(data.data(), data.size());
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CFile_Write(size));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_Flush:
    {
      msgParent__IN_kodi_vfs_CFile_Flush t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      file->Flush();
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_Seek:
    {
      msgParent__IN_kodi_vfs_CFile_Seek t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      msgpack::pack(out,
                    msgParent_OUT_kodi_vfs_CFile_Seek(file->Seek(std::get<1>(t), std::get<2>(t))));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_Truncate:
    {
      msgParent__IN_kodi_vfs_CFile_Truncate t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CFile_Truncate(file->Truncate(std::get<1>(t))));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_GetPosition:
    {
      msgParent__IN_kodi_vfs_CFile_GetPosition t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CFile_GetPosition(file->GetPosition()));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_GetLength:
    {
      msgParent__IN_kodi_vfs_CFile_GetLength t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CFile_GetLength(file->GetLength()));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_AtEnd:
    {
      msgParent__IN_kodi_vfs_CFile_AtEnd t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CFile_AtEnd(file->AtEnd()));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_GetChunkSize:
    {
      msgParent__IN_kodi_vfs_CFile_GetChunkSize t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CFile_GetChunkSize(file->GetChunkSize()));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_IoControlGetSeekPossible:
    {
      msgParent__IN_kodi_vfs_CFile_IoControlGetSeekPossible t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CFile_IoControlGetSeekPossible(
                             file->IoControlGetSeekPossible()));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_IoControlGetCacheStatus:
    {
      msgParent__IN_kodi_vfs_CFile_IoControlGetCacheStatus t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      kodi::vfs::CacheStatus status;
      status.SetForward(std::get<1>(t));
      status.SetMaxRate(std::get<2>(t));
      status.SetCurrentRate(std::get<3>(t));
      status.SetLowspeed(std::get<4>(t));
      bool ret = file->IoControlGetCacheStatus(status);
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CFile_IoControlGetCacheStatus(
                             ret, status.GetForward(), status.GetMaxRate(), status.GetCurrentRate(),
                             status.GetLowspeed()));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_IoControlSetCacheRate:
    {
      msgParent__IN_kodi_vfs_CFile_IoControlSetCacheRate t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CFile_IoControlSetCacheRate(
                             file->IoControlSetCacheRate(std::get<1>(t))));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_IoControlSetRetry:
    {
      msgParent__IN_kodi_vfs_CFile_IoControlSetRetry t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CFile_IoControlSetRetry(
                             file->IoControlSetRetry(std::get<1>(t))));
      break;
    }

    case funcParent_Filesystem_h::kodi_vfs_CFile_GetPropertyValue:
    {
      msgParent__IN_kodi_vfs_CFile_GetPropertyValue t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CFile_GetPropertyValue(file->GetPropertyValue(
                             FilePropertyTypes(std::get<1>(t)), std::get<2>(t))));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_GetPropertyValues:
    {
      msgParent__IN_kodi_vfs_CFile_GetPropertyValues t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_vfs_CFile_GetPropertyValues(file->GetPropertyValues(
                             FilePropertyTypes(std::get<1>(t)), std::get<2>(t))));
      break;
    }
    case funcParent_Filesystem_h::kodi_vfs_CFile_GetFileDownloadSpeed:
    {
      msgParent__IN_kodi_vfs_CFile_GetFileDownloadSpeed t = in.get().as<decltype(t)>();
      kodi::vfs::CFile* file = reinterpret_cast<kodi::vfs::CFile*>(std::get<0>(t));
      if (file == nullptr)
        break;
      msgpack::pack(
          out, msgParent_OUT_kodi_vfs_CFile_GetFileDownloadSpeed(file->GetFileDownloadSpeed()));
      break;
    }

    default:
      kodi::utils::LOG_MESSAGE(
          ADDON_LOG_ERROR,
          "CFilesystem_h::%s: addon called with unknown function id '%i' on group 'kodi'", __func__,
          func);
      return false;
  }

  return true;
}

} // namespace parent
} // namespace sandbox
} /* namespace kodi */

/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "../sandbox/kodi/Filesystem.h"

#include "../../include/kodi/Filesystem.h"
#include "../../include/kodi/General.h"
#include "../../include/kodi/Processor.h"
#include "../sandbox/ShareProcessTransmitter.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace vfs
{

HttpHeader::HttpHeader()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_HttpHeader_construct));
  msgpack::pack(in, msgParent__IN_kodi_vfs_HttpHeader_construct(uint64_t(this)));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_HttpHeader_construct t = ident.get().as<decltype(t)>();
  m_handle = std::get<0>(t);
}

HttpHeader::~HttpHeader()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_HttpHeader_destruct));
  msgpack::pack(in, msgParent__IN_kodi_vfs_HttpHeader_destruct(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
}

std::string HttpHeader::GetValue(const std::string& param) const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_HttpHeader_GetValue));
  msgpack::pack(in, msgParent__IN_kodi_vfs_HttpHeader_GetValue(m_handle, param));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_HttpHeader_GetValue t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

std::vector<std::string> HttpHeader::GetValues(const std::string& param) const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_HttpHeader_GetValues));
  msgpack::pack(in, msgParent__IN_kodi_vfs_HttpHeader_GetValues(m_handle, param));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_HttpHeader_GetValues t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

std::string HttpHeader::GetHeader() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_HttpHeader_GetHeader));
  msgpack::pack(in, msgParent__IN_kodi_vfs_HttpHeader_GetHeader(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_HttpHeader_GetHeader t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

std::string HttpHeader::GetMimeType() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_HttpHeader_GetMimeType));
  msgpack::pack(in, msgParent__IN_kodi_vfs_HttpHeader_GetMimeType(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_HttpHeader_GetMimeType t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

std::string HttpHeader::GetCharset() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_HttpHeader_GetCharset));
  msgpack::pack(in, msgParent__IN_kodi_vfs_HttpHeader_GetCharset(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_HttpHeader_GetCharset t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

std::string HttpHeader::GetProtoLine() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_HttpHeader_GetProtoLine));
  msgpack::pack(in, msgParent__IN_kodi_vfs_HttpHeader_GetProtoLine(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_HttpHeader_GetProtoLine t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool CreateDirectory(const std::string& path)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_CreateDirectory));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CreateDirectory(path));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CreateDirectory t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool DirectoryExists(const std::string& path)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_DirectoryExists));
  msgpack::pack(in, msgParent__IN_kodi_vfs_DirectoryExists(path));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_DirectoryExists t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool RemoveDirectory(const std::string& path, bool recursive)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_RemoveDirectory));
  msgpack::pack(in, msgParent__IN_kodi_vfs_RemoveDirectory(path, recursive));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_RemoveDirectory t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool GetDirectory(const std::string& path,
                  const std::string& mask,
                  std::vector<kodi::vfs::CDirEntry>& items)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_GetDirectory));
  msgpack::pack(in, msgParent__IN_kodi_vfs_GetDirectory(path, mask));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_GetDirectory t = ident.get().as<decltype(t)>();

  for (const auto& entry : std::get<1>(t))
    items.emplace_back(std::get<0>(entry), std::get<1>(entry), std::get<2>(entry),
                       std::get<4>(entry), std::get<5>(entry), std::get<6>(entry),
                       std::get<3>(entry));
  return std::get<0>(t);
}

bool FileExists(const std::string& path, bool usecache)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_FileExists));
  msgpack::pack(in, msgParent__IN_kodi_vfs_FileExists(path, usecache));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_FileExists t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool StatFile(const std::string& filename, kodi::vfs::FileStatus& buffer)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_StatFile));
  msgpack::pack(in, msgParent__IN_kodi_vfs_StatFile(filename));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_StatFile t = ident.get().as<decltype(t)>();
  bool ret = std::get<0>(t);
  buffer.SetDeviceId(std::get<1>(t));
  buffer.SetSize(std::get<2>(t));
  buffer.SetAccessTime(std::get<3>(t));
  buffer.SetModificationTime(std::get<4>(t));
  buffer.SetStatusTime(std::get<4>(t));
  buffer.SetIsDirectory(std::get<5>(t));
  buffer.SetIsSymLink(std::get<6>(t));
  buffer.SetIsBlock(std::get<7>(t));
  buffer.SetIsCharacter(std::get<8>(t));
  buffer.SetIsFifo(std::get<9>(t));
  buffer.SetIsRegular(std::get<10>(t));
  buffer.SetIsSocket(std::get<11>(t));
  buffer.SetFileSerialNumber(std::get<12>(t));

  return ret;
}

bool DeleteFile(const std::string& filename)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_DeleteFile));
  msgpack::pack(in, msgParent__IN_kodi_vfs_DeleteFile(filename));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_DeleteFile t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool RenameFile(const std::string& filename, const std::string& newFileName)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_RenameFile));
  msgpack::pack(in, msgParent__IN_kodi_vfs_RenameFile(filename, newFileName));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_DeleteFile t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool CopyFile(const std::string& filename, const std::string& destination)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_CopyFile));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CopyFile(filename, destination));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_DeleteFile t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

std::string GetFileMD5(const std::string& path)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_GetFileMD5));
  msgpack::pack(in, msgParent__IN_kodi_vfs_GetFileMD5(path));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_GetFileMD5 t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

std::string GetCacheThumbName(const std::string& filename)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_GetCacheThumbName));
  msgpack::pack(in, msgParent__IN_kodi_vfs_GetCacheThumbName(filename));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_GetCacheThumbName t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

std::string MakeLegalFileName(const std::string& filename)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_MakeLegalFileName));
  msgpack::pack(in, msgParent__IN_kodi_vfs_MakeLegalFileName(filename));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_MakeLegalFileName t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

std::string MakeLegalPath(const std::string& path)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_MakeLegalPath));
  msgpack::pack(in, msgParent__IN_kodi_vfs_MakeLegalPath(path));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_MakeLegalPath t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

std::string TranslateSpecialProtocol(const std::string& source)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_TranslateSpecialProtocol));
  msgpack::pack(in, msgParent__IN_kodi_vfs_TranslateSpecialProtocol(source));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_TranslateSpecialProtocol t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool GetDiskSpace(const std::string& path, uint64_t& capacity, uint64_t& free, uint64_t& available)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_GetDiskSpace));
  msgpack::pack(in, msgParent__IN_kodi_vfs_GetDiskSpace(path, capacity, free, available));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_GetDiskSpace t = ident.get().as<decltype(t)>();
  capacity = std::get<1>(t);
  free = std::get<2>(t);
  available = std::get<3>(t);
  return std::get<0>(t);
}

std::string GetFileName(const std::string& path)
{
  /* find the last slash */
  const size_t slash = path.find_last_of("/\\");
  return path.substr(slash + 1);
}

std::string GetDirectoryName(const std::string& path)
{
  // Will from a full filename return the directory the file resides in.
  // Keeps the final slash at end and possible |option=foo options.

  size_t iPosSlash = path.find_last_of("/\\");
  if (iPosSlash == std::string::npos)
    return ""; // No slash, so no path (ignore any options)

  size_t iPosBar = path.rfind('|');
  if (iPosBar == std::string::npos)
    return path.substr(0, iPosSlash + 1); // Only path

  return path.substr(0, iPosSlash + 1) + path.substr(iPosBar); // Path + options
}

void RemoveSlashAtEnd(std::string& path)
{
  if (!path.empty())
  {
    char last = path[path.size() - 1];
    if (last == '/' || last == '\\')
      path.erase(path.size() - 1);
  }
}

unsigned int GetChunkSize(unsigned int chunk, unsigned int minimum)
{
  if (chunk)
    return chunk * ((minimum + chunk - 1) / chunk);
  else
    return minimum;
}

bool IsInternetStream(const std::string& path, bool strictCheck)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_IsInternetStream));
  msgpack::pack(in, msgParent__IN_kodi_vfs_IsInternetStream(path, strictCheck));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_IsInternetStream t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool IsOnLAN(const std::string& path)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_IsOnLAN));
  msgpack::pack(in, msgParent__IN_kodi_vfs_IsOnLAN(path));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_IsOnLAN t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool IsRemote(const std::string& path)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_IsRemote));
  msgpack::pack(in, msgParent__IN_kodi_vfs_IsRemote(path));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_IsRemote t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool IsLocal(const std::string& path)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_IsLocal));
  msgpack::pack(in, msgParent__IN_kodi_vfs_IsLocal(path));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_IsLocal t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool IsURL(const std::string& path)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_IsURL));
  msgpack::pack(in, msgParent__IN_kodi_vfs_IsURL(path));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_IsURL t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool GetHttpHeader(const std::string& url, HttpHeader& header)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_GetHttpHeader));
  msgpack::pack(in, msgParent__IN_kodi_vfs_GetHttpHeader(url, header.m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_GetHttpHeader t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool GetMimeType(const std::string& url, std::string& mimeType, const std::string& useragent)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_GetMimeType));
  msgpack::pack(in, msgParent__IN_kodi_vfs_GetMimeType(url, mimeType, useragent));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_GetMimeType t = ident.get().as<decltype(t)>();
  mimeType = std::get<1>(t);
  return std::get<0>(t);
}

bool GetContentType(const std::string& url, std::string& content, const std::string& useragent)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_GetContentType));
  msgpack::pack(in, msgParent__IN_kodi_vfs_GetContentType(url, content, useragent));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_GetContentType t = ident.get().as<decltype(t)>();
  content = std::get<1>(t);
  return std::get<0>(t);
}

bool GetCookies(const std::string& url, std::string& cookies)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_GetCookies));
  msgpack::pack(in, msgParent__IN_kodi_vfs_GetCookies(url, cookies));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_GetCookies t = ident.get().as<decltype(t)>();
  cookies = std::get<1>(t);
  return std::get<0>(t);
}


CFile::CFile()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_CFile_construct));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_construct(uint64_t(this)));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_construct t = ident.get().as<decltype(t)>();
  m_file = std::get<0>(t);
}

CFile::~CFile()
{
  msgpack::unpacked ident;
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_CFile_destruct));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_destruct(m_file));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
}

bool CFile::OpenFile(const std::string& filename, unsigned int flags)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_CFile_OpenFile));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_OpenFile(m_file, filename, flags));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_OpenFile t = ident.get().as<decltype(t)>();
  m_isOpen = std::get<0>(t);
  return m_isOpen;
}

bool CFile::OpenFileForWrite(const std::string& filename, bool overwrite)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_CFile_OpenFileForWrite));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_OpenFileForWrite(m_file, filename, overwrite));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_OpenFileForWrite t = ident.get().as<decltype(t)>();
  m_isOpen = std::get<0>(t);
  return m_isOpen;
}

void CFile::Close()
{
  if (!m_isOpen)
    return;

  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_CFile_Close));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_Close(m_file));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

bool CFile::CURLCreate(const std::string& url)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_CFile_CURLCreate));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_CURLCreate(m_file, url));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_CURLCreate t = ident.get().as<decltype(t)>();
  m_isOpen = std::get<0>(t);
  return m_isOpen;
}

bool CFile::CURLAddOption(CURLOptiontype type, const std::string& name, const std::string& value)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_CFile_CURLAddOption));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_CURLAddOption(m_file, type, name, value));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_CURLAddOption t = ident.get().as<decltype(t)>();
  m_isOpen = std::get<0>(t);
  return m_isOpen;
}

bool CFile::CURLOpen(unsigned int flags)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_CFile_CURLOpen));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_CURLOpen(m_file, flags));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_CURLOpen t = ident.get().as<decltype(t)>();
  m_isOpen = std::get<0>(t);
  return m_isOpen;
}

ssize_t CFile::Read(void* ptr, size_t size)
{
  if (!m_isOpen)
    return -1;

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_CFile_Read));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_Read(m_file, size));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_Read t = ident.get().as<decltype(t)>();
  ssize_t ret = std::get<0>(t);
  const std::vector<int8_t>& data = std::get<1>(t);
  for (size_t i = 0; i < ret; ++i)
    static_cast<int8_t*>(ptr)[i] = data[i];
  return ret;
}

bool CFile::ReadLine(std::string& line)
{
  if (!m_isOpen)
    return false;

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_CFile_ReadLine));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_ReadLine(m_file, line));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_ReadLine t = ident.get().as<decltype(t)>();
  line = std::get<1>(t);
  return std::get<0>(t);
}

ssize_t CFile::Write(const void* ptr, size_t size)
{
  if (!m_isOpen)
    return -1;

  std::vector<int8_t> data;
  data.resize(size);
  for (unsigned int i = 0; i < size; ++i)
    data[i] = static_cast<const int8_t*>(ptr)[i];

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_CFile_Write));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_Write(m_file, data));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_Write t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CFile::Flush()
{
  if (!m_isOpen)
    return;

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_CFile_Flush));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_Flush(m_file));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
}

int64_t CFile::Seek(int64_t position, int whence)
{
  if (!m_isOpen)
    return -1;

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_CFile_Seek));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_Seek(m_file, position, whence));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_Seek t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

int CFile::Truncate(int64_t size)
{
  if (!m_isOpen)
    return -1;

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_CFile_Truncate));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_Truncate(m_file, size));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_Truncate t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

int64_t CFile::GetPosition() const
{
  if (!m_isOpen)
    return -1;

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_CFile_GetPosition));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_GetPosition(m_file));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_GetPosition t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

int64_t CFile::GetLength() const
{
  if (!m_isOpen)
    return -1;

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_CFile_GetLength));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_GetLength(m_file));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_GetLength t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool CFile::AtEnd() const
{
  if (!m_isOpen)
    return true;

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Filesystem_h, funcParent_Filesystem_h::kodi_vfs_CFile_AtEnd));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_AtEnd(m_file));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_AtEnd t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

int64_t CFile::GetChunkSize() const
{
  if (!m_isOpen)
    return -1;

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_CFile_GetChunkSize));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_GetChunkSize(m_file));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_GetChunkSize t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool CFile::IoControlGetSeekPossible() const
{
  if (!m_isOpen)
    return true;

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_Filesystem_h,
                              funcParent_Filesystem_h::kodi_vfs_CFile_IoControlGetSeekPossible));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_IoControlGetSeekPossible(m_file));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_IoControlGetSeekPossible t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool CFile::IoControlGetCacheStatus(CacheStatus& status) const
{
  if (!m_isOpen)
    return true;

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_CFile_IoControlGetCacheStatus));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_IoControlGetCacheStatus(
                        m_file, status.GetForward(), status.GetMaxRate(), status.GetCurrentRate(),
                        status.GetLowspeed()));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_IoControlGetCacheStatus t = ident.get().as<decltype(t)>();

  status.SetForward(std::get<1>(t));
  status.SetMaxRate(std::get<2>(t));
  status.SetCurrentRate(std::get<3>(t));
  status.SetLowspeed(std::get<4>(t));
  return std::get<0>(t);
}

bool CFile::IoControlSetCacheRate(unsigned int rate)
{
  if (!m_isOpen)
    return true;

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_CFile_IoControlSetCacheRate));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_IoControlSetCacheRate(m_file, rate));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_IoControlSetCacheRate t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool CFile::IoControlSetRetry(bool retry)
{
  if (!m_isOpen)
    return true;

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_CFile_IoControlSetRetry));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_IoControlSetRetry(m_file, retry));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_IoControlSetRetry t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

const std::string CFile::GetPropertyValue(FilePropertyTypes type, const std::string& name) const
{
  if (!m_isOpen)
  {
    kodi::Log(ADDON_LOG_ERROR,
              "kodi::vfs::CURLCreate(...) needed to call before GetPropertyValues!");
    return "";
  }

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_CFile_GetPropertyValue));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_GetPropertyValue(m_file, type, name));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_GetPropertyValue t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

const std::vector<std::string> CFile::GetPropertyValues(FilePropertyTypes type,
                                                        const std::string& name) const
{
  if (!m_isOpen)
  {
    kodi::Log(ADDON_LOG_ERROR,
              "kodi::vfs::CURLCreate(...) needed to call before GetPropertyValues!");
    return std::vector<std::string>();
  }

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_CFile_GetPropertyValues));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_GetPropertyValues(m_file, type, name));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_GetPropertyValues t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

double CFile::GetFileDownloadSpeed() const
{
  if (!m_isOpen)
    return 0.0;

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_Filesystem_h,
                                  funcParent_Filesystem_h::kodi_vfs_CFile_GetFileDownloadSpeed));
  msgpack::pack(in, msgParent__IN_kodi_vfs_CFile_GetFileDownloadSpeed(m_file));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_vfs_CFile_GetFileDownloadSpeed t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

} /* namespace vfs */
} /* namespace kodi */

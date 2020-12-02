/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "c-api/filesystem.h"

#include <map>
#include <string>
#include <vector>

namespace kodi
{
namespace vfs
{

class FileStatus
{
public:
  FileStatus() = default;

  void SetDeviceId(uint32_t deviceId) { m_deviceId = deviceId; }
  uint32_t GetDeviceId() const { return m_deviceId; }
  void SetFileSerialNumber(uint64_t fileSerialNumber) { m_fileSerialNumber = fileSerialNumber; }
  uint64_t GetFileSerialNumber() const { return m_fileSerialNumber; }
  void SetSize(uint64_t size) { m_size = size; }
  uint64_t GetSize() const { return m_size; }
  void SetAccessTime(time_t accessTime) { m_accessTime = accessTime; }
  time_t GetAccessTime() const { return m_accessTime; }
  void SetModificationTime(time_t modificationTime) { m_modificationTime = modificationTime; }
  time_t GetModificationTime() const { return m_modificationTime; }
  void SetStatusTime(time_t statusTime) { m_statusTime = statusTime; }
  time_t GetStatusTime() const { return m_statusTime; }
  void SetIsDirectory(bool isDirectory) { m_isDirectory = isDirectory; }
  bool GetIsDirectory() const { return m_isDirectory; }
  void SetIsSymLink(bool isSymLink) { m_isSymLink = isSymLink; }
  bool GetIsSymLink() const { return m_isSymLink; }
  void SetIsBlock(bool isBlock) { m_isBlock = isBlock; }
  bool GetIsBlock() const { return m_isBlock; }
  void SetIsCharacter(bool isCharacter) { m_isCharacter = isCharacter; }
  bool GetIsCharacter() const { return m_isCharacter; }
  void SetIsFifo(bool isFifo) { m_isFifo = isFifo; }
  bool GetIsFifo() const { return m_isFifo; }
  void SetIsRegular(bool isRegular) { m_isRegular = isRegular; }
  bool GetIsRegular() const { return m_isRegular; }
  void SetIsSocket(bool isSocket) { m_isSocket = isSocket; }
  bool GetIsSocket() const { return m_isSocket; }

private:
  uint32_t m_deviceId = 0;
  uint64_t m_size = 0;
  time_t m_accessTime = 0;
  time_t m_modificationTime = 0;
  time_t m_statusTime = 0;
  bool m_isDirectory = false;
  bool m_isSymLink = false;
  bool m_isBlock = false;
  bool m_isCharacter = false;
  bool m_isFifo = false;
  bool m_isRegular = false;
  bool m_isSocket = false;
  uint64_t m_fileSerialNumber = 0;
};

class CacheStatus
{
public:
  CacheStatus() = default;

  void SetForward(uint64_t forward) { m_forward = forward; }
  uint64_t GetForward() { return m_forward; }
  void SetMaxRate(unsigned int maxrate) { m_maxrate = maxrate; }
  unsigned int GetMaxRate() { return m_maxrate; }
  void SetCurrentRate(unsigned int currate) { m_currate = currate; }
  unsigned int GetCurrentRate() { return m_currate; }
  void SetLowspeed(bool lowspeed) { m_lowspeed = lowspeed; }
  bool GetLowspeed() { return m_lowspeed; }

private:
  uint64_t m_forward = 0;
  unsigned int m_maxrate = 0;
  unsigned int m_currate = 0;
  bool m_lowspeed = false;
};

class HttpHeader
{
public:
  HttpHeader();
  ~HttpHeader();

  std::string GetValue(const std::string& param) const;
  std::vector<std::string> GetValues(const std::string& param) const;
  std::string GetHeader() const;
  std::string GetMimeType() const;
  std::string GetCharset() const;
  std::string GetProtoLine() const;

  uint64_t m_handle;
};

class CDirEntry
{
public:
  CDirEntry(
      const std::string& label = "",
      const std::string& title = "",
      const std::string& path = "",
      bool folder = false,
      int64_t size = -1,
      time_t dateTime = 0,
      const std::map<std::string, std::string>& properties = std::map<std::string, std::string>())
    : m_label(label),
      m_title(title),
      m_path(path),
      m_properties(properties),
      m_folder(folder),
      m_size(size),
      m_dateTime(dateTime)
  {
  }

  const std::string& Label(void) const { return m_label; }
  const std::string& Title(void) const { return m_title; }
  const std::string& Path(void) const { return m_path; }
  bool IsFolder(void) const { return m_folder; }
  int64_t Size(void) const { return m_size; }
  time_t DateTime() { return m_dateTime; }
  void SetLabel(const std::string& label) { m_label = label; }
  void SetTitle(const std::string& title) { m_title = title; }
  void SetPath(const std::string& path) { m_path = path; }
  void SetFolder(bool folder) { m_folder = folder; }
  void SetSize(int64_t size) { m_size = size; }
  void SetDateTime(time_t dateTime) { m_dateTime = dateTime; }
  void AddProperty(const std::string& id, const std::string& value) { m_properties[id] = value; }
  void ClearProperties() { m_properties.clear(); }
  const std::map<std::string, std::string>& GetProperties() const { return m_properties; }

private:
  std::string m_label;
  std::string m_title;
  std::string m_path;
  std::map<std::string, std::string> m_properties;
  bool m_folder = false;
  int64_t m_size = -1;
  time_t m_dateTime = 0;
};

bool CreateDirectory(const std::string& path);
bool DirectoryExists(const std::string& path);
bool RemoveDirectory(const std::string& path, bool recursive = false);
bool GetDirectory(const std::string& path,
                  const std::string& mask,
                  std::vector<kodi::vfs::CDirEntry>& items);
bool FileExists(const std::string& filename, bool usecache = false);
bool StatFile(const std::string& filename, kodi::vfs::FileStatus& buffer);
bool DeleteFile(const std::string& filename);
bool RenameFile(const std::string& filename, const std::string& newFileName);
bool CopyFile(const std::string& filename, const std::string& destination);
std::string GetFileMD5(const std::string& path);
std::string GetCacheThumbName(const std::string& filename);
std::string MakeLegalFileName(const std::string& filename);
std::string MakeLegalPath(const std::string& path);
std::string TranslateSpecialProtocol(const std::string& source);
bool GetDiskSpace(const std::string& path, uint64_t& capacity, uint64_t& free, uint64_t& available);
std::string GetFileName(const std::string& path);
std::string GetDirectoryName(const std::string& path);
void RemoveSlashAtEnd(std::string& path);
unsigned int GetChunkSize(unsigned int chunk, unsigned int minimum);
bool IsInternetStream(const std::string& path, bool strictCheck = false);
bool IsOnLAN(const std::string& path);
bool IsRemote(const std::string& path);
bool IsLocal(const std::string& path);
bool IsURL(const std::string& path);
bool GetHttpHeader(const std::string& url, HttpHeader& header);
bool GetMimeType(const std::string& url, std::string& mimeType, const std::string& useragent = "");
bool GetContentType(const std::string& url,
                    std::string& content,
                    const std::string& useragent = "");
bool GetCookies(const std::string& url, std::string& cookies);

class CFile
{
public:
  CFile();
  virtual ~CFile();
  bool OpenFile(const std::string& filename, unsigned int flags = 0);
  bool OpenFileForWrite(const std::string& filename, bool overwrite = false);
  void Close();
  bool IsOpen() const { return m_isOpen; }
  bool CURLCreate(const std::string& url);
  bool CURLAddOption(CURLOptiontype type, const std::string& name, const std::string& value);
  bool CURLOpen(unsigned int flags = 0);
  ssize_t Read(void* ptr, size_t size);
  bool ReadLine(std::string& line);
  ssize_t Write(const void* ptr, size_t size);
  void Flush();
  int64_t Seek(int64_t position, int whence = SEEK_SET);
  int Truncate(int64_t size);
  int64_t GetPosition() const;
  int64_t GetLength() const;
  bool AtEnd() const;
  int64_t GetChunkSize() const;
  bool IoControlGetSeekPossible() const;
  bool IoControlGetCacheStatus(CacheStatus& status) const;
  bool IoControlSetCacheRate(unsigned int rate);
  bool IoControlSetRetry(bool retry);
  const std::string GetPropertyValue(FilePropertyTypes type, const std::string& name) const;
  const std::vector<std::string> GetPropertyValues(FilePropertyTypes type,
                                                   const std::string& name) const;
  double GetFileDownloadSpeed() const;

private:
  bool m_isOpen = false;
  uint64_t m_file = 0;
};

} /* namespace vfs */
} /* namespace kodi */

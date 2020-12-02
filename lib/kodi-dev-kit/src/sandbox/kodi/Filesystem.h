/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../include/kodi/c-api/filesystem.h"

#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_Filesystem_h : int
{
  kodi_vfs_CreateDirectory = 0,
  kodi_vfs_DirectoryExists,
  kodi_vfs_RemoveDirectory,
  kodi_vfs_GetDirectory,
  kodi_vfs_FileExists,
  kodi_vfs_StatFile,
  kodi_vfs_DeleteFile,
  kodi_vfs_RenameFile,
  kodi_vfs_CopyFile,
  kodi_vfs_GetFileMD5,
  kodi_vfs_GetCacheThumbName,
  kodi_vfs_MakeLegalFileName,
  kodi_vfs_MakeLegalPath,
  kodi_vfs_TranslateSpecialProtocol,
  kodi_vfs_GetDiskSpace,
  kodi_vfs_IsInternetStream,
  kodi_vfs_IsOnLAN,
  kodi_vfs_IsRemote,
  kodi_vfs_IsLocal,
  kodi_vfs_IsURL,
  kodi_vfs_GetHttpHeader,
  kodi_vfs_GetMimeType,
  kodi_vfs_GetContentType,
  kodi_vfs_GetCookies,
  kodi_vfs_HttpHeader_construct,
  kodi_vfs_HttpHeader_destruct,
  kodi_vfs_HttpHeader_GetValue,
  kodi_vfs_HttpHeader_GetValues,
  kodi_vfs_HttpHeader_GetHeader,
  kodi_vfs_HttpHeader_GetMimeType,
  kodi_vfs_HttpHeader_GetCharset,
  kodi_vfs_HttpHeader_GetProtoLine,
  kodi_vfs_CFile_construct,
  kodi_vfs_CFile_destruct,
  kodi_vfs_CFile_OpenFile,
  kodi_vfs_CFile_OpenFileForWrite,
  kodi_vfs_CFile_Close,
  kodi_vfs_CFile_CURLCreate,
  kodi_vfs_CFile_CURLAddOption,
  kodi_vfs_CFile_CURLOpen,
  kodi_vfs_CFile_Read,
  kodi_vfs_CFile_ReadLine,
  kodi_vfs_CFile_Write,
  kodi_vfs_CFile_Flush,
  kodi_vfs_CFile_Seek,
  kodi_vfs_CFile_Truncate,
  kodi_vfs_CFile_GetPosition,
  kodi_vfs_CFile_GetLength,
  kodi_vfs_CFile_AtEnd,
  kodi_vfs_CFile_GetChunkSize,
  kodi_vfs_CFile_IoControlGetSeekPossible,
  kodi_vfs_CFile_IoControlGetCacheStatus,
  kodi_vfs_CFile_IoControlSetCacheRate,
  kodi_vfs_CFile_IoControlSetRetry,
  kodi_vfs_CFile_GetPropertyValue,
  kodi_vfs_CFile_GetPropertyValues,
  kodi_vfs_CFile_GetFileDownloadSpeed,
} funcParent_Filesystem_h;

// clang-format off
typedef std::tuple<std::string>                                                 msgParent__IN_kodi_vfs_CreateDirectory;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_CreateDirectory;
typedef std::tuple<std::string>                                                 msgParent__IN_kodi_vfs_DirectoryExists;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_DirectoryExists;
typedef std::tuple<std::string, bool>                                           msgParent__IN_kodi_vfs_RemoveDirectory;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_RemoveDirectory;
typedef std::tuple<std::string, std::string>                                    msgParent__IN_kodi_vfs_GetDirectory;
typedef std::tuple<bool, std::vector<std::tuple<std::string, std::string,
                   std::string, std::map<std::string, std::string>,
                   bool, int64_t, time_t>>>                                     msgParent_OUT_kodi_vfs_GetDirectory;
typedef std::tuple<std::string>                                                 msgParent__IN_kodi_vfs_StatFile;
typedef std::tuple<bool, uint32_t, uint64_t, time_t, time_t, time_t,
                   bool, bool, bool, bool, bool, bool, bool, uint64_t>          msgParent_OUT_kodi_vfs_StatFile;
typedef std::tuple<std::string, bool>                                           msgParent__IN_kodi_vfs_FileExists;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_FileExists;
typedef std::tuple<std::string>                                                 msgParent__IN_kodi_vfs_DeleteFile;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_DeleteFile;
typedef std::tuple<std::string, std::string>                                    msgParent__IN_kodi_vfs_RenameFile;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_RenameFile;
typedef std::tuple<std::string, std::string>                                    msgParent__IN_kodi_vfs_CopyFile;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_CopyFile;
typedef std::tuple<std::string>                                                 msgParent__IN_kodi_vfs_GetFileMD5;
typedef std::tuple<std::string>                                                 msgParent_OUT_kodi_vfs_GetFileMD5;

typedef std::tuple<std::string>                                                 msgParent__IN_kodi_vfs_GetCacheThumbName;
typedef std::tuple<std::string>                                                 msgParent_OUT_kodi_vfs_GetCacheThumbName;
typedef std::tuple<std::string>                                                 msgParent__IN_kodi_vfs_MakeLegalFileName;
typedef std::tuple<std::string>                                                 msgParent_OUT_kodi_vfs_MakeLegalFileName;
typedef std::tuple<std::string>                                                 msgParent__IN_kodi_vfs_MakeLegalPath;
typedef std::tuple<std::string>                                                 msgParent_OUT_kodi_vfs_MakeLegalPath;
typedef std::tuple<std::string>                                                 msgParent__IN_kodi_vfs_TranslateSpecialProtocol;
typedef std::tuple<std::string>                                                 msgParent_OUT_kodi_vfs_TranslateSpecialProtocol;
typedef std::tuple<std::string, uint64_t, uint64_t, uint64_t>                   msgParent__IN_kodi_vfs_GetDiskSpace;
typedef std::tuple<bool, uint64_t, uint64_t, uint64_t>                          msgParent_OUT_kodi_vfs_GetDiskSpace;
typedef std::tuple<std::string, bool>                                           msgParent__IN_kodi_vfs_IsInternetStream;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_IsInternetStream;
typedef std::tuple<std::string>                                                 msgParent__IN_kodi_vfs_IsOnLAN;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_IsOnLAN;
typedef std::tuple<std::string>                                                 msgParent__IN_kodi_vfs_IsRemote;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_IsRemote;
typedef std::tuple<std::string>                                                 msgParent__IN_kodi_vfs_IsLocal;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_IsLocal;
typedef std::tuple<std::string>                                                 msgParent__IN_kodi_vfs_IsURL;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_IsURL;
typedef std::tuple<std::string, uint64_t>                                       msgParent__IN_kodi_vfs_GetHttpHeader;
typedef std::tuple<bool, uint64_t>                                              msgParent_OUT_kodi_vfs_GetHttpHeader;
typedef std::tuple<std::string, std::string, std::string>                       msgParent__IN_kodi_vfs_GetMimeType;
typedef std::tuple<bool, std::string>                                           msgParent_OUT_kodi_vfs_GetMimeType;
typedef std::tuple<std::string, std::string, std::string>                       msgParent__IN_kodi_vfs_GetContentType;
typedef std::tuple<bool, std::string>                                           msgParent_OUT_kodi_vfs_GetContentType;
typedef std::tuple<std::string, std::string>                                    msgParent__IN_kodi_vfs_GetCookies;
typedef std::tuple<bool, std::string>                                           msgParent_OUT_kodi_vfs_GetCookies;

typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_vfs_HttpHeader_construct;
typedef std::tuple<uint64_t>                                                    msgParent_OUT_kodi_vfs_HttpHeader_construct;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_vfs_HttpHeader_destruct;
typedef std::tuple<uint64_t>                                                    msgParent_OUT_kodi_vfs_HttpHeader_destruct;
typedef std::tuple<uint64_t, std::string>                                       msgParent__IN_kodi_vfs_HttpHeader_GetValue;
typedef std::tuple<std::string>                                                 msgParent_OUT_kodi_vfs_HttpHeader_GetValue;
typedef std::tuple<uint64_t, std::string>                                       msgParent__IN_kodi_vfs_HttpHeader_GetValues;
typedef std::tuple<std::vector<std::string>>                                    msgParent_OUT_kodi_vfs_HttpHeader_GetValues;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_vfs_HttpHeader_GetHeader;
typedef std::tuple<std::string>                                                 msgParent_OUT_kodi_vfs_HttpHeader_GetHeader;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_vfs_HttpHeader_GetMimeType;
typedef std::tuple<std::string>                                                 msgParent_OUT_kodi_vfs_HttpHeader_GetMimeType;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_vfs_HttpHeader_GetCharset;
typedef std::tuple<std::string>                                                 msgParent_OUT_kodi_vfs_HttpHeader_GetCharset;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_vfs_HttpHeader_GetProtoLine;
typedef std::tuple<std::string>                                                 msgParent_OUT_kodi_vfs_HttpHeader_GetProtoLine;

typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_vfs_CFile_construct;
typedef std::tuple<uint64_t>                                                    msgParent_OUT_kodi_vfs_CFile_construct;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_vfs_CFile_destruct;
typedef std::tuple<uint64_t>                                                    msgParent_OUT_kodi_vfs_CFile_destruct;
typedef std::tuple<uint64_t, std::string, unsigned int>                         msgParent__IN_kodi_vfs_CFile_OpenFile;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_CFile_OpenFile;
typedef std::tuple<uint64_t, std::string, bool>                                 msgParent__IN_kodi_vfs_CFile_OpenFileForWrite;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_CFile_OpenFileForWrite;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_vfs_CFile_Close;
typedef std::tuple<uint64_t>                                                    msgParent_OUT_kodi_vfs_CFile_Close;
typedef std::tuple<uint64_t, std::string>                                       msgParent__IN_kodi_vfs_CFile_CURLCreate;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_CFile_CURLCreate;
typedef std::tuple<uint64_t, int, std::string, std::string>                     msgParent__IN_kodi_vfs_CFile_CURLAddOption;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_CFile_CURLAddOption;
typedef std::tuple<uint64_t, unsigned int>                                      msgParent__IN_kodi_vfs_CFile_CURLOpen;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_CFile_CURLOpen;
typedef std::tuple<uint64_t, size_t>                                            msgParent__IN_kodi_vfs_CFile_Read;
typedef std::tuple<size_t, std::vector<int8_t>>                                 msgParent_OUT_kodi_vfs_CFile_Read;
typedef std::tuple<uint64_t, std::string>                                       msgParent__IN_kodi_vfs_CFile_ReadLine;
typedef std::tuple<bool, std::string>                                           msgParent_OUT_kodi_vfs_CFile_ReadLine;
typedef std::tuple<uint64_t, std::vector<int8_t>>                               msgParent__IN_kodi_vfs_CFile_Write;
typedef std::tuple<size_t>                                                      msgParent_OUT_kodi_vfs_CFile_Write;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_vfs_CFile_Flush;
typedef std::tuple<uint64_t>                                                    msgParent_OUT_kodi_vfs_CFile_Flush; // unused
typedef std::tuple<uint64_t, int64_t, int>                                      msgParent__IN_kodi_vfs_CFile_Seek;
typedef std::tuple<int64_t>                                                     msgParent_OUT_kodi_vfs_CFile_Seek;
typedef std::tuple<uint64_t, int64_t>                                           msgParent__IN_kodi_vfs_CFile_Truncate;
typedef std::tuple<int>                                                         msgParent_OUT_kodi_vfs_CFile_Truncate;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_vfs_CFile_GetPosition;
typedef std::tuple<int64_t>                                                     msgParent_OUT_kodi_vfs_CFile_GetPosition;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_vfs_CFile_GetLength;
typedef std::tuple<int64_t>                                                     msgParent_OUT_kodi_vfs_CFile_GetLength;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_vfs_CFile_AtEnd;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_CFile_AtEnd;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_vfs_CFile_GetChunkSize;
typedef std::tuple<int>                                                         msgParent_OUT_kodi_vfs_CFile_GetChunkSize;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_vfs_CFile_IoControlGetSeekPossible;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_CFile_IoControlGetSeekPossible;
typedef std::tuple<uint64_t, uint64_t, unsigned int, unsigned int, bool>        msgParent__IN_kodi_vfs_CFile_IoControlGetCacheStatus;
typedef std::tuple<bool, uint64_t, unsigned int, unsigned int, bool>            msgParent_OUT_kodi_vfs_CFile_IoControlGetCacheStatus;
typedef std::tuple<uint64_t, unsigned int>                                      msgParent__IN_kodi_vfs_CFile_IoControlSetCacheRate;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_CFile_IoControlSetCacheRate;
typedef std::tuple<uint64_t, bool>                                              msgParent__IN_kodi_vfs_CFile_IoControlSetRetry;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_vfs_CFile_IoControlSetRetry;
typedef std::tuple<uint64_t, int, std::string>                                  msgParent__IN_kodi_vfs_CFile_GetPropertyValue;
typedef std::tuple<std::string>                                                 msgParent_OUT_kodi_vfs_CFile_GetPropertyValue;
typedef std::tuple<uint64_t, int, std::string>                                  msgParent__IN_kodi_vfs_CFile_GetPropertyValues;
typedef std::tuple<std::vector<std::string>>                                    msgParent_OUT_kodi_vfs_CFile_GetPropertyValues;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_vfs_CFile_GetFileDownloadSpeed;
typedef std::tuple<double>                                                      msgParent_OUT_kodi_vfs_CFile_GetFileDownloadSpeed;
// clang-format on

//==============================================================================

typedef enum funcChild_Filesystem_h
{
  funcIdAddon_unused_Filesystem_h
} funcChild_Filesystem_h;

} /* namespace sandbox */
} /* namespace kodi */

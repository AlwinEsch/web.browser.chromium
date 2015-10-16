###############################################################################
#                                                                             #
#     Copyright (C) 2015 Team KODI                                            #
#     http://kodi.tv                                                          #
#                                                                             #
#  This program is free software: you can redistribute it and/or modify       #
#  it under the terms of the GNU General Public License as published by       #
#  the Free Software Foundation, either version 3 of the License, or          #
#  (at your option) any later version.                                        #
#                                                                             #
#  This program is distributed in the hope that it will be useful,            #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of             #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              #
#  GNU General Public License for more details.                               #
#                                                                             #
#  You should have received a copy of the GNU General Public License          #
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.      #
#                                                                             #
###############################################################################

# Don't allow binary install for CEF (does not support empty INSTALL_COMMAND in system)
set(cef-binary_NO_INSTALL 1)

include(color-defaults)

# TODO: improve way for binary package check
if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/cef-binary-packages")
  execute_process(COMMAND cmake -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/cef-binary-packages)
endif()
execute_process(
  COMMAND git clone https://github.com/kodi-web/cef-binary-packages.git
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
)

if(NOT CEF_BINARY)
  if(${CORE_SYSTEM_NAME} STREQUAL linux AND ${BITSIZE} STREQUAL 32)
    set(CEF_BINARY i686-linux-gnu)
  elseif(${CORE_SYSTEM_NAME} STREQUAL linux AND ${BITSIZE} STREQUAL 64)
    set(CEF_BINARY x86_64-linux-gnu)
  elseif(${CORE_SYSTEM_NAME} STREQUAL osx AND ${BITSIZE} STREQUAL 32)
    set(CEF_BINARY i386-apple-darwin)
  elseif(${CORE_SYSTEM_NAME} STREQUAL osx AND ${BITSIZE} STREQUAL 64)
    set(CEF_BINARY x86_64-apple-darwin)
  elseif(${CORE_SYSTEM_NAME} STREQUAL ios)
    set(CEF_BINARY arm-apple-darwin)
  elseif(${CORE_SYSTEM_NAME} STREQUAL windows AND ${BITSIZE} STREQUAL 32)
    set(CEF_BINARY i686-windows)
  elseif(${CORE_SYSTEM_NAME} STREQUAL windows AND ${BITSIZE} STREQUAL 64)
    set(CEF_BINARY x86_64-windows)
  elseif(${CORE_SYSTEM_NAME} STREQUAL rbpi)
    set(CEF_BINARY arm-linux-gnueabihf)
  endif()
endif()

#set(CEF_BINARY arm-linux-androideabi)
#set(CEF_BINARY arm-linux-gnueabi)
#set(CEF_BINARY i686-linux-android)

if(NOT CEF_BINARY)
  message(FATAL_ERROR "${BoldWhite}Missing 'CEF_BINARY' value !!!${ColourReset}")
endif()

file(READ ${CMAKE_CURRENT_BINARY_DIR}/cef-binary-packages/x86_64-linux-gnu/binary-package.txt BINARY_PACKAGE_ZIP)
STRING(REGEX REPLACE "\n" "" BINARY_PACKAGE_ZIP "${BINARY_PACKAGE_ZIP}")
file(READ ${CMAKE_CURRENT_BINARY_DIR}/cef-binary-packages/x86_64-linux-gnu/md5sum.txt BINARY_PACKAGE_MD5_SUM)
STRING(REGEX REPLACE "\n" "" BINARY_PACKAGE_MD5_SUM "${BINARY_PACKAGE_MD5_SUM}")
add_revision(cef-binary
  URL ${BINARY_PACKAGE_ZIP}
  URL_MD5 ${BINARY_PACKAGE_MD5_SUM}
)

add_external_project(cef-binary)
ExternalProject_Get_Property(cef-binary SOURCE_DIR)
ExternalProject_Get_Property(cef-binary BINARY_DIR)

set_include_dir(cef-binary ${SOURCE_DIR})
set_libraries(cef-binary cef)

add_custom_command(TARGET cef-binary
  # Static libs which becomes added
  COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/libcef_dll/${CMAKE_STATIC_LIBRARY_PREFIX}cef_dll_wrapper${CMAKE_STATIC_LIBRARY_SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}

  # Shared libs and parts which need present on add-on
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/share/cef
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/share/cef/locales
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${BINARY_DIR}/Resources/locales            ${CMAKE_CURRENT_BINARY_DIR}/share/cef/locales
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/cef.pak                      ${CMAKE_CURRENT_BINARY_DIR}/share/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/cef_100_percent.pak          ${CMAKE_CURRENT_BINARY_DIR}/share/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/cef_200_percent.pak          ${CMAKE_CURRENT_BINARY_DIR}/share/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/devtools_resources.pak       ${CMAKE_CURRENT_BINARY_DIR}/share/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/icudtl.dat                   ${CMAKE_CURRENT_BINARY_DIR}/share/cef

  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_SHARED_LIBRARY_PREFIX}cef${CMAKE_SHARED_LIBRARY_SUFFIX}        ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_SHARED_LIBRARY_PREFIX}ffmpegsumo${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/${CMAKE_BUILD_TYPE}/chrome-sandbox     ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/${CMAKE_BUILD_TYPE}/natives_blob.bin   ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/${CMAKE_BUILD_TYPE}/snapshot_blob.bin  ${CMAKE_CURRENT_BINARY_DIR}/lib/cef

  # TODO: Fix installed library load
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_SHARED_LIBRARY_PREFIX}cef${CMAKE_SHARED_LIBRARY_SUFFIX}        ${CMAKE_CURRENT_BINARY_DIR}/share/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_SHARED_LIBRARY_PREFIX}ffmpegsumo${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}/share/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/${CMAKE_BUILD_TYPE}/chrome-sandbox     ${CMAKE_CURRENT_BINARY_DIR}/share/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/${CMAKE_BUILD_TYPE}/natives_blob.bin   ${CMAKE_CURRENT_BINARY_DIR}/share/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/${CMAKE_BUILD_TYPE}/snapshot_blob.bin  ${CMAKE_CURRENT_BINARY_DIR}/share/cef
)

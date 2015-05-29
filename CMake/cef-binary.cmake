###############################################################################
#                                                                             #
#     Copyright (C) 2015 Team KODI                                            #
#     http://kodi.tv                                                          #
#                                                                             #
# This Program is free software; you can redistribute it and/or modify        #
# it under the terms of the GNU General Public License as published by        #
# the Free Software Foundation; either version 2, or (at your option)         #
# any later version.                                                          #
#                                                                             #
# This Program is distributed in the hope that it will be useful,             #
# but WITHOUT ANY WARRANTY; without even the implied warranty of              #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                #
# GNU General Public License for more details.                                #
#                                                                             #
# You should have received a copy of the GNU General Public License           #
# along with KODI; see the file COPYING.  If not, see                         #
# <http://www.gnu.org/licenses/>.                                             #
#                                                                             #
###############################################################################

# Don't allow binary install for CEF (does not support empty INSTALL_COMMAND in system)
set(cef-binary_NO_INSTALL 1)

include(color-defaults)

if(CEF_COMPLETE)
  if(CEF_NO_DEBUG_BUILD)
    list(APPEND CEF_BUILD_VAL "--no-debug-build ")
  endif()
  if(CEF_NO_RELEASE_BUILD)
    list(APPEND CEF_BUILD_VAL "--no-release-build ")
  endif()

  set(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/CEFComplete/chromium/src/cef/binary_distrib/cef_binary_3.${CEF_COMPLETE_VERSION}_${CEF_OS_NAME}${BITSIZE})

  message(STATUS "Created CEF binary ${BoldWhite}cef_binary_3.${CEF_COMPLETE_VERSION}_${CEF_OS_NAME}${BITSIZE}.zip${ColourReset} becomes stored at ${BoldWhite}${CMAKE_CURRENT_BINARY_DIR}${ColourReset}")

  add_external_project(cef-binary
    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/cef-binary/src/cef-binary
    DOWNLOAD_COMMAND python ${PROJECT_SOURCE_DIR}/CEFParts/cef-automate-git-for-kodi.py
                            --download-dir=${CMAKE_CURRENT_BINARY_DIR}/CEFComplete
                            --branch=${CEF_BRANCH_VERSION}
                            --add-kodi-patch=${PROJECT_SOURCE_DIR}/CEFParts/${CEF_KODI_CHANGE_PATCH}
                            --build-log-file ${CEF_BUILD_VAL}
    UPDATE_COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE} ${CMAKE_CURRENT_BINARY_DIR}/cef-binary/src/cef-binary
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE}.zip ${CMAKE_CURRENT_BINARY_DIR}
  )
else()
  add_external_project(cef-binary)
endif()

ExternalProject_Get_Property(cef-binary SOURCE_DIR)
ExternalProject_Get_Property(cef-binary BINARY_DIR)

set_include_dir(cef-binary ${SOURCE_DIR})
set_libraries(cef-binary cef)

add_custom_command(TARGET cef-binary
  # Static libs which becomes added
  COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/libcef_dll/${CMAKE_STATIC_LIBRARY_PREFIX}cef_dll_wrapper${CMAKE_STATIC_LIBRARY_SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}

  # Shared libs and parts which need present on add-on
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/cef
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/cef/locales
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${BINARY_DIR}/cefsimple/${CMAKE_BUILD_TYPE}/locales ${CMAKE_CURRENT_BINARY_DIR}/cef/locales
  COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/cefsimple/${CMAKE_BUILD_TYPE}/${CMAKE_SHARED_LIBRARY_PREFIX}cef${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/cefsimple/${CMAKE_BUILD_TYPE}/cef_100_percent.pak ${CMAKE_CURRENT_BINARY_DIR}/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/cefsimple/${CMAKE_BUILD_TYPE}/cef_200_percent.pak ${CMAKE_CURRENT_BINARY_DIR}/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/cefsimple/${CMAKE_BUILD_TYPE}/cef.pak ${CMAKE_CURRENT_BINARY_DIR}/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/cefsimple/${CMAKE_BUILD_TYPE}/chrome-sandbox ${CMAKE_CURRENT_BINARY_DIR}/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/cefsimple/${CMAKE_BUILD_TYPE}/devtools_resources.pak ${CMAKE_CURRENT_BINARY_DIR}/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/cefsimple/${CMAKE_BUILD_TYPE}/icudtl.dat ${CMAKE_CURRENT_BINARY_DIR}/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/cefsimple/${CMAKE_BUILD_TYPE}/${CMAKE_SHARED_LIBRARY_PREFIX}ffmpegsumo${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/cefsimple/${CMAKE_BUILD_TYPE}/natives_blob.bin ${CMAKE_CURRENT_BINARY_DIR}/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/cefsimple/${CMAKE_BUILD_TYPE}/snapshot_blob.bin ${CMAKE_CURRENT_BINARY_DIR}/cef
)

cmake_minimum_required(VERSION 3.5)

#file(GLOB_RECURSE CEF_LIB ${PROJECT_BINARY_DIR} FOLLOW_SYMLINKS libcef_dll_wrapper*)

find_library(CEF_LIB NAMES libcef_dll_wrapper${CMAKE_STATIC_LIBRARY_SUFFIX} HINTS ${PROJECT_BINARY_DIR}/libcef_dll_wrapper/*)

file(COPY ${CEF_LIB} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

message(FATAL_ERROR "--------${CMAKE_CURRENT_BINARY_DIR}")
#set(CEF_DIR ${SOURCE_DIR}/cmake)
#find_package(CEF REQUIRED PATHS ${SOURCE_DIR}/cmake)

#[[COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/libcef_dll_wrapper/libcef_dll_wrapper${CMAKE_STATIC_LIBRARY_SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}

# Shared libs and parts which need present on add-on
COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/share/cef
COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/share/cef/locales
COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_DIR}/Resources/locales      ${CMAKE_CURRENT_BINARY_DIR}/share/cef/locales
COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/cef.pak                ${CMAKE_CURRENT_BINARY_DIR}/share/cef
COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/cef_100_percent.pak    ${CMAKE_CURRENT_BINARY_DIR}/share/cef
COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/cef_200_percent.pak    ${CMAKE_CURRENT_BINARY_DIR}/share/cef
COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/cef_extensions.pak     ${CMAKE_CURRENT_BINARY_DIR}/share/cef
COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/devtools_resources.pak ${CMAKE_CURRENT_BINARY_DIR}/share/cef

COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Release/${CMAKE_SHARED_LIBRARY_PREFIX}cef${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Release/chrome-sandbox           ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Release/natives_blob.bin         ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Release/snapshot_blob.bin        ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Release/v8_context_snapshot.bin  ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/icudtl.dat             ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_DIR}/Release                ${CMAKE_CURRENT_BINARY_DIR}/lib/cef]]
# This file becomes used from CMakeLists.txt by ExternalProject
# cmake -P copy-cef.cmake call.

# Read in generated file the current version to find previous generated package
# folder.
file(STRINGS ${PROJECT_BINARY_DIR}/cef_version.txt CEF_VERSION)

if(CORE_SYSTEM_NAME STREQUAL osx)
  set(PACKAGE_PLACE "${PROJECT_BINARY_DIR}/chromium_package/cef_binary_${CEF_VERSION}_macosx${CEF_BIT_STR}")
else()
  set(PACKAGE_PLACE "${PROJECT_BINARY_DIR}/chromium_package/cef_binary_${CEF_VERSION}_${CORE_SYSTEM_NAME}${CEF_BIT_STR}")
endif()

message(STATUS "Used package: ${PACKAGE_PLACE}")
message(STATUS "   copied to: ${SOURCE_DIR}")

# Copy the by Chromium CEF generated package to Kodi's addon build depends folder.
file(COPY ${PACKAGE_PLACE}/ DESTINATION ${SOURCE_DIR})

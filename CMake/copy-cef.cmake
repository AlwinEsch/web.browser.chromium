file(STRINGS ${PROJECT_BINARY_DIR}/cef_version.txt CEF_VERSION)

set(PACKAGE_PLACE "${PROJECT_BINARY_DIR}/chromium_package/cef_binary_${CEF_VERSION}_${CORE_SYSTEM_NAME}${CEF_BIT_STR}_package")

message(STATUS "Used package: ${PACKAGE_PLACE}")
message(STATUS "   copied to: ${SOURCE_DIR}/cef")

file(COPY ${PACKAGE_PLACE}/ DESTINATION ${SOURCE_DIR}/cef)

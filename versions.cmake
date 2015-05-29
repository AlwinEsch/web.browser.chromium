# This maintains the links for all sources used by this superbuild.
# Simply update this file to change the revision.
# One can use different revision on different platforms.
# e.g.
# if (UNIX)
#   ..
# else (APPLE)
#   ..
# endif()

set(CEF_COMPLETE_VERSION 2357.1274.g7b49af6)
if(CEF_COMPLETE)
  set(CEF_OWN_CHANGES_VERSION "0.0.2")
  set(CEF_BRANCH_VERSION 2357)
  set(CEF_KODI_CHANGE_PATCH "CEF-${CEF_COMPLETE_VERSION}-Add-Kodi-related-changes-Version-${CEF_OWN_CHANGES_VERSION}.patch")

  # CEF_OS_NAME and BITSIZE are defined by CMakeLists.txt
  add_revision(cef-binary
  )
else()
  # CEF_OS_NAME and BITSIZE are defined by CMakeLists.txt
  add_revision(cef-binary
    URL http://esmasol.de/data/documents/cef_binary_3_${CEF_COMPLETE_VERSION}_${CEF_OS_NAME}${BITSIZE}-${CEF_OWN_CHANGES_VERSION}.zip
    URL_MD5 130e29b59186b52b66e89543f260cd09
  )
endif()

# This maintains the links for all sources used by this superbuild.
# Simply update this file to change the revision.
# One can use different revision on different platforms.
# e.g.
# if (UNIX)
#   ..
# else (APPLE)
#   ..
# endif()

set(CEF_COMPLETE_VERSION "2357.1275.gd2d36ca")
set(CEF_OWN_CHANGES_VERSION "0.0.2")
if(CEF_COMPLETE)
  set(CEF_BRANCH_VERSION 2357)
  set(CEF_BRANCH_URL https://bitbucket.org/EsMaSol/cef.git)
  add_revision(cef-binary)
else()
  # CEF_OS_NAME and BITSIZE are defined by CMakeLists.txt
  add_revision(cef-binary
    URL http://esmasol.de/data/documents/kodi-web-${CEF_OWN_CHANGES_VERSION}_cef_binary_3_${CEF_COMPLETE_VERSION}_${CEF_OS_NAME}${BITSIZE}.zip
    URL_MD5 1570985420a5b6beb285b13f66facd14
  )
endif()

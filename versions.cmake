# This maintains the links for all sources used by this superbuild.
# Simply update this file to change the revision.
# One can use different revision on different platforms.
# e.g.
# if (UNIX)
#   ..
# else (APPLE)
#   ..
# endif()

set(CEF_COMPLETE_VERSION "3.2357.1293.g905e6a6")
set(CEF_OWN_CHANGES_VERSION "0.0.6")
set(CEF_TYPE "release")

# Set needed MD5 for OS part
if(OS_WINDOWS)
  if(${BITSIZE} STREQUAL 32)
    message(FATAL_ERROR "${BoldWhite}OS ${BoldRed}Windows 32 bit ${BoldWhite}currently not supported !!!${ColourReset}")
  elseif(${BITSIZE} STREQUAL 64)
    message(FATAL_ERROR "${BoldWhite}OS ${BoldRed}Windows 64 bit ${BoldWhite}currently not supported !!!${ColourReset}")
  else()
    message(FATAL_ERROR "${BoldWhite}Missing bit size for OS ${BoldRed}Windows${BoldWhite} !!!${ColourReset}")
  endif()
elseif(OS_LINUX)
  if(${BITSIZE} STREQUAL 32)
    set(CEF_BINARY_MD5 340e9f073da49fc69f1ae5c8d55ce71b)
  elseif(${BITSIZE} STREQUAL 64)
    set(CEF_BINARY_MD5 46ea362faaa4ac05eb1fc7daf0eb3fd1)
  else()
    message(FATAL_ERROR "${BoldWhite}Missing bit size for OS ${BoldRed}Linux${BoldWhite} !!!${ColourReset}")
  endif()
elseif(OS_MACOSX)
  if(${BITSIZE} STREQUAL 64)
    message(FATAL_ERROR "${BoldWhite}OS ${BoldRed}Mac OS X 64 bit ${BoldWhite}currently not supported !!!${ColourReset}")
  elseif(${BITSIZE} STREQUAL 32)
    message(FATAL_ERROR "${BoldWhite}OS ${BoldRed}Mac OS X 32 bit ${BoldWhite}currently not supported !!!${ColourReset}")
  else()
    message(FATAL_ERROR "${BoldWhite}Missing bit size for OS ${BoldRed}Mac OS X${BoldWhite} !!!${ColourReset}")
  endif()
endif()

# CEF_OS_NAME and BITSIZE are defined by CMakeLists.txt
add_revision(cef-binary
  URL https://bitbucket.org/kodi-web/cef/downloads/kodi-web-${CEF_TYPE}-${CEF_OWN_CHANGES_VERSION}_cef_binary_${CEF_COMPLETE_VERSION}_${CEF_OS_NAME}${BITSIZE}.zip
  URL_MD5 ${CEF_BINARY_MD5}
)

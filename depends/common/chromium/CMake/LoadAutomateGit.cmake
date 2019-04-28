if(AUTOMATE_GIT_BRANCH)
  set(AUTOMATE_GIT_URL "https://raw.githubusercontent.com/kodi-web/cef/${AUTOMATE_GIT_BRANCH}/tools/automate/automate-git.py")
else()
  set(AUTOMATE_GIT_URL "https://raw.githubusercontent.com/kodi-web/cef/kodi/tools/automate/automate-git.py" CACHE STRING "URL to automate-git.py")
endif()

if(NOT AUTOMATE_GIT_DEST)
  message(FATAL_ERROR "AUTOMATE_GIT_DEST not defined!")
endif()

file(DOWNLOAD ${AUTOMATE_GIT_URL} ${AUTOMATE_GIT_DEST}/automate-git.py)

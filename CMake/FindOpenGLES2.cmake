# - Try to find OpenGLES2
# Once done this will define
#
#  OPENGLES2_FOUND        - system has OpenGLES2
#  OPENGLES2_INCLUDE_DIR  - the GLES2 include directory
#  OPENGLES2_LIBRARIES    - Link these to use OpenGLES2

find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(OpenGLES2 glesv2)
endif(PKG_CONFIG_FOUND)

if(NOT OPENGLES2_FOUND)
  if("${CORE_SYSTEM_NAME}" STREQUAL "ios")
    find_library(OPENGLES2_gl_LIBRARY NAMES OpenGLES PATHS ${CMAKE_OSX_SYSROOT}/System/Library PATH_SUFFIXES Frameworks NO_DEFAULT_PATH)
    set(OPENGLES2_INCLUDE_DIRS ${OPENGLES2_gl_LIBRARY}/Headers)
    set(OPENGLES2_egl_LIBRARY ${OPENGLES2_gl_LIBRARY})
  else()
    find_path(OPENGLES2_INCLUDE_DIRS GLES2/gl2.h)
    find_library(OPENGLES2_gl_LIBRARY NAMES GLESv2)
    find_library(OPENGLES2_egl_LIBRARY NAMES EGL)
  endif()

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(OpenGLES2 DEFAULT_MSG
    OPENGLES2_INCLUDE_DIRS OPENGLES2_gl_LIBRARY OPENGLES2_egl_LIBRARY)

  set(OPENGLES2_LIBRARIES ${OPENGLES2_gl_LIBRARY} ${OPENGLES2_egl_LIBRARY})
endif(NOT OPENGLES2_FOUND)

mark_as_advanced(
  OPENGLES2_INCLUDE_DIRS
  OPENGLES2_LIBRARIES
  OPENGLES2_gl_LIBRARY
  OPENGLES2_egl_LIBRARY
)

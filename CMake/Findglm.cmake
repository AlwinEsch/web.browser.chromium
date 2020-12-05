#.rst:
# Findglm
# ------------
# Finds the OpenGL Mathematics (GLM) as a header only C++ mathematics library.
#
# This will define the following variables:
#
# GLM_FOUND - system has OpenGLES
# GLM_INCLUDE_DIR - the OpenGLES include directory
#
# Note: Install was removed from GLM on version 0.9.9.6.

find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PC_GLM glm QUIET)
endif()

find_path(GLM_INCLUDE_DIR glm.hpp
                          PATHS ${PC_GLM_INCLUDEDIR}
                          PATH_SUFFIXES glm)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(glm REQUIRED_VARS GLM_INCLUDE_DIR)

mark_as_advanced(GLM_INCLUDE_DIR)

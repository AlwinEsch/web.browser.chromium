add_library(devkit_sandbox INTERFACE)

# cmake_policy(SET CMP0076 NEW)


set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/SharedMemControl.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/ShareProcessReceiver.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/ShareProcessTransmitter.cpp)
set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/shared_data_structure.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/AddonBase.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/AudioEngine.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/Filesystem.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/General.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/Network.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/General.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/ListItem.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/Window.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/controls/Button.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/controls/Edit.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/controls/FadeLabel.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/controls/Image.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/controls/Label.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/controls/Progress.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/controls/RadioButton.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/controls/Rendering.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/controls/SettingsSlider.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/controls/Slider.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/controls/Spin.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/controls/TextBox.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/dialogs/ContextMenu.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/dialogs/ExtendedProgress.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/dialogs/FileBrowser.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/dialogs/Keyboard.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/dialogs/Numeric.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/dialogs/OK.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/dialogs/Progress.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/dialogs/Select.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/dialogs/TextViewer.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/dialogs/YesNo.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/input/Action.h
            ${CMAKE_CURRENT_SOURCE_DIR}/kodi/gui/input/ActionIDs.h
            ${CMAKE_CURRENT_SOURCE_DIR}/SharedMemControl.h
            ${CMAKE_CURRENT_SOURCE_DIR}/shared_process_func_child.h
            ${CMAKE_CURRENT_SOURCE_DIR}/shared_process_func_parent.h
            ${CMAKE_CURRENT_SOURCE_DIR}/ShareProcessReceiver.h
            ${CMAKE_CURRENT_SOURCE_DIR}/ShareProcessTransmitter.h)

set(SOURCES_POSIX ${CMAKE_CURRENT_SOURCE_DIR}/SharedMemControlPosixMMAP.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLExtensionCheck.cpp)
set(HEADERS_POSIX ${CMAKE_CURRENT_SOURCE_DIR}/SharedMemControlPosixMMAP.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLExtensionCheck.h)

set(SOURCES_WINDOWS ${CMAKE_CURRENT_SOURCE_DIR}/SharedMemControlWindows.cpp)
set(HEADERS_WINDOWS ${CMAKE_CURRENT_SOURCE_DIR}/SharedMemControlWindows.h)

target_sources(devkit_sandbox INTERFACE ${SOURCES}
                                        ${HEADERS})

if (WIN32 OR WINDOWS_STORE)
  target_sources(devkit_sandbox INTERFACE ${SOURCES_WINDOWS})
  target_sources(devkit_sandbox INTERFACE ${HEADERS_WINDOWS})
else()
  target_sources(devkit_sandbox INTERFACE ${SOURCES_POSIX})
  target_sources(devkit_sandbox INTERFACE ${HEADERS_POSIX})
endif()

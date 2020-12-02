set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/AddonBaseInternal.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/Processor.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/AudioEngine.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/Filesystem.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/General.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/Network.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/addon-instance/Web.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/General.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/ListItem.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/Window.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/controls/Button.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/controls/Edit.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/controls/FadeLabel.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/controls/Image.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/controls/Label.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/controls/Progress.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/controls/RadioButton.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/controls/Rendering.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/controls/SettingsSlider.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/controls/Slider.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/controls/Spin.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/controls/TextBox.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/dialogs/ContextMenu.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/dialogs/ExtendedProgress.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/dialogs/FileBrowser.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/dialogs/Keyboard.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/dialogs/Numeric.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/dialogs/OK.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/dialogs/Progress.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/dialogs/Select.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/dialogs/TextViewer.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/dialogs/YesNo.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/input/Action.cpp
            )

set(HEADERS AddonBaseInternal.h
            ${CMAKE_CURRENT_SOURCE_DIR}/addon-instance/Web.h
            ../../include/kodi/AddonBase.h
            ../../include/kodi/Processor.h
            ../../include/kodi/AudioEngine.h
            ../../include/kodi/Filesystem.h
            ../../include/kodi/General.h
            ../../include/kodi/Network.h
            ../../include/kodi/addon-instance/Web.h
            ../../include/kodi/gui/General.h
            ../../include/kodi/gui/ListItem.h
            ../../include/kodi/gui/Window.h
            ../../include/kodi/gui/controls/Button.h
            ../../include/kodi/gui/controls/Edit.h
            ../../include/kodi/gui/controls/FadeLabel.h
            ../../include/kodi/gui/controls/Image.h
            ../../include/kodi/gui/controls/Progress.h
            ../../include/kodi/gui/controls/RadioButton.h
            ../../include/kodi/gui/controls/Rendering.h
            ../../include/kodi/gui/controls/SettingsSlider.h
            ../../include/kodi/gui/controls/Slider.h
            ../../include/kodi/gui/controls/Spin.h
            ../../include/kodi/gui/controls/TextBox.h
            ../../include/kodi/gui/dialogs/ContextMenu.h
            ../../include/kodi/gui/dialogs/ExtendedProgress.h
            ../../include/kodi/gui/dialogs/FileBrowser.h
            ../../include/kodi/gui/dialogs/Keyboard.h
            ../../include/kodi/gui/dialogs/Numeric.h
            ../../include/kodi/gui/dialogs/OK.h
            ../../include/kodi/gui/dialogs/Progress.h
            ../../include/kodi/gui/dialogs/Select.h
            ../../include/kodi/gui/dialogs/TextViewer.h
            ../../include/kodi/gui/dialogs/YesNo.h
            ../../include/kodi/gui/input/Action.h)

add_library(kodi-dev-kit STATIC ${SOURCES} ${HEADERS})

target_link_libraries(kodi-dev-kit devkit_sandbox)
target_include_directories(kodi-dev-kit PRIVATE ../../include/kodi)

if (WIN32 OR WINDOWS_STORE)
  target_sources(kodi-dev-kit PRIVATE ${SOURCES_WINDOWS})
  target_sources(kodi-dev-kit PRIVATE ${HEADERS_WINDOWS})
  target_link_libraries(kodi-dev-kit)
else()
  target_sources(kodi-dev-kit PRIVATE ${SOURCES_POSIX})
  target_sources(kodi-dev-kit PRIVATE ${HEADERS_POSIX})
  target_link_libraries(kodi-dev-kit devkit_sandbox pthread rt)
endif()

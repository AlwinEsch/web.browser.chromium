set(SOURCES Launcher.cpp
            kodi/AudioEngine.cpp
            kodi/Filesystem.cpp
            kodi/General.cpp
            kodi/Network.cpp
            kodi/addon-instance/Web.cpp
            kodi/gui/General.cpp
            kodi/gui/ListItem.cpp
            kodi/gui/Window.cpp
            kodi/gui/controls/Button.cpp
            kodi/gui/controls/Edit.cpp
            kodi/gui/controls/FadeLabel.cpp
            kodi/gui/controls/Image.cpp
            kodi/gui/controls/Label.cpp
            kodi/gui/controls/Progress.cpp
            kodi/gui/controls/RadioButton.cpp
            kodi/gui/controls/Rendering.cpp
            kodi/gui/controls/SettingsSlider.cpp
            kodi/gui/controls/Slider.cpp
            kodi/gui/controls/Spin.cpp
            kodi/gui/controls/TextBox.cpp
            kodi/gui/dialogs/ContextMenu.cpp
            kodi/gui/dialogs/ExtendedProgress.cpp
            kodi/gui/dialogs/FileBrowser.cpp
            kodi/gui/dialogs/Keyboard.cpp
            kodi/gui/dialogs/Numeric.cpp
            kodi/gui/dialogs/OK.cpp
            kodi/gui/dialogs/Progress.cpp
            kodi/gui/dialogs/Select.cpp
            kodi/gui/dialogs/TextViewer.cpp
            kodi/gui/dialogs/YesNo.cpp
            kodi/gui/input/Action.cpp
            utils/Log.cpp)

set(HEADERS Launcher.h
            kodi/AudioEngine.h
            kodi/Filesystem.h
            kodi/General.h
            kodi/Network.h
            kodi/addon-instance/Web.h
            kodi/gui/General.h
            kodi/gui/ListItem.h
            kodi/gui/Window.h
            kodi/gui/controls/Button.h
            kodi/gui/controls/Edit.h
            kodi/gui/controls/FadeLabel.h
            kodi/gui/controls/Image.h
            kodi/gui/controls/Label.h
            kodi/gui/controls/Progress.h
            kodi/gui/controls/RadioButton.h
            kodi/gui/controls/Rendering.h
            kodi/gui/controls/SettingsSlider.h
            kodi/gui/controls/Slider.h
            kodi/gui/controls/Spin.h
            kodi/gui/controls/TextBox.h
            kodi/gui/dialogs/ContextMenu.h
            kodi/gui/dialogs/ExtendedProgress.h
            kodi/gui/dialogs/FileBrowser.h
            kodi/gui/dialogs/Keyboard.h
            kodi/gui/dialogs/Numeric.h
            kodi/gui/dialogs/OK.h
            kodi/gui/dialogs/Progress.h
            kodi/gui/dialogs/Select.h
            kodi/gui/dialogs/TextViewer.h
            kodi/gui/dialogs/YesNo.h
            kodi/gui/input/Action.h
            utils/Log.h
            utils/Utils.h
            utils/uuid.h)

set(SOURCES_POSIX LauncherPosix.cpp)

set(HEADERS_POSIX LauncherPosix.h)

add_library(kodisandbox-server STATIC ${SOURCES}
                                      ${HEADERS})

target_link_libraries(kodisandbox-server devkit_sandbox)

if (WIN32 OR WINDOWS_STORE)
  target_sources(kodisandbox-server PRIVATE ${SOURCES_WINDOWS})
  target_sources(kodisandbox-server PRIVATE ${HEADERS_WINDOWS})
  target_link_libraries(kodisandbox-server )
else()
  target_sources(kodisandbox-server PRIVATE ${SOURCES_POSIX})
  target_sources(kodisandbox-server PRIVATE ${HEADERS_POSIX})
  target_link_libraries(kodisandbox-server pthread rt)
endif()

set(SOURCES DialogBrowserContextMenu.cpp
            DialogCookie.cpp
            DialogDownload.cpp
            DialogFile.cpp
            DialogKeyboard.cpp
            DialogSSLInformation.cpp
            GUIManager.cpp)

set(HEADERS DialogBrowserContextMenu.h
            DialogCookie.h
            DialogDownload.h
            DialogFile.h
            DialogKeyboard.h
            DialogSSLInformation.h
            GUIManager.h)

core_add_library(app_main_gui)

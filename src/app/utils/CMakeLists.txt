set(SOURCES ExtensionUtils.cpp
            FileUtils.cpp
            Info.cpp
            SystemTranslator.cpp)

set(HEADERS ExtensionUtils.cpp
            FileUtils.h
            Info.h
            SystemTranslator.h
            XMLUtils.h)

core_add_library(app_utils)

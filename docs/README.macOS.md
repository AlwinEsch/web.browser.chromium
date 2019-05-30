![Logo](resources/banner_slim.png)

# Mac OS X build guide
This is the general Mac OS X build guide. Please read it in full before you proceed to familiarize yourself with the build procedure.

Several distribution **[specific build guides](../README.md)** are available.

--------------------------------------------------
Special CMake Options:
 - `-DTHIRD_PARTY_PATH=YOUR_WANTED_PATH`
   Feature to save Chromium elsewhere. Thus, the addon construction folder can be 
   deleted without the huge package must be downloaded again in a new construction.

--------------------------------------------------

The following instructions assume you will have built Kodi already in the `kodi-build` directory 
suggested by the README. 

**Initial Load:**
1. `git clone -b add-webbrowser-support https://github.com/AlwinEsch/kodi`
2. Build Kodi by instructions defined here: https://github.com/xbmc/xbmc/blob/master/docs/README.macOS.md
3. `git clone https://github.com/AlwinEsch/web.browser.chromium`
4. `cd web.browser.chromium && mkdir build && cd build`

**Build with xcodebuild:**

5. `/Users/Shared/xbmc-depends/x86_64-darwin18.5.0-native/bin/cmake -G Xcode -DADDONS_TO_BUILD=web.browser.chromium -DADDON_SRC_PREFIX=../.. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$HOME/Library/Application Support/Kodi/addons" -DPACKAGE_ZIP=1 -DTHIRD_PARTY_PATH=$HOME/chromium ../../kodi/cmake/addons`
6. `cmake --build . --config "Release" `

**Build with make:**

5. `/Users/Shared/xbmc-depends/x86_64-darwin18.5.0-native/bin/cmake -DADDONS_TO_BUILD=web.browser.chromium -DADDON_SRC_PREFIX=../.. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$HOME/Library/Application Support/Kodi/addons" -DPACKAGE_ZIP=1 -DTHIRD_PARTY_PATH=$HOME/chromium ../../kodi/cmake/addons`
6. `make`

**CAUTION**:<br>The addon can't be tested by start of generated `kodi.bin`! Use created `kodi.app` where can be started from GUI or from command line by `open kodi`.<br>
The background is that this addon in its elements needs the Info.plist used by Kodi.


***WARNING***:<br>The libcef.so (Chromium code and needed from addon) created in debug mode can be about 4 GB in size, so in this case the first Kodi start with this library can be extended a little bit (this is cached in the following starts and goes faster). Further is the debug build currently not stable enough and should be only used during development!

![Logo](resources/banner_slim.png)

# Linux build guide
This is the general Linux build guide. Please read it in full before you proceed to familiarize yourself with the build procedure.

Several distribution **[specific build guides](../README.md)** are available.

**Note**:<br>Do not get confused if the `Still working on` is rewritten throughout
the Chromium download. The download may take several hours depending on the system
and internet connection.

--------------------------------------------------
Special CMake Options:
 - `-DTHIRD_PARTY_PATH=YOUR_WANTED_PATH`
   Feature to save Chromium elsewhere. Thus, the addon construction folder can be 
   deleted without the huge package must be downloaded again in a new construction.

--------------------------------------------------

**Note**:<br>Unfortunately, the path defined here is mandatory, as the part stored in the **[Chromium depends](../depends/common/chromium)**
folder is not available in the normal system, so Linux can not simply use a `mkdir build && cd build && cmake ..` 
in this case!

The following instructions assume you will have built Kodi already in the `kodi-build` directory 
suggested by the README.

1. `git clone https://github.com/AlwinEsch/kodi.git`
2. `git clone https://github.com/AlwinEsch/web.browser.chromium.git`
3. `cd web.browser.chromium && mkdir build && cd build`
4. `cmake -DADDONS_TO_BUILD=web.browser.chromium -DADDON_SRC_PREFIX=../.. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$HOME/.kodi/addons -DPACKAGE_ZIP=1 -DTHIRD_PARTY_PATH=$HOME/chromium ../../kodi/cmake/addons`
5. `make`

**WARNING**:<br>The libcef.so (Chromium code and needed from addon) created in debug mode can be about 4 GB in size, so in this case the first Kodi start with this library can be extended a little bit (this is cached in the following starts and goes faster). Further is the debug build currently not stable enough and should be only used during development!


<p align="center">
  <a href="README.Linux.md" title="Linux"><img src="resources/linux.svg" height="128"></a>
</p>

# Linux build guide
This is the general Linux build guide. Please read it in full before you proceed to familiarize yourself with the build procedure.

Several distribution **[specific build guides](README.md)** are available.

The following instructions assume you will have built Kodi already in the `kodi-build` directory 
suggested by the README.

1. `git clone https://github.com/AlwinEsch/kodi.git`
2. `git clone https://github.com/AlwinEsch/web.browser.chromium.git`
3. `cd web.browser.chromium && mkdir build && cd build`
4. `cmake -DADDONS_TO_BUILD=web.browser.chromium -DADDON_SRC_PREFIX=../.. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$HOME/.kodi/addons -DPACKAGE_ZIP=1 ../../kodi/cmake/addons`
5. `make`

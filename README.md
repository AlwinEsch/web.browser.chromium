# web.browser.chromium

![Logo](web.browser.chromium/resources/icon.png)

A Chromium based web browser addon for Kodi using [CEF (Chromium Embedded Framework)](https://bitbucket.org/chromiumembedded/cef/src/master/).


**WARNING**: This addon is currently not supported direct by Kodi and still in developement!

Currently Kodi to test here: https://github.com/AlwinEsch/kodi/tree/add-webbrowser-support

### Linux

The following instructions assume you will have built Kodi already in the `kodi-build` directory 
suggested by the README.

1. `git clone https://github.com/AlwinEsch/kodi.git`
2. `git clone https://github.com/AlwinEsch/web.browser.chromium.git`
3. `cd web.browser.chromium && mkdir build && cd build`
4. `cmake -DADDONS_TO_BUILD=web.browser.chromium -DADDON_SRC_PREFIX=../.. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$HOME/.kodi/addons -DPACKAGE_ZIP=1 ../../kodi/cmake/addons`
5. `make`

**WARNING**: 
- With debug generated libcef have ~4GB!
- Chromium download and build takes a long time! Around 14GB source and longer build time.
- After build 41GB data in source folder

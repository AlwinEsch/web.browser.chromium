![Logo](resources/banner_slim.png)

# Windows build guide

This is the general Windows build guide. Please read it in full before you proceed to familiarize yourself with the build procedure.

Several distribution **[specific build guides](../README.md)** are available.

**WARNING**:<br>
If you use this addon on Windows make sure your drive `C:` have more as
40 GByte free space, otherwise define with `-DTHIRD_PARTY_PATH=YOUR_PATH`
place to somewhere else.
<br><br>
Windows is limited to a path length of 256 characters what makes it 
impossible to build over Kodi's addon build folder and define separate to
`C:\chromium` where it becomes created!
<br><br>
Also if windows registry and git commands are changes is this not fixed.
Chromium's build way use a own git where not can be edited!

**Note**:<br>Do not get confused if the `Still working on` is rewritten throughout
the Chromium download. The download may take several hours depending on the system
and internet connection.

--------------------------------------------------
Special CMake Options:
 - `-DTHIRD_PARTY_PATH=YOUR_WANTED_PATH`
   Feature to save Chromium elsewhere. Thus, the addon construction folder can be 
   deleted without the huge package must be downloaded again in a new construction.
   Default is `C:\chromium`.

--------------------------------------------------

The following instructions assume you will have built Kodi already in the `kodi-build` directory 
suggested by the README.

1. `git clone https://github.com/AlwinEsch/kodi.git`
2. `git clone https://github.com/AlwinEsch/web.browser.chromium.git`
3. `cd web.browser.chromium && mkdir build && cd build`
4. a. `cmake -G "Visual Studio 15" -T host=x64 -DADDONS_TO_BUILD=web.browser.chromium -DADDON_SRC_PREFIX=../.. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=C:/Users/<YOUR_USER_NAME>/AppData/Roaming/Kodi/addons -DPACKAGE_ZIP=1 -DTHIRD_PARTY_PATH="D:/chromium" ../../kodi/cmake/addons`
<br>or this for 64 bit build<br>
   b. `cmake -G "Visual Studio 15 Win64" -T host=x64 -DADDONS_TO_BUILD=web.browser.chromium -DADDON_SRC_PREFIX=../.. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=C:/Users/<YOUR_USER_NAME>/AppData/Roaming/Kodi/addons -DPACKAGE_ZIP=1 -DTHIRD_PARTY_PATH="D:/chromium" ../../kodi/cmake/addons`
5. `cmake --build . --config "Release"`

> `THIRD_PARTY_PATH` here only as example, leave away to use `C:` or add something else. Also must `<YOUR_USER_NAME>` changed to your user name where Kodi's data becomes handled.

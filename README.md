![Logo](docs/resources/banner_slim.png)

# Kodi's web.browser.chromium Addon Documentation Home
Welcome to Kodi's Web Browser addon documentation home. Geared at developers and it contains platform specific build guides.

> **WARNING:**<br>
This addon is currently not supported direct by Kodi and still in developement!<br>
Currently Kodi to test here: https://github.com/AlwinEsch/kodi/tree/add-webbrowser-support<br>
Also are not all below linked OS supported and still in Todo!

This is a Chromium based web browser addon for Kodi using [CEF (Chromium Embedded Framework)](https://bitbucket.org/chromiumembedded/cef/src/master/). This brings a good usable API which brings almost the complete diversity of Chromium for us.

The for Kodi used CEF is deposited here https://github.com/kodi-web/cef and contains the changes necessary, therefore this is not directly usable for other purposes.

If you haven't done so, we encourage you to read our **[contributing guide](https://github.com/xbmc/xbmc/blob/master/docs/CONTRIBUTING.md)** first. It contains pertinent information about our development model.

## Building Addon
This uses CMake as its building system but instructions are highly dependent on your operating system and target platform. Choose your platform below and read the guide carefully before proceeding.

Furthermore, this CMake accesses the Chromium and CEF based building system, which is basically based on GN and Python, which therefore has some limitations and is not directly usable via the standard dependency path of Kodi Binary addons.

**Warning**:<br>Building this addon with Chromium requires at least 16 GB of RAM (better 32 GB) and more than 15 GB will be downloaded and over 40 GB can be used on your hard drive! Also, the construction can take up to several hours.

<p align="center">
  <a href="docs/README.Android.md" title="Android"><img src="docs/resources/android.svg" height="84"></a>
  <a href="docs/README.Fedora.md" title="Fedora"><img src="docs/resources/fedora.svg" height="84"></a>
  <a href="docs/README.FreeBSD.md" title="FreeBSD"><img src="docs/resources/freebsd.svg" height="84"></a>
  <a href="docs/README.iOS.md" title="iOS"><img src="docs/resources/ios.svg" height="84"></a>
  <a href="docs/README.Linux.md" title="Linux"><img src="docs/resources/linux.svg" height="84"></a>
  <a href="docs/README.macOS.md" title="macOS"><img src="docs/resources/macos.svg" height="84"></a>
  <a href="docs/README.openSUSE.md" title="openSUSE"><img src="docs/resources/opensuse.svg" height="84"></a>
  <a href="docs/README.RaspberryPi.md" title="Raspberry Pi"><img src="docs/resources/raspberrypi.svg" height="84"></a>
  <a href="docs/README.Ubuntu.md" title="Ubuntu"><img src="docs/resources/ubuntu.svg" height="84"></a>
  <a href="docs/README.Windows.md" title="Windows"><img src="docs/resources/windows.svg" height="84"></a>
</p>


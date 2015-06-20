#!/bin/bash
###############################################################################
#                                                                             #
#     Copyright (C) 2015 Team KODI                                            #
#     http://kodi.tv                                                          #
#                                                                             #
#  This program is free software: you can redistribute it and/or modify       #
#  it under the terms of the GNU General Public License as published by       #
#  the Free Software Foundation, either version 3 of the License, or          #
#  (at your option) any later version.                                        #
#                                                                             #
#  This program is distributed in the hope that it will be useful,            #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of             #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              #
#  GNU General Public License for more details.                               #
#                                                                             #
#  You should have received a copy of the GNU General Public License          #
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.      #
#                                                                             #
###############################################################################
#                                                                             #
#  bash command options taken from original "install-build-deps.sh"           #
#                                                                             #
#  Copyright (c) 2012 The Chromium Authors. All rights reserved.              #
#  Use of this source code is governed by a BSD-style license that can be     #
#  found in the LICENSE file.                                                 #
#                                                                             #
###############################################################################

. ./tool_functions

BASE_PATH=`pwd`

usage()
{
  echo "Usage: $0 [--options]"
  echo "Options:"
  echo "--[no-]syms: enable or disable installation of debugging symbols"
  echo "--lib32: enable installation of 32-bit libraries, e.g. for V8 snapshot"
  echo "--[no-]arm: enable or disable installation of arm cross toolchain"
  echo "--[no-]chromeos-fonts: enable or disable installation of Chrome OS"\
       "fonts"
  echo "--[no-]nacl: enable or disable installation of prerequisites for"\
       "building standalone NaCl and all its toolchains"
  echo "--no-prompt: silently select standard options/defaults"
  echo "--quick-check: quickly try to determine if dependencies are installed"
  echo "               (this avoids interactive prompts and sudo commands,"
  echo "               so might not be 100% accurate)"
  echo "--unsupported: attempt installation even on unsupported systems"
  echo "Script will prompt interactively if options not given."
  exit 1
}

# Checks whether a particular package is available in the repos.
# USAGE: $ package_exists <package name>
package_exists()
{
  apt-cache pkgnames | grep -x "$1" > /dev/null 2>&1
}

# These default to on because (some) bots need them and it keeps things
# simple for the bot setup if all bots just run the script in its default
# mode.  Developers who don't want stuff they don't need installed on their
# own workstations can pass --no-arm --no-nacl when running the script.
do_inst_arm=1
do_inst_nacl=1

while test "$1" != ""
do
  case "$1" in
  --syms)                   do_inst_syms=1;;
  --no-syms)                do_inst_syms=0;;
  --lib32)                  do_inst_lib32=1;;
  --arm)                    do_inst_arm=1;;
  --no-arm)                 do_inst_arm=0;;
  --chromeos-fonts)         do_inst_chromeos_fonts=1;;
  --no-chromeos-fonts)      do_inst_chromeos_fonts=0;;
  --nacl)                   do_inst_nacl=1;;
  --no-nacl)                do_inst_nacl=0;;
  --no-prompt)              do_default=1
                            do_quietly="-qq --assume-yes"
    ;;
  --quick-check)            do_quick_check=1;;
  --unsupported)            do_unsupported=1;;
  *) usage;;
  esac
  shift
done

if file /sbin/init | grep -q 'ELF 64-bit'; then
  do_inst_lib32=0
  do_inst_arm=0
elif test "$do_inst_arm" = "1"; then
  do_inst_lib32=1
fi

# Check for lsb_release command in $PATH
if ! which lsb_release > /dev/null; then
  printf "${RED}${BRIGHT}ERROR:${WHITE} lsb_release not found in \$PATH${NORMAL}\n" >&2
  exit 1;
fi

lsb_release=$(lsb_release --codename --short)
ubuntu_codenames="(precise|trusty|utopic|vivid)"
if [ 0 -eq "${do_unsupported-0}" ] && [ 0 -eq "${do_quick_check-0}" ] ; then
  if [[ ! $lsb_release =~ $ubuntu_codenames ]]; then
    printf "${RED}${BRIGHT}ERROR:${WHITE} Only Ubuntu 12.04 (precise), 14.04 (trusty), 14.10 (utopic) and\n15.04 (vivid) are currently supported${NORMAL}\n" >&2
    exit 1
  fi
  if ! uname -m | egrep -q "i686|x86_64"; then
    printf "${RED}${BRIGHT}ERROR:${WHITE} Only x86 architectures are currently supported${NORMAL}\n" >&2
    exit
  fi
fi

if [ "x$(id -u)" != x0 ] && [ 0 -eq "${do_quick_check-0}" ]; then
  echo "Running as non-root user."
  echo "You might have to enter your password one or more times for 'sudo'."
  echo
fi

#if [ $do_inst_arm != 0 ];then
#  # arm cross toolchain packages needed to build chrome on armhf
#  arm_list="libc6-dev-armhf-cross
#            linux-libc-dev-armhf-cross
#            g++-arm-linux-gnueabihf
#            icedtea-7-plugin"
#
#  # Work around for dependency issue Ubuntu/Trusty: http://crbug.com/435056
#  if [ "x$lsb_release" = "xtrusty" ]; then
#    arm_list+=" g++-4.8-multilib-arm-linux-gnueabihf
#                gcc-4.8-multilib-arm-linux-gnueabihf"
#  fi
#fi

printf "${WHITE}${BRIGHT}Update distribution source ...${NORMAL}"
sudo apt-get update >> "${BASE_PATH}/chromium/build.log"
evaluate_retval

# Note: Currently only checked on 14.10!
printf "${WHITE}${BRIGHT}Install general related build depends ...${NORMAL}\n"
sudo apt-get -y -q install automake autopoint git-core subversion p7zip-full >> "${BASE_PATH}/chromium/build.log"
evaluate_retval

# Note: Currently only checked on 14.10!
printf "${WHITE}${BRIGHT}Install kodi's related build depends ...${NORMAL}\n"
sudo apt-get -y -q install bison build-essential ccache cmake curl \
  cvs default-jre fp-compiler gawk gdc gettext git-core gperf libasound2-dev libass-dev \
  libavcodec-dev libavfilter-dev libavformat-dev libavutil-dev libbluetooth-dev \
  libbluray-dev libbluray1 libboost-dev libboost-thread-dev libbz2-dev libcap-dev \
  libcdio-dev libcec-dev libcec2 libcrystalhd-dev libcrystalhd3 libcurl3 \
  libcurl4-gnutls-dev libcwiid-dev libcwiid1 libdbus-1-dev libenca-dev libflac-dev \
  libfontconfig1-dev libfreetype6-dev libfribidi-dev libglew-dev libiso9660-dev \
  libjasper-dev libjpeg-dev libltdl-dev liblzo2-dev libmad0-dev libmicrohttpd-dev \
  libmodplug-dev libmp3lame-dev libmpeg2-4-dev libmpeg3-dev libmysqlclient-dev \
  libnfs-dev libogg-dev libpcre3-dev libplist-dev libpng12-dev libpostproc-dev \
  libpulse-dev libsamplerate0-dev libsdl1.2-dev libsdl-gfx1.2-dev libsdl-image1.2-dev \
  libsdl-mixer1.2-dev libshairport-dev libsmbclient-dev libsqlite3-dev libssh-dev \
  libssl-dev libswscale-dev libtiff5-dev libtinyxml-dev libtool libudev-dev \
  libusb-dev libva-dev libva-egl1 libva-tpi1 libvdpau-dev libvorbisenc2 libxml2-dev \
  libxmu-dev libxrandr-dev libxrender-dev libxslt1-dev libxt-dev libyajl-dev mesa-utils \
  nasm pmount python-dev python-imaging python-sqlite swig unzip yasm zip zlib1g-dev \
  libtag1-dev librtmp-dev gnutls-dev >> "${BASE_PATH}/chromium/build.log"
evaluate_retval

# Note: Currently only checked on 14.10!
printf "${WHITE}${BRIGHT}Install chromium CEF related build depends ...${NORMAL}\n"
sudo apt-get -y -q install libgtk2.0-dev libgtkglext1-dev libxtst-dev libxss-dev libpci-dev \
  libgconf2-dev libgnome-keyring-dev libnspr4-dev libnss3-dev libcups2-dev libexif-dev \
  libtinfo-dev libkrb5-dev $arm_list >> "${BASE_PATH}/chromium/build.log"
evaluate_retval

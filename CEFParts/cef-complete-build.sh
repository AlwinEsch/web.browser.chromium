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

COLUMNS=80

. ./tool_functions

DEPOT_TOOLS_URL="https://bitbucket.org/kodi-web/chromium_depot_tools.git"
CEF_SOURCE_URL="https://bitbucket.org/kodi-web/cef.git"
CHROMIUM_BRANCH="2357"
KODI_BINARY_VERSION="0.0.3"

START_TIME=`date`
START_TIME_SEC=`date +%s`

DIST=`grep DISTRIB_ID /etc/*-release | awk -F '=' '{print $2}'`
BASE_PATH=`pwd`
CONTINUES_INSTALL_ALLOWED=false

##
# Default values
#
DISTNAME=utopic
DO_INST_SYMS=0
DO_CREATE_X86=1
DO_CREATE_X64=1
DO_CREATE_ARM=0
DO_BUILD_CEF=1
DO_UPDATE=1
DO_CLEAN=0
CHROMIUM_CHECKOUT_CHANGED=0
CEF_CHECKOUT_CHANGED=0
FORCE_CEF_DISTRIB=1
DO_BUILD_DISTRIB=1
DEBUG_BUILD=0

usage()
{
  echo "Usage: $0 [--options]"
  echo "Options:"
  echo "--branch      chromium branch to use, default is \"$CHROMIUM_BRANCH\""
  echo "--distname    select internal used distribution, possible are only Ubuntu"
  echo "              12.04 (precise), 14.04 (trusty) and 14.10 (utopic),"
  echo "              default is \"${DISTNAME}\""
  echo "--noupdate    do not update Chromium or CEF"
  echo "--force-clean force a clean checkout of Chromium and CEF. This will"
  echo "              trigger a new update, build and distribution."
  echo "--[no-]syms   enable or disable installation of debugging symbols"
  echo "--[no-]x86    enable or disable installation of x86 32bit toolchain"
  echo "--[no-]x64    enable or disable installation of x64 64bit toolchain"
  echo "--[no-]arm    enable or disable installation of arm cross toolchain"
  echo "--[no-]debug  enable or disable installation of debug CEF parts"
  echo "--no-cef      do not build CEF parts, used to have system for chroot only"
  echo "--no-distrib  do not creates CEF distribution parts"
  echo "--help        this help message"
  echo "Script will prompt interactively for other values."
  exit 1
}

process_opts()
{
  while test "$1" != "" ; do
    case "$1" in
      --distname)
        case "$2" in
            "")
              echo "Option distname, no argument"
              shift
              ;;
            *)
              DISTNAME="$2"
              shift
              ;;
        esac ;;
      --branch)
        case "$2" in
            "")
              echo "Option branch, no argument"
              shift
              ;;
            *)
              EXIST=`git ls-remote $CEF_SOURCE_URL $2 | wc -l`
              if [ EXIST -eq 1 ];then
                CHROMIUM_BRANCH="$2"
              else
                printf "${RED}${BRIGHT}ERROR: ${WHITE}Selected CEF branch \"%i\" does not exists${NORMAL}\n" "$2" >&2
                exit 1
              fi
              shift
              ;;
        esac ;;
      --noupdate)               DO_UPDATE=0;;
      --force-clean)            DO_CLEAN=1;;
      --syms)                   DO_INST_SYMS=1;;
      --no-syms)                DO_INST_SYMS=0;;
      --x86)                    DO_CREATE_X86=1;;
      --no-x86)                 DO_CREATE_X86=0;;
      --x64)                    DO_CREATE_X64=1;;
      --no-x64)                 DO_CREATE_X64=0;;
      --arm)                    DO_CREATE_ARM=1;;
      --no-arm)                 DO_CREATE_ARM=0;;
      --no-cef)                 DO_BUILD_CEF=0;;
      --no-distrib)             DO_BUILD_DISTRIB=0;;
      --debug)                  DEBUG_BUILD=1;;
      --no-debug)               DEBUG_BUILD=0;;
      --help)                   usage;;
      *)
        echo "invalid command-line option: $OPTARG"
        usage
        ;;
    esac
    shift
  done
}

print_package_missing_warning()
{
  printf "${WHITE}${BRIGHT}Missing package ${CYAN}%s${NORMAL}" $1
  if [ $CONTINUES_INSTALL_ALLOWED = false ]; then
    printf "${WHITE}, press enter or space to allow install of all missing parts${NORMAL}\n"
  fi
}

check_dev_package()
{
  which gcc > /dev/null
  if [ $? -eq 1 ]; then
    printf "${WHITE}${BRIGHT}Missing build system, complete kodi dependencies becomes\ninstalled, press enter or space to install${NORMAL}\n"
    read -N1 key
    if [[ $key = "" ]]; then
      sudo apt-get build-dep xbmc
    else
      printf "${RED}${BRIGHT}Build stopped due to missing package ${NORMAL}'%s'\n" kodi-dev
      exit 1
    fi
  fi
}

check_package()
{
  dpkg -s $1 > /dev/null
  if [ $? -eq 1 ]; then
    print_package_missing_warning $1
    if [ $CONTINUES_INSTALL_ALLOWED == true ]; then
      sudo apt-get install -qy $1
    else
      read -N1 key
      CONTINUES_INSTALL_ALLOWED=true
      if [[ $key = "" ]]; then
        sudo apt-get install -qy $1
      else
        printf "${RED}${BRIGHT}Build stopped due to missing package ${NORMAL}'%s'\n" $1
        exit 1
      fi
    fi
  fi
}

check_packages()
{
  check_dev_package
  check_package git
  check_package subversion
  check_package clang
}

##
# Create chroot build environment
#
create_chroot_build_environment()
{
  printf "${BRIGHT}${GREEN}Start creation of chroot system for %s on %s${NORMAL} ..." $2 ${DISTNAME}
  if [ ! -f "${BASE_PATH}/chromium/buildFlag0003_chroot_build_env_done_${DISTNAME}_${1}bit" ]; then
    printf "\n  ${WHITE}(follow related requests of them)${NORMAL}\n$"
    bash "${BASE_PATH}/install-chroot.sh" -t $1 -d $DISTNAME -u
    evaluate_retval

    touch "${BASE_PATH}/chromium/buildFlag0003_chroot_build_env_done_${DISTNAME}_${1}bit"
  else
    log_done_msg
  fi
}

##
# Downlad and install build dependencies
#
build_dependencies()
{
  if [ $1 = "arm" ];then
    return
  fi
  printf "${GREEN}${BRIGHT}Downlad and install build dependencies for ${2} on ${DISTNAME} ${NORMAL} ..."
  if [ ! -f "${BASE_PATH}/chromium/buildFlag0004_chroot_build_deps_done_${DISTNAME}_${2}" ]; then
    printf "\n"
    cd "$BASE_PATH"
    if [ DO_INST_SYMS != 0 ];then
      symbols=" --syms"
    else
      symbols= "--no-syms"
    fi
    sudo ${DISTNAME}${1} bash -c "\"$BASE_PATH/install-build-deps-kodi.sh\" ${symbols};
          rc=$?;
          /etc/init.d/cron stop >/dev/null 2>&1 || :;
          /etc/init.d/rsyslog stop >/dev/null 2>&1 || :;
          /etc/init.d/dbus stop >/dev/null 2>&1 || :;
          exit $rc"
#    evaluate_reterr

#    sudo /usr/local/bin/${DISTNAME}${1} bash -c "$BASE_PATH/install-build-deps-android.sh"
#    evaluate_reterr

#    printf "${WHITE}${BRIGHT}Set build files link definations${NORMAL}"
#    GCC_VERSION=`${DISTNAME}${1} /usr/bin/gcc-* --version | grep g++ | awk -F '-' '{print $2}' | awk -F ' ' '{print $1}'`
#    evaluate_reterr
#    sudo ${DISTNAME}${1} ln -sf g++-$GCC_VERSION /usr/bin/c++ -T
#    evaluate_reterr
#    sudo ${DISTNAME}${1} ln -sf g++-$GCC_VERSION /usr/bin/g++ -T
#    evaluate_reterr
#    sudo ${DISTNAME}${1} ln -sf gcc-$GCC_VERSION /usr/bin/gcc -T
#    evaluate_reterr
#    sudo ${DISTNAME}${1} ln -sf gcc-ar-$GCC_VERSION /usr/bin/gcc-ar -T
#    evaluate_reterr
#    sudo ${DISTNAME}${1} ln -sf gcc-nm-$GCC_VERSION /usr/bin/gcc-nm -T
#    evaluate_reterr
#    sudo ${DISTNAME}${1} ln -sf gcc-ranlib-$GCC_VERSION /usr/bin/gcc-ranlib -T
#    evaluate_reterr
#    sudo ${DISTNAME}${1} ln -sf gcov-$GCC_VERSION /usr/bin/gcov -T
    evaluate_retval

    touch "${BASE_PATH}/chromium/buildFlag0004_chroot_build_deps_done_${DISTNAME}_${2}"
  else
    log_done_msg
  fi
}

##
# Run the cef_create_projects script (.bat on Windows, .sh on OS X and Linux)
# to generate the build files based on the GYP configuration.
#
create_projects()
{
  local target=$1
  local system=$2

  if [ -d "${BASE_PATH}/chromium/.reverse_backup/${system}/out" ];then
    printf "\n${WHITE}${BRIGHT}Moving in previous builded parts${NORMAL}\n"
    if [ -d "${BASE_PATH}/chromium/src/out" ];then
      rm -Rf "${BASE_PATH}/chromium/src/out"
      evaluate_reterr
    fi
    mv "${BASE_PATH}/chromium/.reverse_backup/${system}/out" "${BASE_PATH}/chromium/src"
    evaluate_retval
  fi

  if [ $target == "64" ];then
    printf "${WHITE}${BRIGHT}Moving in previous removed 64 bit parts${NORMAL}"
    if [ ! -d "${BASE_PATH}/chromium/src/third_party/binutils/Linux_x64" ];then
      mv "${BASE_PATH}/chromium/.reverse_backup/third_party/binutils/Linux_x64" "${BASE_PATH}/chromium/src/third_party/binutils"
    fi
    evaluate_retval
  fi

  VERBOSE=""
  printf "${WHITE}${BRIGHT}Run the cef_create_projects script to generate needed ${system} CEF build files${NORMAL}\n"
  sh -c cat > "$BASE_PATH/chromium/cef_build_${DISTNAME}_${target}.sh" << EOF
#!/bin/bash
##
# Automatic generated creation script for kodi's web add-on related CEF parts.
# Usable for reduced time during development on chromium and CEF
#

VERBOSE=$VERBOSE
TARGET=$target
TARGET_NAME=$system
USE_DEBUG=$3
USE_RELEASE=1
BASE_PATH=$BASE_PATH
REV_BACKUP_PATH=\${BASE_PATH}/chromium/.reverse_backup

. \${BASE_PATH}/tool_functions

export PATH="\${BASE_PATH}/chromium/depot_tools:\${PATH}"
export GYP_GENERATORS=ninja

printf "\${WHITE}\${BRIGHT}Starting build of CEF system for %s ...\${NORMAL}\n" \$TARGET_NAME

#gclient runhooks

# Backup 64 bit parts, which becomes removed on source for 32 bit build
mkdir -p \${REV_BACKUP_PATH}/third_party/binutils
if [ -d "\${BASE_PATH}/chromium/src/third_party/binutils/Linux_x64" ];then
  cp -Rf "\${BASE_PATH}/chromium/src/third_party/binutils/Linux_x64" "\${REV_BACKUP_PATH}/third_party/binutils"
fi

if [ \$TARGET = "32" ];then
  export GYP_DEFINES="clang=0 target_arch=ia32"
elif [ \$TARGET = "64" ];then
  export GYP_DEFINES="target_arch=x64"
elif [ \$TARGET = "arm" ];then
  export GYP_DEFINES="OS=linux target_arch=arm arm_version=6 arm_neon=0 arm_float_abi=hard clang=0 enable_tracing=1"
else
  printf "Not supported target %s selected" \$TARGET
  log_failure_msg
  exit 1
fi

cd \${BASE_PATH}/chromium/src/cef
./cef_create_projects.sh
evaluate_retval
cd \${BASE_PATH}/chromium

printf "\${WHITE}\${BRIGHT}Update chromium clang scripts ...\${NORMAL}\n"
python \${BASE_PATH}/chromium/src/tools/clang/scripts/update.py --if-needed
evaluate_retval

printf "\${WHITE}\${BRIGHT}Set last change flags ...\${NORMAL}"
python \${BASE_PATH}/chromium/src/build/util/lastchange.py -o src/build/util/LASTCHANGE
evaluate_reterr
python \${BASE_PATH}/chromium/src/build/util/lastchange.py -s src/third_party/WebKit -o src/build/util/LASTCHANGE.blink
evaluate_retval

printf "\${WHITE}\${BRIGHT}Download third party binutils ...\${NORMAL}\n"
python \${BASE_PATH}/chromium/src/third_party/binutils/download.py
evaluate_retval

if [ \$TARGET = "32" -o  \$TARGET = "arm" ];then
  printf "\${WHITE}\${BRIGHT}Removing 64 bit parts from binutils ...\${NORMAL}"
  if [ -d \${BASE_PATH}/chromium/src/third_party/binutils/Linux_x64 ];then
    rm -Rf \${BASE_PATH}/chromium/src/third_party/binutils/Linux_x64
    evaluate_retval
  else
    printf " \${CYAN}(Ignore 'WARNING' before)\${NORMAL}"
    log_done_msg
  fi
elif [ ! -d \${BASE_PATH}/chromium/src/third_party/binutils/Linux_x64 -a \$TARGET = "64" ];then
  printf "\${WHITE}\${BRIGHT}Restore 64 bit parts from binutils ...\${NORMAL}"
  cp -Rf \${REV_BACKUP_PATH}/third_party/binutils \${BASE_PATH}/chromium/src/third_party/binutils/Linux_x64
  evaluate_retval
fi

printf "\${WHITE}\${BRIGHT}Start debug build of chromium CEF (%s) ...\${NORMAL}" \$TARGET_NAME
if [ \$USE_DEBUG != "0" ];then
  printf "\n"
  ninja \$VERBOSE -C \${BASE_PATH}/chromium/src/out/Debug cefclient cef_unittests chrome_sandbox -j 8
  evaluate_retval
else
  log_skip_msg
fi

printf "\${WHITE}\${BRIGHT}Start release build of chromium CEF (%s) ...\${NORMAL}\n" \$TARGET_NAME
ninja \$VERBOSE -C \${BASE_PATH}/chromium/src/out/Release cefclient cef_unittests chrome_sandbox -j 8
evaluate_retval

EOF
  evaluate_reterr

  # Make created script executable
  chmod 755 "$BASE_PATH/chromium/cef_build_${DISTNAME}_${1}.sh"
  evaluate_reterr

  # Call build script
  if [ $target == "arm" ];then
    target="32"
  fi
  ${DISTNAME}${target} "$BASE_PATH/chromium/cef_build_${DISTNAME}_${1}.sh"
  evaluate_reterr
}

##
#
delete_directory()
{
  # Removes an existing directory.
  local path="${1}"
  if [ -d "${path}" ];then
    printf "Removing directory %s" "${path}"
    rm -Rf "${path}"
    evaluate_retval
  fi
}

##
# Function to create binary distribution packages which include needed kodi's
# add-on parts.
#
create_build_distrib()
{
  local chrootName=$1
  local system=$2
  local systemShort=$3

  printf "${WHITE}${BRIGHT}Start creation of kodi's build distribution for %s ...${NORMAL}" $system

  mkdir -p "${BASE_PATH}/binary_distrib"

  if [ $CHROMIUM_CHECKOUT_CHANGED != 0 -o $CEF_CHECKOUT_CHANGED != 0 -o $FORCE_CEF_DISTRIB != 0 ];then
    # Values for make_distrib.py:
    export CEF_COMMAND_7ZIP="7z"
    #
    # --output-dir    - output directory [required]
    # --allow-partial - allow creation of partial distributions
    # --no-symbols    - don't create symbol files
    # --no-docs       - don't create documentation
    # --no-archive    - don't create archives for output directories
    # --ninja-build   - build was created using ninja
    # --x64-build     - build was created for 64-bit systems
    # --minimal       - include only release build binary files
    # --client        - include only the cefclient application
    # -q, --quiet     - do not output detailed status information

    # Create the requested distribution types.
    local options="--ninja-build --quiet --allow-partial "

    # Create the distribution.
    $chrootName python "${BASE_PATH}/chromium/src/cef/tools/make_distrib.py" \
      --output-dir "${BASE_PATH}/binary_distrib/" ${options} >> "${BASE_PATH}/chromium/build.log" 2>&1
    evaluate_reterr

    cd "${BASE_PATH}/binary_distrib"
    local name=`ls *_linux${systemShort}.7z`
    mv "${name}" "kodi-web-${KODI_BINARY_VERSION}_${name}"
    evaluate_reterr
    cd "${BASE_PATH}"

    printf "${WHITE}${BRIGHT}Moving parts to backup folder (prevent delete on other build type) ...${NORMAL}" $system
    mkdir -p "${BASE_PATH}/chromium/.reverse_backup/$system"
    mv "${BASE_PATH}/chromium/src/out" "${BASE_PATH}/chromium/.reverse_backup/$system"
    evaluate_retval
  else
    log_skip_msg
  fi
}

##
# Start of process points
#
printf "${WHITE}${BRIGHT}*-----------------------------------------------------------------------------*${NORMAL}\n"
printf "${WHITE}${BRIGHT}|                                                                             |${NORMAL}\n"
printf "${WHITE}${BRIGHT}|                      CEF complete build, Version %s                      |${NORMAL}\n" $KODI_BINARY_VERSION
printf "${WHITE}${BRIGHT}|                                                                             |${NORMAL}\n"
printf "${WHITE}${BRIGHT}|-----------------------------------------------------------------------------|${NORMAL}\n"
printf "${WHITE}${BRIGHT}|                                                                             |${NORMAL}\n"
printf "${WHITE}${BRIGHT}|${NORMAL}     Copyright (C) 2015 Team KODI                                            ${WHITE}${BRIGHT}|${NORMAL}\n"
printf "${WHITE}${BRIGHT}|${NORMAL}     http://kodi.tv                                                          ${WHITE}${BRIGHT}|${NORMAL}\n"
printf "${WHITE}${BRIGHT}|                                                                             |${NORMAL}\n"
printf "${WHITE}${BRIGHT}|${NORMAL}     GNU General Public License, Version 3                                   ${WHITE}${BRIGHT}|${NORMAL}\n"
printf "${WHITE}${BRIGHT}|                                                                             |${NORMAL}\n"
printf "${WHITE}${BRIGHT}|-----------------------------------------------------------------------------|${NORMAL}\n"
printf "\n"

process_opts "$@"

ubuntu_codenames="(precise|trusty|utopic|vivid)"
if [ 0 -eq "${do_unsupported-0}" ] && [ 0 -eq "${do_quick_check-0}" ] ; then
  if [[ ! $DISTNAME =~ $ubuntu_codenames ]]; then
    printf "${RED}${BRIGHT}ERROR: ${WHITE}Only Ubuntu 12.04 (precise), 14.04 (trusty), 14.10 (utopic) and${NORMAL}\n" >&2
    printf "${WHITE}${BRIGHT}       15.04 (vivid) are currently supported${NORMAL}\n" >&2
    exit 1
  fi

  if ! uname -m | egrep -q "i686|x86_64"; then
    echo "Only x86 architectures are currently supported" >&2
    exit
  fi
fi

printf "${WHITE}${BRIGHT}This script will help you through the process of installing a Debian or Ubuntu${NORMAL}\n"
printf "${WHITE}${BRIGHT}distribution in a chroot environment. You will have to provide your \"sudo\"${NORMAL}\n"
printf "${WHITE}${BRIGHT}password when requested.${NORMAL}\n"
printf "\n"

if [ "$DIST" != "Ubuntu" -a "$DIST" != "Debian" ]; then
  printf "${RED}${BRIGHT}Build not possible, need performed on ubuntu or debian based system (needed${NORMAL}\n" >&2
  printf "${RED}${BRIGHT}from chromium parts)!${NORMAL}\n" >&2
  exit 1
fi

if [ ! -d "$BASE_PATH/chromium" ];then
  mkdir -p "$BASE_PATH/chromium"
fi

if [ -f "$BASE_PATH/chromium/info_startDiskSize" ];then
  FREE_DISK_SPACE=`cat "$BASE_PATH/chromium/info_startDiskSize"`
else
  FREE_DISK_SPACE=`df  . | tail -1 | tr -s ' ' | cut -d' ' -f4`
  echo $FREE_DISK_SPACE > "$BASE_PATH/chromium/info_startDiskSize"
fi

# 25 GByte
if [ $FREE_DISK_SPACE -gt 32212254720 ]; then
  printf "${RED}${BRIGHT}Not enough free space available!${NORMAL}\n" >&2
  exit 1
fi

printf "${WHITE}${BRIGHT}Build info:${NORMAL}\n"
printf "${WHITE} - Depot tools :\t${NORMAL}%s\n" $DEPOT_TOOLS_URL
printf "${WHITE} - CEF branch :\t\t%s${NORMAL}\n" $CHROMIUM_BRANCH
printf "${WHITE} - CEF URL :\t\t%s${NORMAL}\n" $CEF_SOURCE_URL
printf "${WHITE} - CEF source dir. :\t%s${NORMAL}\n" "$BASE_PATH/chromium/cef"
printf "${WHITE} - Force clean :\t"
if [ $DO_CLEAN != 0 ]; then
  printf "Yes${NORMAL}\n"
else
  printf "No${NORMAL}\n"
fi

printf "\n${WHITE}${BRIGHT}System info:${NORMAL}\n"
printf "${WHITE} - Build start time :\t$START_TIME${NORMAL}\n"
printf "${WHITE} - Linux version :\t$(uname -r)${NORMAL}\n"
printf "${WHITE} - Used distribution :\t%s${NORMAL}\n" "$DISTNAME"
memory=`grep MemTotal /proc/meminfo | awk '{print $2}'`
memory=`expr $memory / 1024`
printf "${WHITE} - Memory size :\t%s MByte${NORMAL}\n" "$memory"
printf "${WHITE} - Free Disk size :\t%s GByte\n${NORMAL}\n" `expr $FREE_DISK_SPACE / 1024 / 1024`
if [ "x$(id -u)" != x0 ] && [ 0 -eq "${do_quick_check-0}" ]; then
  printf "${WHITE}${BRIGHT}Running as non-root user.${NORMAL}\n"
  printf "${WHITE}${YELLOW}You might have to enter your password one or more times for 'sudo'.${NORMAL}\n\n"
fi

if [ $DEBUG_BUILD != 0 -a $memory -lt 8182 ];then
  printf "${WHITE}${RED}Memory size to small for debug build of CEF!${NORMAL}\n\n"
  exit
fi
if [ $memory -lt 4096 ];then
  printf "${WHITE}${RED}Memory size to small for build of CEF!${NORMAL}\n\n"
  exit
fi

printf "${WHITE}${BRIGHT}|-----------------------------------------------------------------------------|${NORMAL}\n"

##
# Initial log file
#
if [ ! -f $BASE_PATH/chromium/build.log ];then
  sh -c cat > "$BASE_PATH/chromium/build.log" << EOF
#
# Cromium build log messages
#
# Becomes updated on every cef-complete-build.sh call and contains always also
# messages from before
#

EOF
else
  sh -c cat >> "$BASE_PATH/chromium/build.log" << EOF

###############################################################################
# Cromium build log messages
# New build started at ${START_TIME}
#

EOF
fi

##
# Process bar handle bash script to prevent chromium load from very big console
# message list
#
sh -c cat > "$BASE_PATH/chromium/.tempLogger" << EOF
#!/bin/bash

# Process bar handler

GREEN=\$(tput setaf 2)
YELLOW=\$(tput setaf 3)
BLUE=\$(tput setaf 4)
MAGENTA=\$(tput setaf 5)
CYAN=\$(tput setaf 6)
WHITE=\$(tput setaf 7)
BRIGHT=\$(tput bold)
NORMAL=\$(tput sgr0)

printf "In progress, for details see $BASE_PATH/chromium/build.log\n"

POSITION=0

while [ -f .tempLoggerActive ]; do
  printf "\$WHITE[\$NORMAL"

  PROGRESS=""
  COUNTER=0
  while [ \$COUNTER -lt 52 ]; do
    let COUNTER=COUNTER+1
    if [ \$POSITION == \$COUNTER ]; then
      PROGRESS=\${PROGRESS}\$BRIGHT\$GREEN"|"\$NORMAL
    else
      PROGRESS=\${PROGRESS}"."
    fi
  done
  printf \$PROGRESS
  let POSITION=POSITION+1
  if [ \$POSITION -ge 52 ]; then
    POSITION=0
  fi

  printf "\$WHITE] %s\$NORMAL\r" "\`tail -1 "$BASE_PATH/chromium/build.log" | cut -d' ' -f1 | cut -c-17\`"
  sleep 1
done
printf "\n"
EOF

##
# Check base packages to start creation of chroot system and kodi
#
check_packages

##
# Download required chromium depot tools
#
printf "\n${WHITE}${BRIGHT}Download chromium depot tools${NORMAL}"
if [ ! -d "${BASE_PATH}/chromium/depot_tools" ]; then
  printf " ...\n"
  git -C "$BASE_PATH/chromium" clone $DEPOT_TOOLS_URL depot_tools
  evaluate_retval
  CEF_CHECKOUT_NEW=1
else
  CEF_CHECKOUT_NEW=0
  log_done_msg
fi

export PATH="${PATH}:${BASE_PATH}/chromium/depot_tools"

##
# Update depot tools
#
if [ $CEF_CHECKOUT_NEW = 0 -a $DO_UPDATE != 0 ];then
  printf "${WHITE}${BRIGHT}Update chromium depot tools ... ${NORMAL}"
  cd ${BASE_PATH}/chromium/depot_tools
  update_depot_tools >>"$BASE_PATH/chromium/build.log" 2>&1
  evaluate_retval
fi

##
# Download CEF source code from the CEF Git repository to a "cef" directory
# inside the Chromium "src" directory. If Chromium code was downloaded to
# "/path/to/chromium/src" then CEF code should be present later on
# "/path/to/chromium/src/cef". Note that the directory must be named "cef".
#
if [ $DO_CLEAN != 0 ];then
  rm -fr "$BASE_PATH/chromium/cef"
fi

if [ $DO_BUILD_CEF != 0 ];then
  if [ ! -d "$BASE_PATH/chromium/cef" ]; then
    printf "${WHITE}${BRIGHT}Download CEF source code${NORMAL} ...\n"
    git -C "$BASE_PATH/chromium" clone $CEF_SOURCE_URL --branch $CHROMIUM_BRANCH cef
    evaluate_retval
  elif [ $DO_UPDATE != 0 ];then
    printf "${WHITE}${BRIGHT}CEF check newest revision${NORMAL} ..."
    echo "CEF check newest revision:" >> "$BASE_PATH/chromium/build.log"
    git -C "$BASE_PATH/chromium/cef" pull >>"$BASE_PATH/chromium/build.log" 2>&1
    evaluate_retval
    echo "" >> "$BASE_PATH/chromium/build.log"
  fi
else
  log_skip_msg
fi

##
# Create gclient configuration file.
#
if [ ! -f "${BASE_PATH}/chromium/.gclient" -o $DO_CLEAN != 0 ];then
  sh -c cat > "$BASE_PATH/chromium/.gclient" << EOF
solutions = [
  {
    "managed": False,
    "name": "src",
    "url": "https://chromium.googlesource.com/chromium/src.git",
    "custom_deps": {
      "build": None,
      "build/scripts/command_wrapper/bin": None,
      "build/scripts/gsd_generate_index": None,
      "build/scripts/private/data/reliability": None,
      "build/scripts/tools/deps2git": None,
      "build/third_party/lighttpd": None,
      "commit-queue": None,
      "depot_tools": None,
      "src/chrome_frame/tools/test/reference_build/chrome": None,
      "src/chrome/tools/test/reference_build/chrome_linux": None,
      "src/chrome/tools/test/reference_build/chrome_mac": None,
      "src/chrome/tools/test/reference_build/chrome_win": None,
    },
    "deps_file": ".DEPS.git",
    "safesync_url": ""
  }
]
EOF
fi
#      "build/third_party/webrtc": None,

##
# Initial Chromium checkout.
#
printf "${WHITE}${BRIGHT}Download chromium source code${NORMAL} ..."
if [ ! -d "${BASE_PATH}/chromium/src" -a $DO_UPDATE != 0 ];then
  printf "\n${RED}${BRIGHT}WARNING: ${WHITE}Download takes a very long time ${UNDERLINE}(more as 12 hours possible)${NORMAL}\n"
  cd "$BASE_PATH/chromium"
  CHROMIUM_CHECKOUT_NEW=1
  touch "$BASE_PATH/chromium/.tempLoggerActive"
  bash "$BASE_PATH/chromium/.tempLogger" &
  tailPID=$!
  echo "Initial Chromium checkout:" >> "$BASE_PATH/chromium/build.log"
  echo "started" >> "$BASE_PATH/chromium/build.log"
  gclient sync --nohooks --with_branch_heads --jobs 16 >> "$BASE_PATH/chromium/build.log" 2>&1
  error_value="${?}"
  rm "$BASE_PATH/chromium/.tempLoggerActive"
  if [ ${error_value} = 0 ]; then
    log_success_msg
    sleep 1
  fi
  if [[ ! -z `ps --no-headers -p $tailPID` ]]; then
    kill $tailPID
  fi
  if [ ${error_value} != 0 ]; then
    log_failure_msg
    exit ${error_value}
  fi
  echo "" >> "$BASE_PATH/chromium/build.log"
else
  CHROMIUM_CHECKOUT_NEW=0
  log_done_msg
fi

##
# Determine if the CEF checkout needs to change.
#
if [ -d "${BASE_PATH}/chromium/src/cef" -a $DO_UPDATE != 0 ]; then
  CEF_CURRENT_HASH=`git -C "${BASE_PATH}/chromium/src/cef" rev-parse "HEAD"`
  CEF_DESIRED_HASH=`git -C "${BASE_PATH}/chromium/cef" rev-parse "HEAD"`
  if [ $CEF_CURRENT_HASH != $CEF_DESIRED_HASH ]; then
    CEF_CHECKOUT_CHANGED=1
    printf "${WHITE}${BRIGHT}CEF Current Checkout:${NORMAL} %s\n" $CEF_CURRENT_HASH
    printf "${WHITE}${BRIGHT}CEF Desired Checkout:${NORMAL} %s\n" $CEF_DESIRED_HASH
  else
    CEF_CHECKOUT_CHANGED=0
  fi
fi

##
# Determine if the Chromium checkout needs to change.
#
CHROMIUM_CHECKOUT=`cat "$BASE_PATH/chromium/cef/CHROMIUM_BUILD_COMPATIBILITY.txt" | tail -2 | cut -d' ' -f4 | cut -d "'" -f2 | head -1`
if [ -d "${BASE_PATH}/chromium/src" -a $DO_UPDATE != 0 ]; then
  CHROMIUM_CURRENT_HASH=`git -C "${BASE_PATH}/chromium/src" rev-parse "HEAD"`
  CHROMIUM_DESIRED_HASH=`git -C "${BASE_PATH}/chromium/src" rev-parse $CHROMIUM_CHECKOUT`
  if [ $CHROMIUM_CHECKOUT_NEW != 0 -o $CHROMIUM_CURRENT_HASH != $CHROMIUM_DESIRED_HASH ]; then
    CHROMIUM_CHECKOUT_CHANGED=1
  else
    CHROMIUM_CHECKOUT_CHANGED=0
  fi
  printf "${WHITE}${BRIGHT}Chromium Current Checkout:${NORMAL} %s\n" $CHROMIUM_CURRENT_HASH
  printf "${WHITE}${BRIGHT}Chromium Desired Checkout:${NORMAL} %s\n                           (%s)\n" $CHROMIUM_DESIRED_HASH $CHROMIUM_CHECKOUT
fi

if [ $CHROMIUM_CHECKOUT_CHANGED != 0 -o $CEF_CHECKOUT_CHANGED != 0 ];then
  rm -fr "$BASE_PATH/chromium/src/cef"
fi

if [ $DO_CLEAN != 0 ];then
  rm -fr "$BASE_PATH/chromium/src/out"
fi

##
# Update the Chromium checkout.
#
cd "$BASE_PATH/chromium/src"
printf "${WHITE}${BRIGHT}Update the Chromium checkout to needed source${NORMAL} ..."
if [ $CHROMIUM_CHECKOUT_CHANGED != 0 ];then
  printf "\n"
  if [ $CHROMIUM_CHECKOUT_NEW != 0 ];then
    if [ $DO_CLEAN != 0 ];then
      # Remove all local changes including third-party git checkouts managed by
      # gclient.
      git clean -dffx
    else
      # Revert all changes in the Chromium checkout.
      gclient revert --nohooks
    fi
    evaluate_reterr
  fi
  # Fetch new sources.
  git fetch
  evaluate_reterr
  # Also fetch tags, which are required for release branch builds.
  git fetch --tags
  evaluate_reterr

  # Checkout the requested branch.
  if [ $DO_CLEAN = 0 ];then
    git checkout $CHROMIUM_CHECKOUT
    if [ ${?} != 0 ];then
      DO_CLEAN=1
    fi
  fi
  if [ $DO_CLEAN != 0 ];then
    git checkout --force $CHROMIUM_CHECKOUT
  fi
  evaluate_reterr

  # Patch the Chromium DEPS file if necessary.
  if [[ -z `patch --dry-run -Np0 -i "$BASE_PATH/chromium/cef/patch/patches/.DEPS.git.patch"` ]];then
    patch -Np0 -i "$BASE_PATH/chromium/cef/patch/patches/.DEPS.git.patch"
  fi
  # Set the GYP_CHROMIUM_NO_ACTION value temporarily so that `gclient sync` does
  # not run gyp.
  set GYP_CHROMIUM_NO_ACTION 1

  # Update third-party dependencies including branch/tag information.
  if [ DO_CLEAN != 0 ];then
    gclient sync --reset --nohooks --with_branch_heads --jobs 16
  else
    gclient sync --nohooks --with_branch_heads --jobs 16
  fi

  # Clear the GYP_CHROMIUM_NO_ACTION value.
  unset GYP_CHROMIUM_NO_ACTION

  # Delete the src/out directory created by `gclient sync`.
  rm -fr "$BASE_PATH/chromium/src/out"
  log_success_msg
else
  log_skip_msg
fi

# Copy CEF to chromium source folder
printf "${WHITE}${BRIGHT}Transfering CEF source code to chromium${NORMAL}"
if [ ! -d "$BASE_PATH/chromium/src/cef" ]; then
  cp -fR "$BASE_PATH/chromium/cef" "$BASE_PATH/chromium/src"
  evaluate_retval
else
  log_skip_msg
fi

##
# Create chroot build environment
#
if [ $DO_CREATE_X86 != 0 ];then
  create_chroot_build_environment 32 "x86-32bit"
fi
if [ $DO_CREATE_X64 != 0 ];then
  create_chroot_build_environment 64 "x64-64bit"
fi

##
# CEF build handling
#
# function "build_dependencies" installs the needed dependencies to compile
# chromium and CEF
# function "create_projects" creates separate script to build related parts,
# usable as independent script for development and tests
# function "create_build_distrib" creates the final packages which becomes
# used from kodi's web add-on
#
if [ $DO_BUILD_CEF != 0 ];then
  if [ $DO_CREATE_X86 != 0 ];then
    build_dependencies 32 "x86-32bit"
    create_projects 32 "x86-32bit" $DEBUG_BUILD
    if [ DO_BUILD_DISTRIB != 0 ];then
      create_build_distrib ${DISTNAME}32 "x86-32bit" "32"
    fi
  fi
  if [ $DO_CREATE_X64 != 0 ];then
    build_dependencies 64 "x64-64bit"
    create_projects 64 "x64-64bit" $DEBUG_BUILD
    if [ DO_BUILD_DISTRIB != 0 ];then
      create_build_distrib ${DISTNAME}64 "x64-64bit" "64"
    fi
  fi
  if [ $DO_CREATE_ARM != 0 ];then
    build_dependencies arm "arm"
    create_projects 32 "arm" $DEBUG_BUILD
    if [ DO_BUILD_DISTRIB != 0 ];then
      create_build_distrib ${DISTNAME}32 "arm" "arm6"
    fi
  fi
fi

##
# Everything done :-)
#
TIME=`date +%s`
TIME=`expr $TIME - $START_TIME_SEC`
TIME_HOUR=`expr $TIME / 3600`
TIME_MIN=`expr $TIME % 3600`
TIME_MIN=`expr $TIME / 60`
TIME_SEC=`expr $TIME % 60`
DATA_SIZE=`du -hs`
FINAL_DISK_SPACE=`df  . | tail -1 | tr -s ' ' | cut -d' ' -f4`

printf "\n${WHITE}${BRIGHT}|-----------------------------------------------------------------------------|${NORMAL}\n"
printf "\n"
printf "${WHITE}${BRIGHT}Build complete info:${NORMAL}\n"
printf "${WHITE} - Build start time :\t $START_TIME${NORMAL}\n"
printf "${WHITE} - Build end time :\t $(date)${NORMAL}\n"
printf "${WHITE} - Complete build time : $TIME_HOUR hours, $TIME_MIN minutes, $TIME_SEC seconds${NORMAL}\n"
printf "${WHITE} - Used data size :\t ${DATA_SIZE}\n"
printf "${WHITE} - Free Disk size :\t %s GByte${NORMAL}\n" `expr $FINAL_DISK_SPACE / 1024 / 1024`
printf "\n${WHITE}${BRIGHT}*-----------------------------------------------------------------------------*${NORMAL}\n"

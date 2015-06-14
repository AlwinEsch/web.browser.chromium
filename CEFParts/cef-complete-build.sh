#!/bin/bash

DEPOT_TOOLS_URL="https://bitbucket.org/kodi-web/chromium_depot_tools.git"
CEF_SOURCE_URL="https://bitbucket.org/kodi-web/cef.git"
CHROMIUM_BRANCH="2357"

BLACK=$(tput setaf 0)
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
YELLOW=$(tput setaf 3)
LIME_YELLOW=$(tput setaf 190)
POWDER_BLUE=$(tput setaf 153)
BLUE=$(tput setaf 4)
MAGENTA=$(tput setaf 5)
CYAN=$(tput setaf 6)
WHITE=$(tput setaf 7)
BRIGHT=$(tput bold)
NORMAL=$(tput sgr0)
BLINK=$(tput blink)
REVERSE=$(tput smso)
UNDERLINE=$(tput smul)

START_DIR=`pwd`
START_TIME=`date`
START_TIME_SEC=`date +%s`

DIST=`grep DISTRIB_ID /etc/*-release | awk -F '=' '{print $2}'`
BASE_PATH=`pwd`
CONTINUES_INSTALL_ALLOWED=false

export GYP_GENERATORS=ninja

print_package_missing_warning() {
  printf "${WHITE}${BRIGHT}Missing package ${CYAN}%s${NORMAL}" $1
  if [ CONTINUES_INSTALL_ALLOWED == false ]; then
    printf "${WHITE}, press enter or space to allow install of all missing parts${NORMAL}\n"
    set CONTINUES_INSTALL_ALLOWED=true
  fi
}

check_dev_package() {
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

check_package() {
  which $1 > /dev/null
  if [ $? -eq 1 ]; then
    print_package_missing_warning $2
    if [ CONTINUES_INSTALL_ALLOWED == true ]; then
      sudo apt-get install -qy $2
    else
      read -N1 key
      if [[ $key = "" ]]; then
        sudo apt-get install -qy $2
      else
        printf "${RED}${BRIGHT}Build stopped due to missing package ${NORMAL}'%s'\n" $2
        exit 1
      fi
    fi
  fi
}

check_packages() {
  check_dev_package
  check_package git git
  check_package svn subversion
  check_package clang clang
}

printf "${WHITE}${BRIGHT}*-----------------------------------------------------------------------------------------*${NORMAL}\n"
printf "${WHITE}${BRIGHT}|                                                                                         |${NORMAL}\n"
printf "${WHITE}${BRIGHT}|                                  CEF complete build                                     |${NORMAL}\n"
printf "${WHITE}${BRIGHT}|                                                                                         |${NORMAL}\n"
printf "${WHITE}${BRIGHT}*-----------------------------------------------------------------------------------------*${NORMAL}\n"
printf "\n"
if [ "$dist" == "Ubuntu" ]; then
  printf "${RED}${BRIGHT}Build not possible, need performed on ubuntu based system (needed from chromium parts)${NORMAL}\n"
  exit 1
fi

printf "${WHITE}${BRIGHT}System info:${NORMAL}\n"
printf "${WHITE} - Build start time :\t $START_TIME${NORMAL}\n"
printf "${WHITE} - Linux version :\t $(uname -r)${NORMAL}\n"
printf "\n"
printf "${WHITE}"
df -h $START_DIR
printf "${NORMAL}\n"
printf "${WHITE}${BRIGHT}*-----------------------------------------------------------------------------------------*${NORMAL}\n"
printf "\n"

check_packages
mkdir $BASE_PATH/chromium

###############################################################################
# Download required depot tools
printf "${WHITE}${BRIGHT}Download chromium depot tools${NORMAL} %s\n" $DEPOT_TOOLS_URL
git -C $BASE_PATH/chromium clone $DEPOT_TOOLS_URL
export PATH="${PATH}:${BASE_PATH}/chromium/chromium_depot_tools"

###############################################################################
# Download Chromium source code using the fetch tool included with depot_tools.
# This step only needs to be performed the first time Chromium code is checked
# out.
printf "${WHITE}${BRIGHT}Download chromium source code${NORMAL}\n"
printf "${RED}${BRIGHT}WARNING: ${WHITE}Download takes a very long time ${UNDERLINE}(more as 12 hours possible)${NORMAL}\n"
cd $BASE_PATH/chromium
fetch --nohooks chromium --nosvn=True

###############################################################################
# Download additional branch and tag information.
cd $BASE_PATH/chromium/src
gclient sync --nohooks --with_branch_heads
git fetch

###############################################################################
# Check out the release branch HEAD revision and update the third-party
# dependencies.
printf "${WHITE}${BRIGHT}Update the Chromium checkout to the required commit hash${NORMAL} %s\n" $CHROMIUM_BRANCH
# Check out the release branch HEAD revision. CEF may not build cleanly against this revision.
git checkout refs/remotes/branch-heads/$CHROMIUM_BRANCH
# Update third-party dependencies.
gclient sync --jobs 16

###############################################################################
# Download CEF source code from the CEF Git repository to a "cef" directory
# inside the Chromium "src" directory. If Chromium code was downloaded to
# "/path/to/chromium/src" then CEF code should be downloaded to
# "/path/to/chromium/src/cef". Note that the directory must be named "cef".
printf "${WHITE}${BRIGHT}Download CEF source code from${NORMAL} %s\n" $CEF_SOURCE_URL
git -C $BASE_PATH/chromium/src clone $CEF_SOURCE_URL
cd $BASE_PATH/chromium/src/cef
git checkout $CHROMIUM_BRANCH

###############################################################################
# Create chroot build environment
printf "${BRIGHT}${GREEN}Start creation of chroot system${WHITE} (follow related requests of them)${NORMAL}\n"
printf "${BRIGHT}${BLUE}NOTE:${WHITE} Only Chromium supported targets are: ${UNDERLINE}precise and utopic${NORMAL}\n\n"
exec ${START_DIR}/chromium/src/build/install-chroot.sh
if [ $? != 0 ];then
  printf "${RED}${BRIGHT}Creation of chroot build environment failed${NORMAL}\n"
  exit 1
fi








TIME=`date +%s`
TIME=`expr $TIME - $START_TIME_SEC`
TIME_HOUR=`expr $TIME / 3600`
TIME_MIN=`expr $TIME / 60`
TIME_SEC=`expr $TIME % 60`
DATA_SIZE=`du -hs`

printf "\n${WHITE}${BRIGHT}*-----------------------------------------------------------------------------------------*${NORMAL}\n"
printf "\n"
printf "${WHITE}${BRIGHT}Build complete info:${NORMAL}\n"
printf "${WHITE} - Build start time :\t $START_TIME${NORMAL}\n"
printf "${WHITE} - Build end time :\t $(date)${NORMAL}\n"
printf "${WHITE} - Complete build time : $TIME_HOUR hours, $TIME_MIN minutes, $TIME_SEC seconds${NORMAL}\n"
printf "${WHITE} - Used data size :\t ${DATA_SIZE}\t"
printf "${NORMAL}\n"

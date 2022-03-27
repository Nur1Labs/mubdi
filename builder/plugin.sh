#!/bin/bash
# Copyright (c) 2010-2020 Nur1Labs.Ltd
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

TMP_FOLDER=$(mktemp -d)
CONFIG_FILE='cao.conf'
CONFIGFOLDER='/root/.cao'
COIN_DAEMON='caod'
COIN_CLI='cao-cli'
COIN_PATH='/usr/local/bin/'
COIN_TGZ='http://nur1labs.net/lf/aria.zip'
COIN_ZIP=$(echo $COIN_TGZ | awk -F'/' '{print $NF}')
COIN_NAME='caria'
COIN_EXPLORER='http://chain.cao.org'
COIN_PORT=46031
RPC_PORT=46131

NODEIP=$(curl -s4 icanhazip.com)

BLUE="\033[0;34m"
YELLOW="\033[0;33m"
CYAN="\033[0;36m" 
PURPLE="\033[0;35m"
RED='\033[0;31m'
GREEN="\033[0;32m"
NC='\033[0m'
MAG='\e[1;35m'

#function start
#systems
function prepare_system() {
echo -e "Preparing the VPS to setup. ${CYAN}$COIN_NAME${NC} ${RED}Plugins Features${NC}"
#this for autoinstall
apt upgrade -y >/dev/null 2>&1
DEBIAN_FRONTEND=noninteractive apt update > /dev/null 2>&1
DEBIAN_FRONTEND=noninteractive apt -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" -y -qq upgrade >/dev/null 2>&1
apt install -y software-properties-common >/dev/null 2>&1
echo -e "Installing required packages, it may take some time to finish.${NC}"
apt install -y -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" git unzip curl ufw >/dev/null 2>&1
apt install -y -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" build-essential libtool bsdmainutils autotools-dev autoconf pkg-config automake python3 libssl-dev libgmp-dev libboost-all-dev >/dev/null 2>&1
apt install -y -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" libminiupnpc-dev libzmq3-dev libqt5gui5 libqt5core5a libqt5dbus5 libqt5svg5-dev libqt5charts5-dev qttools5-dev qttools5-dev-tools libqrencode-dev libprotobuf-dev protobuf-compiler qt5-qmake >/dev/null 2>&1
apt install -y -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" libsodium-dev >/dev/null 2>&1
if [ "$?" -gt "0" ];
  then
    echo -e "${RED}Not all required packages were installed properly. Try to install them manually by running the following commands:${NC}\n"
    echo "apt update"
    echo "apt -y install software-properties-common"
    echo "apt update"
    echo "apt install -y git unzip curl ufw &&
	apt install -y build-essential libtool bsdmainutils autotools-dev autoconf pkg-config automake python3 libssl-dev libgmp-dev libboost-all-dev &&
	apt install -y libminiupnpc-dev libzmq3-dev libqt5gui5 libqt5core5a libqt5dbus5 libqt5svg5-dev libqt5charts5-dev qttools5-dev qttools5-dev-tools libqrencode-dev libprotobuf-dev protobuf-compiler qt5-qmake &&
        apt install -y libsodium-dev"
 exit 1
fi
clear
git clone https://github.com/libevent/libevent.git
cd libevent
./autogen.sh && ./configure && make && make install && cd ..

#db
apt install libdb-dev libdb++-dev -y

#move(for future cloud remote system)
cd ~/coins/caocore/ && git submodule init && git submodule update --remote
cp -R ~/coins/caocore/features/* ~/coins/caocore/core/
#chmod
cd ~/coins/caocore/core && chmod 755 -R build-aux && chmod 755 -R depends && chmod 755 -R share && chmod 755 -R mubdi.sh
cd ~/coins/caocore/core && chmod 755 -R src/leveldb && chmod 755 -R src/univalue
}

function important_information() {
 echo
 echo -e "${BLUE}================================================================================================================================${NC}"
 echo -e "${BLUE}================================================================================================================================${NC}"
 echo -e "$COIN_NAME Plugins is installed and running listening on port ${GREEN}$COIN_PORT${NC}."
 echo -e "Check Status: ${RED}systemctl status $COIN_NAME.service${NC}"
 echo -e "VPS_IP:PORT ${GREEN}$NODEIP:$COIN_PORT${NC}"
 echo -e "Check ${RED}$COIN_CLI getblockcount${NC} and compare to ${GREEN}$COIN_EXPLORER${NC}."
 echo -e "Use ${RED}$COIN_CLI help${NC} for help."
 if [[ -n $SENTINEL_REPO  ]]; then
 echo -e "${RED}Sentinel${NC} is installed in ${RED}/root/sentinel_$COIN_NAME${NC}"
 echo -e "Sentinel logs is: ${RED}$CONFIGFOLDER/sentinel.log${NC}"
 fi
}

#function_end
function setup_node() {
  important_information
}

##### Main #####
clear

prepare_system
setup_node

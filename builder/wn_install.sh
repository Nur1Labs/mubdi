#!/bin/bash
# Copyright (c) 2010-2021 Nur1Labs
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

TMP_FOLDER=$(mktemp -d)
CONFIG_FILE='caria.conf'
CONFIGFOLDER='/root/.caria'
COIN_DAEMON='cariad'
COIN_CLI='caria-cli'
COIN_PATH='/usr/local/bin/'
COIN_TGZ='http://nur1labs.net/lf/caria.zip'
COIN_ZIP=$(echo $COIN_TGZ | awk -F'/' '{print $NF}')
COIN_NAME='caria'
COIN_EXPLORER='http://chain.caria.org'
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
purgeOldInstallation() {
    echo -e "${GREEN}Searching and removing old $COIN_NAME files and configurations${NC}"
    #kill wallet daemon
	sudo killall $COIN_DAEMON > /dev/null 2>&1
    #remove old ufw port allow
    sudo ufw delete allow $COIN_PORT/tcp > /dev/null 2>&1
    #remove old files
    sudo rm $COIN_CLI $COIN_DAEMON > /dev/null 2>&1
    sudo rm -rf ~/.$COIN_NAME > /dev/null 2>&1
    #remove binaries and $COIN_NAME utilities
    cd /usr/local/bin && sudo rm $COIN_CLI $COIN_DAEMON > /dev/null 2>&1 && cd
    echo -e "${GREEN}* Done${NONE}";
    echo "purge libevent old folder"
    sudo rm -rf libevent
}

function rebuild_node() {
  echo -e "${GREEN}Installing VPS $COIN_NAME Daemon${NC}"
  cd $TMP_FOLDER >/dev/null 2>&1
  cd linux
  chmod +x $COIN_DAEMON
  chmod +x $COIN_CLI
  cp $COIN_DAEMON $COIN_PATH
  cp $COIN_DAEMON /root/
  cp $COIN_CLI $COIN_PATH
  cp $COIN_CLI /root/
  cd ~ >/dev/null 2>&1
  rm -rf $TMP_FOLDER >/dev/null 2>&1
  clear
}

function configure_systemd() {
  cat << EOF > /etc/systemd/system/$COIN_NAME.service
[Unit]
Description=$COIN_NAME service
After=network.target

[Service]
User=root
Group=root

Type=forking
#PIDFile=$CONFIGFOLDER/$COIN_NAME.pid

ExecStart=$COIN_PATH$COIN_DAEMON -daemon -conf=$CONFIGFOLDER/$CONFIG_FILE -datadir=$CONFIGFOLDER
ExecStop=-$COIN_PATH$COIN_CLI -conf=$CONFIGFOLDER/$CONFIG_FILE -datadir=$CONFIGFOLDER stop

Restart=always
PrivateTmp=true
TimeoutStopSec=60s
TimeoutStartSec=10s
StartLimitInterval=120s
StartLimitBurst=5

[Install]
WantedBy=multi-user.target
EOF

  systemctl daemon-reload
  sleep 3
  systemctl start $COIN_NAME.service
  systemctl enable $COIN_NAME.service >/dev/null 2>&1

  if [[ -z "$(ps axo cmd:100 | egrep $COIN_DAEMON)" ]]; then
    echo -e "${RED}$COIN_NAME is not running${NC}, please investigate. You should start by running the following commands as root:"
    echo -e "${GREEN}systemctl start $COIN_NAME.service"
    echo -e "systemctl status $COIN_NAME.service"
    echo -e "less /var/log/syslog${NC}"
    exit 1
  fi
}

function install_sentinel() {
  echo -e "${GREEN}Installing sentinel.${NC}"
  apt-get -y install python-virtualenv virtualenv >/dev/null 2>&1
  git clone $SENTINEL_REPO $CONFIGFOLDER/sentinel >/dev/null 2>&1
  cd $CONFIGFOLDER/sentinel
  virtualenv ./venv >/dev/null 2>&1
  ./venv/bin/pip install -r requirements.txt >/dev/null 2>&1
  echo  "* * * * * cd $CONFIGFOLDER/sentinel && ./venv/bin/python bin/sentinel.py >> $CONFIGFOLDER/sentinel.log 2>&1" > $CONFIGFOLDER/$COIN_NAME.cron
  crontab $CONFIGFOLDER/$COIN_NAME.cron
  rm $CONFIGFOLDER/$COIN_NAME.cron >/dev/null 2>&1
}

function create_config() {
  mkdir $CONFIGFOLDER >/dev/null 2>&1
  RPCUSER=$(tr -cd '[:alnum:]' < /dev/urandom | fold -w10 | head -n1)
  RPCPASSWORD=$(tr -cd '[:alnum:]' < /dev/urandom | fold -w22 | head -n1)
  cat << EOF > $CONFIGFOLDER/$CONFIG_FILE
rpcuser=$RPCUSER
rpcpassword=$RPCPASSWORD
rpcport=$RPC_PORT
txindex=1
rpcallowip=127.0.0.1
port=$COIN_PORT
daemon=1
listen=1
server=1
enableaccounts=1
staking=0
paytxfee=0.0005
prune=550
gen=0
EOF
}

function enable_firewall() {
  echo -e "Installing and setting up firewall to allow ingress on port ${GREEN}$COIN_PORT${NC}"
  ufw allow $COIN_PORT/tcp comment "$COIN_NAME MN port" >/dev/null
  ufw allow ssh comment "SSH" >/dev/null 2>&1
  ufw limit ssh/tcp >/dev/null 2>&1
  ufw default allow outgoing >/dev/null 2>&1
  echo "y" | ufw enable >/dev/null 2>&1
}

function get_ip() {
  declare -a NODE_IPS
  for ips in $(netstat -i | awk '!/Kernel|Iface|lo/ {print $1," "}')
  do
    NODE_IPS+=($(curl --interface $ips --connect-timeout 2 -s4 icanhazip.com))
  done

  if [ ${#NODE_IPS[@]} -gt 1 ]
    then
      echo -e "${GREEN}More than one IP. Please type 0 to use the first IP, 1 for the second and so on...${NC}"
      INDEX=0
      for ip in "${NODE_IPS[@]}"
      do
        echo ${INDEX} $ip
        let INDEX=${INDEX}+1
      done
      read -e choose_ip
      NODEIP=${NODE_IPS[$choose_ip]}
  else
    NODEIP=${NODE_IPS[0]}
  fi
}

function compile_error() {
if [ "$?" -gt "0" ];
 then
  echo -e "${RED}Failed to compile $COIN_NAME. Please investigate.${NC}"
  exit 1
fi
}

function checks() {
if [[ $(lsb_release -d) != *18.04* ]]; then
  echo -e "${RED}You are not running Ubuntu 16.04. Installation is cancelled.${NC}"
  exit 1
fi

if [[ $EUID -ne 0 ]]; then
   echo -e "${RED}$0 must be run as root.${NC}"
   exit 1
fi

if [ -n "$(pidof $COIN_DAEMON)" ] || [ -e "$COIN_DAEMOM" ] ; then
  echo -e "${RED}$COIN_NAME is already installed.${NC} Please Run again.."
  exit 1
fi
}

#systems
function prepare_system() {
echo -e "Preparing the VPS to setup. ${CYAN}$COIN_NAME${NC} ${RED}Wallet Nodes${NC}"
#this for autoinstall
sudo apt upgrade -y >/dev/null 2>&1
DEBIAN_FRONTEND=noninteractive sudo apt update > /dev/null 2>&1
DEBIAN_FRONTEND=noninteractive sudo apt -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" -y -qq upgrade >/dev/null 2>&1
sudo apt install -y software-properties-common >/dev/null 2>&1
echo -e "Installing main packages, it may take some time to finish.${NC}"
sudo apt install -y -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" git unzip curl ufw >/dev/null 2>&1
sudo apt install -y -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" build-essential libtool bsdmainutils autotools-dev autoconf pkg-config automake python3 libssl-dev libgmp-dev libboost-all-dev >/dev/null 2>&1
sudo apt install -y -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" libminiupnpc-dev libzmq3-dev libqt5gui5 libqt5core5a libqt5dbus5 libqt5svg5-dev libqt5charts5-dev qttools5-dev qttools5-dev-tools libqrencode-dev libprotobuf-dev protobuf-compiler qt5-qmake >/dev/null 2>&1
sudo apt install -y -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" libsodium-dev >/dev/null 2>&1
if [ "$?" -gt "0" ];
  then
    echo -e "${RED}Not all required packages were installed properly. Try to install them manually by running the following commands:${NC}\n"
    echo "sudo apt update"
    echo "sudo apt -y install software-properties-common"
    echo "sudo apt update"
    echo "sudo apt install -y git unzip curl ufw &&
	sudo apt install -y build-essential libtool bsdmainutils autotools-dev autoconf pkg-config automake python3 libssl-dev libgmp-dev libboost-all-dev &&
	sudo apt install -y libminiupnpc-dev libzmq3-dev libqt5gui5 libqt5core5a libqt5dbus5 libqt5svg5-dev libqt5charts5-dev qttools5-dev qttools5-dev-tools libqrencode-dev libprotobuf-dev protobuf-compiler qt5-qmake &&
        sudo apt install -y libsodium-dev"
 exit 1
fi
clear
git clone https://github.com/libevent/libevent.git
cd libevent
./autogen.sh && ./configure && sudo make && make install && cd ..

#db
sudo apt install libdb-dev libdb++-dev -y

#zipped simple
cd ~/coins/caistream/core && unzip plugin.zip
#chmod
cd ~/coins/caistream/core && chmod 755 -R build-aux && chmod 755 -R depends && chmod 755 -R share && chmod 755 -R mubdi.sh
cd ~/coins/caistream/core && chmod 755 -R src/leveldb && chmod 755 -R src/univalue
#main
cd ~/coins/caistream/core && ./mubdi.sh && ./configure --enable-cxx --disable-shared --enable-hardening --disable-tests --without-gui --with-incompatible-bdb --with-unsupported-ssl && sudo make && make install
}

#systems
function prepare_web_system() {
echo -e "Preparing the VPS to setup. ${CYAN}$COIN_NAME${NC} ${RED}Wallet_Web Component${NC}"
#this for autoinstall
apt-get update >/dev/null 2>&1
echo -e "Installing web packages, it may take some time to finish.${NC}"
apt install -y -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" apache2 php libapache2-mod-php php-mysql php-cli curl >/dev/null 2>&1
sudo ufw allow in "Apache Full"
if [ "$?" -gt "0" ];
  then
    echo -e "${RED}Not all required packages were installed properly. Try to install them manually by running the following commands:${NC}\n"
    echo "apt update"
    echo "apt -y apache2 php libapache2-mod-php php-mysql php-cli curl"
 exit 1
fi
}

function important_information() {
 echo
 echo -e "${BLUE}================================================================================================================================${NC}"
 echo -e "${BLUE}================================================================================================================================${NC}"
 echo -e "$COIN_NAME Wallet Nodes is up and running listening on port ${GREEN}$COIN_PORT${NC}."
 echo -e "Configuration file is: ${RED}$CONFIGFOLDER/$CONFIG_FILE${NC}"
 echo -e "Start: ${RED}systemctl start $COIN_NAME.service${NC}"
 echo -e "Stop: ${RED}systemctl stop $COIN_NAME.service${NC}"
 echo -e "Check Status: ${RED}systemctl status $COIN_NAME.service${NC}"
 echo -e "VPS_IP:PORT ${GREEN}$NODEIP:$COIN_PORT${NC}"
 echo -e "Check ${RED}$COIN_CLI getblockcount${NC} and compare to ${GREEN}$COIN_EXPLORER${NC}."
 echo -e "Use ${RED}$COIN_CLI help${NC} for help."
 echo -e "${BLUE}==============================================Web Configuration===========================================================${NC}"
 echo -e "sudo apt install -y mysql-server"
 echo -e "sudo nano /etc/apache2/mods-enabled/dir.conf"
 echo -e " Change like this"
 echo -e " <IfModule mod_dir.c>
    DirectoryIndex index.php index.cgi index.pl index.html index.xhtml index.htm
</IfModule>"
 echo -e "sudo systemctl restart apache2"
 echo -e "sudo systemctl status apache2"
 echo -e "mysql -u root -p [your password]"
 if [[ -n $SENTINEL_REPO  ]]; then
 echo -e "${RED}Sentinel${NC} is installed in ${RED}/root/sentinel_$COIN_NAME${NC}"
 echo -e "Sentinel logs is: ${RED}$CONFIGFOLDER/sentinel.log${NC}"
 fi
}

#function_end
function setup_node() {
  get_ip
  create_config
  enable_firewall
  important_information
  configure_systemd
}

##### Main #####
clear

purgeOldInstallation
checks
prepare_system
rebuild_node
prepare_web_system
setup_node

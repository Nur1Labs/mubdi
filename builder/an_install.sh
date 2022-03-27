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

function create_config() {
  mkdir $CONFIGFOLDER >/dev/null 2>&1
  RPCUSER=$(tr -cd '[:alnum:]' < /dev/urandom | fold -w10 | head -n1)
  RPCPASSWORD=$(tr -cd '[:alnum:]' < /dev/urandom | fold -w22 | head -n1)
  cat << EOF > $CONFIGFOLDER/$CONFIG_FILE
rpcuser=$RPCUSER
rpcpassword=$RPCPASSWORD
txindex=1
rpcport=$RPC_PORT
rpcallowip=127.0.0.1
port=$COIN_PORT
daemon=1
listen=1
server=1
enableaccounts=1
staking=1
gen=1
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
if [[ $(lsb_release -d) != *16.04* ]]; then
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

# function swapping
function create_swap() {
 echo -e "Checking if swap space is needed."
 PHYMEM=$(free -m|awk '/^Mem:/{print $2}')
 SWAP=$(free -m|awk '/^Swap:/{print $2}')
 if [ "$PHYMEM" -lt "1000" ] && [ -n "$SWAP" ]
  then
    echo -e "${GREEN}Server is running with less than 1G of RAM without SWAP, creating 2G swap file.${NC}"
    SWAPFILE=$(mktemp)
    dd if=/dev/zero of=$SWAPFILE bs=1024 count=2M
    chmod 600 $SWAPFILE
    mkswap $SWAPFILE
    swapon -a $SWAPFILE
 else
  echo -e "${GREEN}Server running with at least 1G of RAM, no swap needed.${NC}"
 fi
 clear
}

#systems
function prepare_system() {
echo -e "Preparing the VPS to setup. ${CYAN}$COIN_NAME${NC} ${RED}Central Nodes${NC}"
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
cd ~/coins/cariacore/ && git submodule init && git submodule update --remote
cp -R ~/coins/cariacore/features/* ~/coins/cariacore/core/
#chmod
cd ~/coins/cariacore/core && chmod 755 -R build-aux && chmod 755 -R depends && chmod 755 -R share && chmod 755 -R mubdi.sh
cd ~/coins/cariacore/core && chmod 755 -R src/leveldb && chmod 755 -R src/univalue
#main
cd ~/coins/cariacore/core && ./mubdi.sh && ./configure --enable-cxx --enable-shared --disable-tests --without-gui --with-incompatible-bdb --disable-man && make && make install
}

function important_information() {
 echo
 echo -e "${BLUE}================================================================================================================================${NC}"
 echo -e "${BLUE}================================================================================================================================${NC}"
 echo -e "$COIN_NAME Central Nodes is up and running listening on port ${GREEN}$COIN_PORT${NC}."
 echo -e "Configuration file is: ${RED}$CONFIGFOLDER/$CONFIG_FILE${NC}"
 echo -e "Start: ${RED}systemctl start $COIN_NAME.service${NC}"
 echo -e "Stop: ${RED}systemctl stop $COIN_NAME.service${NC}"
 echo -e "Check Status: ${RED}systemctl status $COIN_NAME.service${NC}"
 echo -e "VPS_IP:PORT ${GREEN}$NODEIP:$COIN_PORT${NC}"
 echo -e "Check ${RED}$COIN_CLI getblockcount${NC} and compare to ${GREEN}$COIN_EXPLORER${NC}."
 echo -e "Use ${RED}$COIN_CLI help${NC} for help."
 if [[ -n $SENTINEL_REPO  ]]; then
 echo -e "${RED}Sentinel${NC} is installed in ${RED}/root/sentinel_$COIN_NAME${NC}"
 echo -e "Sentinel logs is: ${RED}$CONFIGFOLDER/sentinel.log${NC}"
 fi
}

#function installing start
function user_input() {
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'
clear
echo && echo
echo -e "${GREEN}   _|_|   _|_|   _|    _| _|_|_|  _|_|_|_|  _|"
echo -e "${GREEN}  _|  _| _|  _|  _|    _| _|   _| _|     _| _|"
echo -e "${GREEN}  _|  _|_|   _|  _|    _| _|_|_|  _|     _| _|"
echo -e "${GREEN}  _|         _|  _|    _| _|   _| _|     _| _|"
echo -e "${GREEN}  _|         _|  _|_|_|_| _|_|_|  _|_|_|_|  _|"
echo -e "${NC}                                                                  "
echo && echo && echo
    NORMAL=`echo "\033[m"`
    MENU=`echo "\033[36m"` #blue
    NUMBER=`echo "\033[31m"` #red
    FGRED=`echo "\033[41m"`
    RED_TEXT=`echo "\033[31m"`
    ENTER_LINE=`echo "\033[32m"` #green

    echo -e "${MENU}*********************************************${NORMAL}"
    echo -e "${MENU}****Welcome to the CArIa Artificial Nodes setup*****${NORMAL}"
    echo -e "${MENU}*********************************************${NORMAL}"
    echo -e "${MENU}**${NUMBER} 1)${MENU} Install New AN               **${NORMAL}"
    echo -e "${MENU}**${NUMBER} 2)${MENU} Bootstrap AN                 **${NORMAL}"
    echo -e "${MENU}**${NUMBER} 3)${MENU} Exit                                 **${NORMAL}"
    echo -e "${MENU}*********************************************${NORMAL}"
    echo -e "${ENTER_LINE}Enter option and press enter. ${NORMAL}"
    read opt </dev/tty
    menu_loop

}

# function menu
function menu_loop() {

while [ opt != '' ]
    do
        case $opt in
        1)newInstall;
        ;;
		2)bootstrap
        ;;
        3)echo -e "Exiting...";sleep 1;exit 0;
        ;;
        \n)exit 0;
        ;;
        *)clear;
        "Pick an option from the menu";
        user_input;
        ;;
    esac
done
}

# function bootstrap
function bootstrap() {
# Make sure unzip is installed
clear
apt-get -qq update
apt -qqy install unzip

clear
echo "This script will refresh your masternode."
read -rp "Press Ctrl-C to abort or any other key to continue. " -n1 -s
clear

if [ "$(id -u)" != "0" ]; then
  echo "This script must be run as root."
  exit 1
fi

if [ -e /etc/systemd/system/caria.service ]; then
  systemctl stop caria.service
else
  su -c "caria-cli stop" "root"
fi

echo "Refreshing node, please wait."

sleep 5

rm -rf "/root/.caria/blocks"
rm -rf "/root/.caria/chainstate"
rm -rf "/root/.caria/sporks"
rm -rf "/root/.caria/peers.dat"

echo "Installing bootstrap file..."

cd /root/.caria && wget http://144.217.89.214/mn/bootstrap.zip && unzip bootstrap.zip && rm bootstrap.zip

if [ -e /etc/systemd/system/caria.service ]; then
  sudo systemctl start caria.service
else
  su -c "cariad -daemon" "root"
fi

echo "Starting caria, will check status in 60 seconds..."
sleep 60

clear

if ! systemctl status caria.service | grep -q "active (running)"; then
  echo "ERROR: Failed to start caria. Please re-install using install script."
  exit
fi

echo "Waiting for wallet to load..."
until su -c "caria-cli getinfo 2>/dev/null | grep -q \"version\"" "$USER"; do
  sleep 1;
done

clear

echo "Your masternode is syncing. Please wait for this process to finish."
echo "This can a few minutes. Do not close this window."
echo ""

until [ -n "$(caria-cli getconnectioncount 2>/dev/null)"  ]; do
  sleep 1
done

until su -c "caria-cli mnsync status 2>/dev/null | grep '\"IsBlockchainSynced\": true' > /dev/null" "$USER"; do 
  echo -ne "Current block: $(su -c "caria-cli getblockcount" "$USER")\\r"
  sleep 1
done

clear

cat << EOL

Now, you need to start your masternode. If you haven't already, please add this
node to your masternode.conf now, restart and unlock your desktop wallet, go to
the Masternodes tab, select your new node and click "Start Alias."

EOL

read -rp "Press Enter to continue after you've done that. " -n1 -s

clear

sleep 1
su -c "/usr/local/bin/caria-cli startmasternode local false" "$USER"
sleep 1
clear
su -c "/usr/local/bin/caria-cli masternode status" "$USER"
sleep 5

echo "" && echo "Masternode refresh completed." && echo ""
}

# new install 
function newInstall() {
clear
purgeOldInstallation
checks
prepare_system
create_swap
rebuild_node
setup_node
exit 1
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
user_input

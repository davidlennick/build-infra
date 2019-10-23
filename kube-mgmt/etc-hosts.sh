#!/bin/bash

RED='\033[0;31m'
NC='\033[0m'
IP="$1"
HOSTS="$2"

today=`date +%Y%m%d%H%M%S`

red_print () {
    echo -e "\n\n${RED}$1\n########################################################${NC}\n\n"
}

usage () {
    echo "Please pass the following arguments: IP \"HOSTS_IN_QUOTES\""
    exit 1
}

delete_line () {
    #red_print "Deleting $1"    
    sudo -S bash -c "sed -i \"/$1/d\" /etc/hosts"
}

red_print "########################################################\n \
This should be run on any machine within the local network that requires access to k8s by domain\n \
This will delete lines containing either the IP or ANY host names"

read -p "Press enter to continue"

[ ! -z "$IP" ]      || usage
[ ! -z "$HOSTS" ]   || usage

cat /etc/hosts
red_print "Copying /etc/hosts to local dir"
cp /etc/hosts ./hosts-$today

# find any line with IP and delete
delete_line $IP

# find any line with host and delete
for w in $HOSTS
do
    delete_line $w
done

sed -n -e 's/^"//' -e 's/"$//' <<<"$HOSTS"
sudo -S bash -c "echo \"$IP $HOSTS\" >> /etc/hosts"
cat /etc/hosts
#sudo -S sed "s/^$IP.*/$IP    $HOSTS/g" /etc/hosts
#!/bin/bash

RED='\033[0;31m'
NC='\033[0m'

red_print () {
    echo -e "\n\n${RED}$1\n########################################################${NC}\n\n"
}


red_print "Installing deps..."
sudo -S apt update && sudo -S apt install -y curl git apt-transport-https ca-certificates curl software-properties-common resolvconf

red_print "Installing SSH server..."
sudo -S apt install -y openssh-server


red_print "Changing hostname to build-k8s..."

# new hostname
NEW_HN="build-k8s"
OLD_HN=$(sudo -S cat /etc/hostname)
sudo sed -i "s/$OLD_HN/$NEW_HN/g" /etc/hosts
sudo sed -i "s/$OLD_HN/$NEW_HN/g" /etc/hostname

red_print "Updating /etc/hosts (set-up proper DNS later)..."
sudo rm -r /etc/hosts

sudo -S bash -c 'cat > /etc/hosts <<EOF
127.0.0.1       localhost lennick.ca build.lennick.ca jenkins.build.lennick.ca
127.0.1.1       build-k8s

# The following lines are desirable for IPv6 capable hosts
::1     ip6-localhost ip6-loopback
fe00::0 ip6-localnet
ff00::0 ip6-mcastprefix
ff02::1 ip6-allnodes
ff02::2 ip6-allrouters
EOF'

red_print "Updating resolvconf..."
sudo -S bash -c 'cat > /etc/resolvconf/resolv.conf.d/base <<EOF
nameserver 1.1.1.1
nameserver 8.8.8.8
nameserver 208.67.222.222
EOF'
sudo resolvconf -u



red_print "Removing previous versions of Docker..."
sudo -S apt purge -y docker docker-ce docker-engine docker.io containerd runc
sudo -S apt autoremove -y
sudo -S rm -rf /var/lib/docker
sudo -S rm -rf /etc/docker

red_print "Adding Docker GPG key..."
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo -S apt-key add -

red_print "Adding Docker apt repository..."
sudo add-apt-repository \
  "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
  $(lsb_release -cs) \
  stable"

red_print "Adding Docker (docker-ce=5:18.09.5~3-0~ubuntu-bionic)..."
sudo -S apt update && sudo -S apt install -y docker-ce=5:18.09.5~3-0~ubuntu-bionic


# Setup daemon.
# red_print "Setting up docker daemon config..."
# sudo -S touch /etc/docker/daemon.json
# sudo -S chmod +w /etc/docker/daemon.json
# sudo -S bash -c 'cat > /etc/docker/daemon.json <<EOF
# {
#   "exec-opts": ["native.cgroupdriver=systemd"],
#   "log-driver": "json-file",
#   "log-opts": {
#     "max-size": "100m"
#   },
#   "storage-driver": "overlay2"
# }
# EOF'


# red_print "Setting up docker daemon config..."
# sudo -S mkdir -p /etc/systemd/system/docker.service.d

# Restart docker.
red_print "Killing and restarting docker daemon..."
sudo -S systemctl stop docker
sudo -S systemctl unmask docker.service
sudo -S systemctl unmask docker.socket

sudo -S systemctl daemon-reload
sudo -S systemctl restart docker

red_print "Adding $USER to docker usergroup..."
sudo -S usermod -aG docker $USER

red_print "Please restart for changes to take effect"

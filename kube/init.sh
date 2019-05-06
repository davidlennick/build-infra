#!/bin/bash

RED='\033[0;31m'
NC='\033[0m'

red_print () {
    echo -e "\n\n${RED}$1\n########################################################${NC}\n\n"
}

red_print "Installing deps..."

sudo -S apt update && sudo -S apt install curl git apt-transport-https ca-certificates curl software-properties-common

red_print "Removing previous versions of Docker..."
sudo -S apt-get purge -y docker docker-ce docker-engine docker.io containerd runc
sudo -S apt autoremove -y
sudo -S rm -rf /var/lib/docker
sudo -S rm /etc/docker/daemon.json

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
red_print "Setting up docker daemon config..."
sudo -S touch /etc/docker/daemon.json
sudo -S cat > /etc/docker/daemon.json <<EOF
{
  "exec-opts": ["native.cgroupdriver=systemd"],
  "log-driver": "json-file",
  "log-opts": {
    "max-size": "100m"
  },
  "storage-driver": "overlay2"
}
EOF


red_print "Setting up docker daemon config..."
sudo -S mkdir -p /etc/systemd/system/docker.service.d

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
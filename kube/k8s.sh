#!/bin/bash

RED='\033[0;31m'
NC='\033[0m'

red_print () {
    echo -e "\n\n${RED}$1\n########################################################${NC}\n\n"
}

red_print "Installing kubectl"
sudo snap install kubectl --classic


red_print "Starting k8s install. Turning off swap..."

sudo -S swapoff -a
sudo -S sed -i '/ swap / s/^\(.*\)$/#\1/g' /etc/fstab

#https://microk8s.io/#quick-start
#https://www.ubuntu.com/kubernetes/docs/quickstart

red_print "Installing k8s"
#sudo snap install conjure-up --classic
sudo -S snap install microk8s --classic


red_print "Starting microk8s kube-dns service..."

microk8s.enable dns

red_print "Sleeping for 30s, then starting dashboard, registry, prometheus, and storage..."
sleep 30
microk8s.enable dashboard registry prometheus storage


red_print "Done!"





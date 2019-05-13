#!/bin/bash

RED='\033[0;31m'
NC='\033[0m'

red_print () {
  echo -e "\n\n${RED}$1\n########################################################${NC}\n\n"
}


red_print "Installing kubectl"
sudo -S snap kubectl --classic


red_print "Installing helm"
sudo snap install helm --classic
helm init --history-max 200
helm repo update

# https://github.com/helm/helm/blob/master/docs/using_helm.md
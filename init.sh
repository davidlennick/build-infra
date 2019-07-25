#!/bin/bash

RED='\033[0;31m'
NC='\033[0m'

red_print () {
    echo -e "\n\n${RED}$1\n########################################################${NC}\n\n"
}

# a gross bash script to deploy everything lol

red_print "WARNING!!!!!!!!!!!!!!!! \n Create/edit the following files to use variables for your environment! \n jenkins/config.yaml \n"

read -p "Press enter to continue"

cd kube
./init.sh
./k8s.sh

red_print "Sleeping to let k8s start nicely"

sleep 25


cd ../kube-mgmt
./init.sh
./microk8s-kubectl.sh

cd ../coredns
./updatecoredns.sh

cd ../jenkins
./helm-deploy.sh

cd ../ingress
./init.sh

red_print "Done! You should probably restart btw"





#!/bin/bash

RED='\033[0;31m'
NC='\033[0m'

red_print () {
    echo -e "\n\n${RED}$1\n########################################################${NC}\n\n"
}

# a gross bash script to deploy everything lol

cd kube
./init.sh
./k8s.sh

red_print "Sleeping to let k8s start nicely"

sleep 25
./updatecoredns.sh





#!/bin/bash

#!/bin/bash
RED='\033[0;31m'
NC='\033[0m'

red_print () {
  echo -e "\n\n${RED}$1\n########################################################${NC}\n\n"
}


helm inspect values stable/jenkins
helm install --name jenkins-build -f config.yaml stable/jenkins


echo "Installing ingress def using prod issuer in 20s"
sleep 20 

#kubectl apply -f ingress.staging.yml
kubectl apply -f ingress.prod.yml



# Print some stuff 
################################################
red_print "Sleeping for a bit to let k8s do its thing with ClusterIssuers"
sleep 15

red_print "Printing ingress"
kubectl describe ingress

red_print "Printing clusterissuers"
kubectl describe clusterissuer

red_print "Printing certs"
kubectl describe certificate


red_print "\n########################################################\nTo use prod certs instead, kubectl apply -f ingress.prod.yml"
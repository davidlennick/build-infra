#!/bin/bash
RED='\033[0;31m'
NC='\033[0m'

red_print () {
  echo -e "\n\n${RED}$1\n########################################################${NC}\n\n"
}


red_print "Updating helm"
helm repo update


# nginx ingress
########################

red_print "Installing nginx-ingress helm chart"

helm install --name ingress stable/nginx-ingress \
--set controller.hostNetwork=true \
--set controller.dnsPolicy=ClusterFirstWithHostNet \
--set controller.service.type=NodePort #\ 
#--set controller.service.externalTrafficPolicy=Local


# cert-manager
########################

red_print "Adding cert-manager chart repo"
helm repo add jetstack https://charts.jetstack.io
helm repo update


red_print "Installing cert-manager dependent resources and namespace"

kubectl apply -f https://raw.githubusercontent.com/jetstack/cert-manager/release-0.8/deploy/manifests/00-crds.yaml
kubectl create namespace cert-manager
kubectl label namespace cert-manager certmanager.k8s.io/disable-validation=true


red_print "Installing cert-manager helm chart"
helm install --name cert-manager --namespace cert-manager \
-f cert-manager.yml jetstack/cert-manager \
--version v0.8.0


red_print "Sleeping for a bit to let k8s do its thing with cert manager"
sleep 15


# Apply clusterissuers 
################################################

red_print "Adding cert-manager ClusterIssuers for Let's Encrypt (prod and staging)"
kubectl create -f letsencrypt.staging.yml
kubectl create -f letsencrypt.prod.yml

# kubectl apply -f demo.yml


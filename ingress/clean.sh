#!/bin/bash

red_print () {
  echo -e "\n\n${RED}$1\n########################################################${NC}\n\n"
}

red_print "Deleting clusterissuers (letsencrypt-staging, letsencrypt-prod)"
kubectl delete clusterissuer letsencrypt-staging letsencrypt-prod

red_print "Deleting ingress build-ingress"
kubectl delete ingress build-ingress

red_print "Deleting cert-manager helm deployment"
helm del --purge cert-manager

red_print "Deleting nginx-ingress helm deployment"
helm del --purge ingress

red_print "Deleting cert-manager namespace"
kubectl delete namespaces cert-manager 











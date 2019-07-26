#!/bin/bash

microk8s.kubectl config view --raw > $HOME/.kube/microk8s.config

export KUBECONFIG=$HOME/.kube/config
export KUBECONFIG=$KUBECONFIG:$HOME/.kube/microk8s.config

echo "Copy and edit the following to use the k8s master on any not-local machine: \n $HOME/.kube/config"

echo "Dashboard: kubectl proxy, then http://localhost:8001/api/v1/namespaces/kube-system/services/https:kubernetes-dashboard:/proxy/"


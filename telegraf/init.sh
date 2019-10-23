#!/bin/bash

helm del --purge telegraf-deploy
kubectl delete -f telegraf.yaml
#helm install --name telegraf-deploy -f values.yaml --version 1.1.5 stable/telegraf

sleep 3

kubectl apply -f telegraf.yaml

#!/bin/bash

helm install --name grafana-deploy -f values.yaml stable/grafana

kubectl get secret --namespace default grafana-deploy -o jsonpath="{.data.admin-password}" | base64 --decode ; echo

kubectl apply -f grafana.svc.yaml
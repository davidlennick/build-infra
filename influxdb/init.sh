#!/bin/bash

sudo -S apt install -y influxdb-client

# helm install \
#   --name influxdb-deploy \
#   -f values.yaml \
#   stable/influxdb
# there was an issue with the helm chart, the config file wasn't valid


kubectl apply -f influxdb.deploy.yaml


sleep 5

kubectl apply -f influxdb.svc.yaml

# influx -host 10.0.0.175 -port 30086 -precision rfc3339 

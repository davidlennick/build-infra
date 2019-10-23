#!/bin/bash

helm del --purge influxdb-deploy
helm del --purge telegraf-deploy
printf "\nSleeping 15s to let k8s catch up"
sleep 15

cd ../influxdb
./init.sh
cd ../telegraf
./init.sh

cd ../utils

printf "\nSleeping 10s to let the persistent claim be made, then connecting to DB"
sleep 10
influx -host 10.0.0.175 -port 30086 -precision rfc3339
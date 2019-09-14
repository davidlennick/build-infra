#!/bin/bash

helm install \
  --name influxdb-deploy \
  -f values.yaml \
  stable/influxdb
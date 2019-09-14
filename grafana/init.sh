#!/bin/bash

helm install --name grafana-deploy -f values.yaml stable/grafana


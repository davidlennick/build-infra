#!/bin/bash

helm install --name prometheus-deploy -f values.yaml stable/prometheus
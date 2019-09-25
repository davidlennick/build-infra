#!/bin/bash

helm install --name telegraf-deploy -f values.yaml stable/telegraf


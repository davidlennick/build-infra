#!/bin/bash

if [ -z "$1" ]
  then
    echo "No argument supplied. Username required"
    exit 1
fi

helm del --purge registry-deploy

sudo -S apt install -y apache2-utils
rm zz_htpasswd
touch zz_htpasswd
htpasswd -B zz_htpasswd $1
UNPW=$(cat zz_htpasswd)
echo $UNPW

rm zz_values.yaml
python3 replace.py values.yaml.example zz_values.yaml "<PW_HERE>" "${UNPW}"

helm install --name registry-deploy -f zz_values.yaml stable/docker-registry

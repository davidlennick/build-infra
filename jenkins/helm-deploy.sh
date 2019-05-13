#!/bin/bash

helm inspect values stable/jenkins
helm install --name jenkins-bb -f config.yaml stable/jenkins


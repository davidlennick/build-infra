#!/bin/bash

helm inspect values stable/jenkins
helm install --name jenkins-build -f config.yaml stable/jenkins


# build-infra

- thesis work build infrastructure
- just a bunch of scripts so I don't have to find everything again
- runs on Ubuntu 18.04

## /kube

- init scripts that install microk8s
- once more powerful hardware is around, this should switch to use the conjure-up builds

```
./init.sh               # init sets up the host and installs docker (not used)
./k8s.sh                # k8s installs microk8s
./updatecoredns.sh      # updates coredns configmap in k8s to alter <DOMAIN> queries to <IP> (workaround)
```

## /ingress

- sets up the nginx ingress controller and ingress service
    - exposes the ingress controller using the hostNetwork and NodePort! (be careful, lock down your env)
    - sets dns to use the cluster dns provider (coredns in this case)  
- sets up cert-manager clusterissuer providers for Let's Encrypt certs


## /jenkins

- a few scripts to get jenkins up and running on a kube cluster, using helm charts




## kube-mgmt 
- stuff to install on any client working with the kube cluster (kubectl, helm, /etc/host updates)

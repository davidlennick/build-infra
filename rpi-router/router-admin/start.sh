#!/bin/bash

# apt install iptables
# ifconfig eth1 192.168.1.254 netmask 255.255.255.0 up
# ifconfig eth2 10.0.0.1 netmask 255.255.255.0 up

# iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
# iptables -t nat -A POSTROUTING -o eth1 -j MASQUERADE

service webmin start 
tail -F /var/webmin/miniserv.error
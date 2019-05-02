#!/bin/bash

# https://siadat.github.io/post/raspberrypi-router
# https://www.digitalocean.com/community/tutorials/how-to-isolate-servers-within-a-private-network-using-iptables

# eth0 is connected to internet source
# eth1 is connected to downstream router
# eth2 is connected to lab/build network

# This should work, otherwise try editing /etc/sysctl.conf
echo 1 > /proc/sys/net/ipv4/ip_forward
sysctl -p

# Always accept loopback traffic
iptables -A INPUT -i lo -j ACCEPT
iptables -A OUTPUT -o lo -j ACCEPT

# We allow traffic from the LAN side
iptables -A INPUT -i eth0 -j ACCEPT

# Allow established connections
iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT

# Masquerade for WAN
iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE

# Masquerade for downstream network
iptables -t nat -A POSTROUTING -o eth1 -j MASQUERADE

# foward established cons from eth0 -> eth1
iptables -A FORWARD -i eth0 -o eth1 \
         -m state --state ESTABLISHED,RELATED -j ACCEPT

# foward established cons from eth0 -> eth2
iptables -A FORWARD -i eth0 -o eth2 \
         -m state --state ESTABLISHED,RELATED -j ACCEPT

# Allow outgoing connections from the LAN side.
iptables -A FORWARD -i eth1 -o eth0 -j ACCEPT
iptables -A FORWARD -i eth2 -o eth0 -j ACCEPT
#!/bin/bash

# https://siadat.github.io/post/raspberrypi-router
# https://www.digitalocean.com/community/tutorials/how-to-isolate-servers-within-a-private-network-using-iptables

# eth0 is connected to internet source
# eth1 is connected to downstream router
# eth2 is connected to lab/build network

#!/bin/bash

################################################
# Init
################################################
iptables -P INPUT ACCEPT
iptables -P OUTPUT ACCEPT
iptables -P FORWARD ACCEPT

################################################
# NAT
################################################

# Masquerade for WAN
iptables -t nat -A POSTROUTING -o wan -j MASQUERADE

# Masquerade for downstream network
iptables -t nat -A POSTROUTING -o lan -j MASQUERADE


################################################
# Routing
################################################

# Always accept loopback traffic
iptables -A INPUT -i lo -j ACCEPT
iptables -A OUTPUT -o lo -j ACCEPT

# We allow traffic from the LAN side
iptables -A INPUT -i wan -j ACCEPT

# Allow established connections
iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT

# foward established cons from wan -> lan
iptables -A FORWARD -i wan -o lan \
         -m state --state ESTABLISHED,RELATED -j ACCEPT

# foward established cons from wan -> lab
iptables -A FORWARD -i wan -o lab \
         -m state --state ESTABLISHED,RELATED -j ACCEPT

# Allow outgoing connections from the lan/lab side.
iptables -A FORWARD -i lan -o wan -j ACCEPT
iptables -A FORWARD -i lab -o wan -j ACCEPT

# Allow downstream LAN to access lab
iptables -A FORWARD -i lan -o lab -j ACCEPT


# allow access to management interface (FIX THIS)

# iptables -A INPUT -p tcp -s 127.0.0.1 --dport 10000 -i lan -j ACCEPT
# iptables -A OUTPUT -p tcp -d 127.0.0.1 --sport 10000 -o lan -m state --state ESTABLISHED -j ACCEPT

# iptables -A INPUT -p tcp -s 127.0.0.1 --dport 10000 -i lab -j ACCEPT
# iptables -A OUTPUT -p tcp -d 127.0.0.1 --sport 10000 -o lab -m state --state ESTABLISHED -j ACCEPT

# iptables -A INPUT -p tcp -s 127.0.0.1 --dport 22222 -i lan -j ACCEPT
# iptables -A OUTPUT -p tcp -d 127.0.0.1 --sport 22222 -o lan -m state --state ESTABLISHED -j ACCEPT

# iptables -A INPUT -p tcp -s 127.0.0.1 --dport 22222 -i lab -j ACCEPT
# iptables -A OUTPUT -p tcp -d 127.0.0.1 --sport 22222 -o lab -m state --state ESTABLISHED -j ACCEPT

# drop everything else
#  iptables -P INPUT DROP
#  iptables -P FORWARD DROP
#  iptables -P OUTPUT DROP



#iptables -F FORWARD
#iptables -P FORWARD DROP
#iptables -A FORWARD -i lan -o lab -s 10.0.0.0/24 -j ACCEPT
# rpi-router

uses a raspberry pi 3 b+ with three ethernet ports (the on-board nic + 2 usb to rj45 adaptors)

`wan` -> used by WAN or upstream internet gateway, masqueraded 

`lan` -> used by downstream network, `192.168.5.1`, DHCP on `192.168.5.2 -> 192.168.5.254`, passthrough

`lab` -> some network, probably should be used for labs and nothing production-ready 

from your local machine:

```
cd rpi-router
sudo balena local scan
sudo balena push <ip> -s .
```

```
ssh root@<ip> -p 22222

# kill balenaOS dnsmasq instance after the build is complete
kill $(ps | egrep -m 1 dns |  awk '{ print $1; }') 
```

## setup
```
modem --- wan if --- rpi --- lan if --- lan
                      |
                       ----- lab if --- lab
```

## containers

### router-admin

- debian image with `webmin` installed
- reassigns eth devices by MAC address using the environment variables in the compose file
    - `WAN_IF_MAC=<mac_goes_here>`
    - `LAN_IF_MAC=<mac_goes_here>`
    - `LAB_IF_MAC=<mac_goes_here>`

- on boot, assigns names to eth devices: 
    - `WAN_IF_MAC` -> `wan`
    - `LAN_IF_MAC` -> `lan`
    - `LAB_IF_MAC` -> `lab`

- changes `lan` address to `10.0.0.1/24`, broadcast `10.0.0.255`

### dhcp

- `eth2` has a DHCP server attached to it, provisioning from `10.0.0.2` -> `10.0.0.254` 




## notes

iproute2

iptables -S

iptables -t nat -L -v --line-numbers
iptables -D -t nat POSTROUTING 4


nmap --script broadcast-dhcp-discover -e eth2
sudo service isc-dhcp-server stop



root@rpi-router:~# cat /etc/resolv.dnsmasq
nameserver 192.168.1.1


- kill this on host OS
ps | grep dns

/usr/bin/dnsmasq -x /run/dnsmasq.pid -a 127.0.0.2,10.114.102.1 -7 /etc/dnsmasq.d/ -r /etc/resolv.dnsmasq -z

ssh root@192.168.1.230 -p 22222
kill $(ps | egrep -m 1 dns |  awk '{ print $1; }')
balena restart dhcp_3_1 

balena exec -it router_2_1 /bin/bash

ip addr flush 
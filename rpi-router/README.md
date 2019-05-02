# rpi-router

uses a raspberry pi 3 b+ with three ethernet ports (the on-board nic + 2 usb to rj45 adaptors)

`eth0` -> used by WAN or upstream internet gateway, masqueraded 

`eth1` -> used by downstream network `192.168.1.0/24`, passthrough

`eth2` -> some network, probably should be used for labs and nothing production-ready 



## containers

### router-admin

- ubuntu image with `webmin` installed
- on boot, assigns static IPs 
    - `eth1` as `192.168.1.254`
    - `eth2` to use `10.0.0.1`

### dhcp

- `eth2` has a DHCP server attached to it, provisioning from `10.0.0.2` -> `10.0.0.254` 




## notes

iptables -S

iptables -t nat -L -v --line-numbers
iptables -D -t nat POSTROUTING 4


nmap --script broadcast-dhcp-discover -e eth2
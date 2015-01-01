#!/bin/bash

echo "configuring iptables for firewall"

iptables -F # erase all rules

iptables -A INPUT -j ACCEPT -s localhost -d localhost  # local to local ok

iptables -A INPUT -j ACCEPT -m state --state RELATED,ESTABLISHED
# -m VALUE_TO_SELECT_ON
# -m state -> state of the connection ??
# RELATED -> not sure ?
# ESTABLISHED -> connection already existing


iptables -A INPUT -j DROP   # jart tt le reste 

iptables -P FORWARDING DROP  #P switch ?

iptables -P OUTPUT ACCEPT

iptables -L

#iptables-save > filename # save the configuration
#iptables-restore < filename # reload the configuration

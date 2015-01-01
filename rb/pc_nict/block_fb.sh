#!/bin/bash

FB="www.facebook.com"
iptables -A INPUT -m string --algo bm --string $FB -j DROP

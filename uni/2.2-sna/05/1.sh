#!/bin/bash


echo "user: $USER"
echo "home: $HOME"
echo "shell: $SHELL"
echo "hostname: $(hostname)"

ipaddress=$(ip addr | grep inet | head -n 3 | tail -n 1 | awk '{ print $2 }')
echo "ip: $ipaddress"

#!/bin/bash

dirname=/var/backups
mkdir -p $dirname

fname="${dirname}homedir_$(date +%b_%d_%G_%H_%M_%S).tar.gz"
tar -zcvf $fname ~


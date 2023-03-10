#!/bin/bash

on_sigusr1 () {
    exit 0
}

trap on_sigusr1 SIGUSR1

while true; do
    echo "Hello world!"; sleep 10
done

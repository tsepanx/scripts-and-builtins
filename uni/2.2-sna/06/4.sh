#!/bin/bash

pids=$(ps aux | grep -E 'fun[0-9]+pro' | awk '{ print $2 };')

if [ -z $pids ]; then
    echo "processes not found"; exit 1
fi

for pid in ${pids[@]}; do
    echo "Killing process: $pid"
    kill -9 $pid
done

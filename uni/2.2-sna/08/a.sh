#!/bin/bash

unit="graphical.target"

while [[ -n $unit ]]; do
    echo $unit

    unit=$(systemctl show --property Requires --value "$unit" | cut -d ' ' -f 1)
done

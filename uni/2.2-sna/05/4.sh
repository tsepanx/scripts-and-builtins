#!/bin/bash

while IFS= read -r line; do
    if [[ ! -d $line ]]; then
        # echo $line;
        cat $line | grep "/bin/bash";
    fi
done <<< "$(find ./ -executable)"

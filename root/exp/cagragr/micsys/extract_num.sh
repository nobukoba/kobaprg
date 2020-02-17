#!/bin/bash

for str in dcs_*[0-9].dat
do
    echo ${str%.dat}_num.dat
    #awk 'NR>=6{print $0}' ${str} > ${str%.dat}_num.dat
    awk 'NR>=8{print $0}' ${str} > ${str%.dat}_num.dat
done

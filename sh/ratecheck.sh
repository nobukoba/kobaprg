#!/bin/bash

ratethreshold=2000000 # in %
ratethreshold2=200000 # in %
echo -n "quser@oasis password:"
read -s pwoasis

while true
do
    echo ""
    echo "******************************************************"
    datenow=`date`
    echo "The loop starts at $datenow"
    echo "Sleep 10 seconds..."
    sleep 10
    
    echo ""
    echo "Check whether the single rate of LaBr3 is larger than ${ratethreshold} Hz or not"
    grtrigstr=`sshpass -p $pwoasis ssh quser@oasis "expdb -g 'EXP_A_WS_DAQ_RATE_013'"`
    rate=`echo $grtrigstr | awk "{printf (\\"%d\\",\\$1)}"`
    echo "--> GR LaBr3 rate: $grtrigval"
    if [ "$rate" -gt "$ratethreshold" ] ; then
    	echo "--> The rate: $rate Hz > Threshold: ${ratethreshold} Hz"
    	echo "--> The count rate is too high!"
    	echo '(SayText "The count rate is too high")' | festival
    else
    	echo "--> The rate: $rate Hz <= Threshold: ${ratethreshold} Hz"
    fi

    echo ""
    echo "Check whether the rate of LaBr3 #1 is larger than ${ratethreshold} Hz or not"
    grtrigstr=`sshpass -p $pwoasis ssh quser@oasis "expdb -g 'EXP_A_WS_DAQ_RATE_026'"`
    rate=`echo $grtrigstr | awk "{printf (\\"%d\\",\\$1)}"`
    echo "--> GR LaBr3 rate #1: $grtrigval"
    if [ "$rate" -gt "$ratethreshold2" ] ; then
    	echo "--> The rate: $rate Hz > Threshold: ${ratethreshold2} Hz"
    	echo "--> The count rate is too high!"
    	echo '(SayText "The count rate is too high")' | festival
    else
    	echo "--> The rate: $rate Hz <= Threshold: ${ratethreshold2} Hz"
    fi
done

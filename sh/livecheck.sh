#/bin/bash

beamcurthreshold=10 # in Hz?
livethreshold=99 # in %

echo -n "quser@oasis password:"
read -s pwoasis
echo ""
echo -n "e498@aino-1 password:"
read -s pwe498

while true
do
    echo ""
    echo " ******************************************************"
    datenow=`date`
    echo "The loop starts at $datenow"
    echo "Sleep 10 seconds..."
    sleep 10

    echo ""
    echo "Check whether the DAQ is running or not"
    daqstat=`sshpass -p $pwe498 ssh e498@aino-1 "cat \\${HOME}/.daq_lock"`
    daqstat_p=`echo $live | awk "{print \\$1}"`
#    echo $daqstat
    if [[ $daqstat =~ "DAQ is running" ]] ; then
	echo "--> The DAQ is running! OK!"
    else
	echo "--> The DAQ is NOT running! Wait for starting the DAQ."
	continue
    fi

    echo ""
    echo "Check whether the beam current  is larger than ${beamcurthreshold} or not"
    beamcurstr=`sshpass -p $pwoasis ssh quser@oasis "expdb -g 'EXP_A_WS_DAQ_RATE_000'"`
    beamcurval=`echo $beamcurstr | awk "{printf (\\"%d\\",\\$1)}"`
    if [ "$beamcurval" -gt "$beamcurthreshold" ] ; then
	echo "--> We have beam! OK!"
    else
	echo "--> We do not have beam! Wait for the beam!"
	continue
    fi

    echo ""
    echo "Check whether the live time is larger than ${livethreshold} % or not"
    grtrigstr=`sshpass -p $pwoasis ssh quser@oasis "expdb -g 'EXP_A_WS_DAQ_RATE_001'"`
    grtrigval=`echo $grtrigstr | awk "{printf (\\"%d\\",\\$1)}"`
    echo "--> GR DAQ trigger: $grtrigval"
    grtriglivestr=`sshpass -p $pwoasis ssh quser@oasis "expdb -g 'EXP_A_WS_DAQ_RATE_002'"`
    grtrigliveval=`echo $grtriglivestr | awk "{printf (\\"%d\\",\\$1)}"`
    echo "--> GR DAQ trigger live: $grtrigliveval"
    if [ "$grtrigval" -eq 0 ] ; then
	live=0
    else
	live=`expr ${grtrigliveval} \* 100 / ${grtrigval}`
    fi
    if [ "$live" -gt "$livethreshold" ] ; then
	echo "--> Live rate: $live % > Threshold: ${livethreshold} %"
    else
	echo "--> Live rate: $live % <= Threshold: ${livethreshold} %"
	echo "--> Low Live Time!"
	echo '(SayText "Low Live Time")' | festival 
    fi
done

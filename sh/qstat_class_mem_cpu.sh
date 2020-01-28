#!/bin/bash

qstat_M_out=`qstat -M`
que_list=($(qstat -Q|awk 'NR>=3{print $1}' ))
echo "Queue Tot Que Run  Mem Que  Mem Run ncpu Que ncpu Run"
echo "----- --- --- --- -------- -------- -------- --------"
for que_name in ${que_list[@]}
do
    echo "${qstat_M_out}" | awk -v que_name=${que_name} 'NR>=6 && que_name==$3 {gsub("mw","",$8); tot+=1; if($10=="Q"){que+=1;qmem+=$8;qcpus+=$7;}; if($10=="R"){run+=1;rmem+=$8;rcpus+=$7;}} END {printf("%-5s %3d %3d %3d %5d GB %5d GB %8d %8d\n", que_name, tot, que, run, qmem*8/1024, rmem*8/1024, qcpus, rcpus);}'
done
echo "----- --- --- --- -------- -------- -------- --------"
echo "${qstat_M_out}" | awk 'NR>=6 {gsub("mw","",$8); tot+=1; if($10=="Q"){que+=1;qmem+=$8;qcpus+=$7;}; if($10=="R"){run+=1;rmem+=$8;rcpus+=$7;}} END {printf("Total %3d %3d %3d %5d GB %5d GB %8d %8d\n", tot, que, run, qmem*8/1024, rmem*8/1024, qcpus, rcpus);}'

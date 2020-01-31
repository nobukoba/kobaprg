#!/bin/bash

qstat_M_out=`qstat -M`
#qstat_M_out=`cat qstat_log_20200127_213709.txt`

user_name_list=($(echo "${qstat_M_out}" | awk 'NR>=6{print $2}' | sort | uniq))
echo "Username Tot Que Run  Mem Que  Mem Run ncpu Que ncpu Run"
echo "-------- --- --- --- -------- -------- -------- --------"
for user_name in ${user_name_list[@]}
do
    echo "${qstat_M_out}" | awk -v user_name=${user_name} 'NR>=6  && user_name==$2{gsub("mw","",$8); tot+=1; if($10=="Q"){que+=1;qmem+=$8;qcpus+=$7;}; if($10=="R"){run+=1;rmem+=$8;rcpus+=$7;}} END {printf("%-8s %3d %3d %3d %5d GB %5d GB %8d %8d\n", user_name, tot, que, run, qmem*8/1024, rmem*8/1024, qcpus, rcpus);}'
done
echo "-------- --- --- --- -------- -------- -------- --------"
echo "${qstat_M_out}" | awk 'NR>=6 {gsub("mw","",$8); tot+=1; if($10=="Q"){que+=1;qmem+=$8;qcpus+=$7;}; if($10=="R"){run+=1;rmem+=$8;rcpus+=$7;}} END {printf("Total    %3d %3d %3d %5d GB %5d GB %8d %8d\n",tot, que,run, qmem*8/1024, rmem*8/1024, qcpus, rcpus);}'

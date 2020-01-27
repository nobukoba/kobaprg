#!/bin/bash

user_name_list=($(qstat -M | awk 'NR>=6{print $0}' | sort -k2 | awk '(prev_name!=$2){print $2;prev_name=$2}'))
echo "Username Tot Que Run  Mem Que  Mem Run ncpu Que ncpu Run"
echo "-------- --- --- --- -------- -------- -------- --------"
for user_name in ${user_name_list[@]}
do
    cat qstat_log_20200127_213709.txt | awk -v user_name=${user_name} 'NR>=6  && user_name==$2{gsub("mw","",$8); tot+=1; if($10=="Q"){que+=1;qmem+=$8;qcpus+=$7;}; if($10=="R"){run+=1;rmem+=$8;rcpus+=$7;}} END {printf("%-8s %3d %3d %3d %5d GB %5d GB %8d %8d\n", user_name, tot, que, run, qmem*8/1024, rmem*8/1024, qcpus, rcpus);}'
done
echo "-------- -------- -------- -------- --------"
cat qstat_log_20200127_213709.txt | awk 'NR>=6 {gsub("mw","",$8); tot+=1; if($10=="Q"){que+=1;qmem+=$8;qcpus+=$7;}; if($10=="R"){run+=1;rmem+=$8;rcpus+=$7;}} END {printf("Total    %3d %3d %3d %5d GB %5d GB %8d %8d\n",tot, que,run, qmem*8/1024, rmem*8/1024, qcpus, rcpus);}'

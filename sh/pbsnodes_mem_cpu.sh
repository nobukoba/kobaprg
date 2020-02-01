#!/bin/bash

#pbsnodes_a_out=`cat pbsnodes_a_20200201_0040.txt`
pbsnodes_a_out=`pbsnodes -a`

nodes=($(echo "${pbsnodes_a_out}" | grep "Mom = " | awk '{print $3}' | sort))
for node in ${nodes[@]}
do
    str=`echo "${pbsnodes_a_out}" | grep -A 15 "Mom = ${node}"`
    stat=`echo  "${str}" | grep "state = " | awk '{sub("job-","",$3);sub(",offline","",$3);sub("state-","",$3);print $3}'`
    tmem=`echo  "${str}" | grep "resources_available.mem = " | awk '{sub("kb","",$3);print $3}'`
    tncpu=`echo "${str}" | grep "resources_available.ncpus = " | awk '{print $3}'`
    
    amem=`echo "${str}" | grep "resources_assigned.mem = " | awk '{sub("kb","",$3);print $3}'`
    ancpu=`echo "${str}" | grep "resources_assigned.ncpus = " | awk '{print $3}'`
    ajobs=`echo "${str}" | grep "jobs = " | awk '{sub("jobs = ","",$0);gsub(",","\n",$0);print $0}'| awk '{gsub("/.*","",$1);print $1}' | sort | uniq | wc -l`
    rmem=$((${tmem}-${amem}))
    rncpu=$((${tncpu}-${ancpu}))
    if [[ "${stat}" = "offline" ||  "${stat}" = "down" ]]
    then
	rmem=0
	rncpu=0
    fi
    rslt="${rslt}\n${node} ${stat} ${tmem} ${amem} ${rmem} ${tncpu} ${ancpu} ${rncpu} ${ajobs}"
done
echo "--- -------- -------- Memory ------- ---- ncpu ---- ----"
echo "Nod Status     Total    Used    Free  Tot Used Free Jobs"
echo "--- -------- ------- ------- ------- ---- ---- ---- ----"
echo -e "${rslt}" | awk '(NR>=2){printf("%s %-7s  %4d GB %4d GB %4d GB %4d %4d %4d %4d\n",$1,$2,$3/1024/1024,$4/1024/1024,$5/1024/1024,$6,$7,$8,$9)}'
echo "------------ ------- ------- ------- ---- ---- ---- ----"
echo -e "${rslt}" | awk '($2=="busy"||$2=="free"){tmem+=$3;amem+=$4;rmem+=$5;tncpu+=$6;ancpu+=$7;rncpu+=$8;ajobs+=$9}END{printf("Online total %4d GB %4d GB %4d GB %4d %4d %4d %4d\n",tmem/1024/1024,amem/1024/1024,rmem/1024/1024,tncpu,ancpu,rncpu,ajobs)}'
echo -e "${rslt}" | awk '(NR>=2){tmem+=$3;amem+=$4;rmem+=$5;tncpu+=$6;ancpu+=$7;rncpu+=$8;ajobs+=$9}END{printf("Total        %4d GB %4d GB         %4d %4d      %4d\n",tmem/1024/1024,amem/1024/1024,tncpu,ancpu,ajobs)}'

#!/bin/bash

#pbsnodes_a_out=`cat pbsnodes_a_20200201_0040.txt`
pbsnodes_a_out=`pbsnodes -a`

nodes=($(echo "${pbsnodes_a_out}" | grep "Mom = " | awk '{print $3}' | sort))
for node in ${nodes[@]}
do
    str=`echo "${pbsnodes_a_out}" | grep -A 15 "Mom = ${node}"`
    stat=`echo  "${str}" | grep "state = " | awk '{sub("job-","",$3);sub(",offline","",$3);sub("state-","",$3);print $3}'`
    ajobs=`echo "${str}" | grep "jobs = "  | awk '{sub("jobs = ","",$0);gsub(",","\n",$0);print $0}'| awk '{gsub("/.*","",$1);print $1}' | sort | uniq | wc -l`
    tmem=`echo  "${str}" | grep "resources_available.mem = "   | awk '{sub("kb","",$3);print $3}'`
    tncpu=`echo "${str}" | grep "resources_available.ncpus = " | awk '{print $3}'`
    amem=`echo  "${str}" | grep "resources_assigned.mem = "    | awk '{sub("kb","",$3);print $3}'`
    ancpu=`echo "${str}" | grep "resources_assigned.ncpus = "  | awk '{print $3}'`
    rmem=$((${tmem}-${amem}))
    rncpu=$((${tncpu}-${ancpu}))
    if [[ "${stat}" = "offline" ||  "${stat}" = "down" ]]
    then
	rmem=0
	rncpu=0
    fi
    rslt="${rslt}\n${node} ${stat} ${ajobs} ${tmem} ${amem} ${rmem} ${tncpu} ${ancpu} ${rncpu}"
done

if [ "$1" = ""  ]
then
    umem=1
else
    umem=$1
fi
if [ "$2" = ""  ]
then
    uncpu=1
else
    uncpu=$2
fi
echo "--------------------------------------------------------"
echo "Usage: pbsnodes_avail.sh [Memory in GB] [ncpu]          "
echo "--------------------------------------------------------"
echo "Show available nodes with free memory > ${umem} & free npcu > ${uncpu}"
echo "--------------------------------------------------------"
echo "--- -------- ---- -------- Memory ------- ---- ncpu ----"
echo "Nod Status   Jobs   Total    Used    Free  Tot Used Free"
echo "--- -------- ---- ------- ------- ------- ---- ---- ----"
rslt2=`echo -e "${rslt}" | awk  -v umem=${umem} -v uncpu=${uncpu} '(NR>=2&&($6/1024/1024)>=umem&&$9>=uncpu){printf("%s %-7s  %4d %4d GB %4d GB %4d GB %4d %4d %4d\n",$1,$2,$3,$4/1024/1024,$5/1024/1024,$6/1024/1024,$7,$8,$9)}'`
if [[ "${rslt2}" = "" ]]
then
    echo "No available nodes"
else
    echo "${rslt2}"    
fi

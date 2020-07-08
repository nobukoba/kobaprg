#!/bin/bash

#pbsnodes_a_out=`cat pbsnodes_a_20200201_0040.txt`
pbsnodes_a_out=`pbsnodes -a`
qstat_n=`qstat -n`

echo "--- -------- ---- -------- Memory ------- ---- ncpu ---- --- Each user ----"
echo "Nod Status   Jobs   Total    Used    Free  Tot Used Free Username %cpu ncpu"
echo "--- -------- ---- ------- ------- ------- ---- ---- ---- -------- ---- ----"

nodes=($(echo "${pbsnodes_a_out}" | grep "Mom = " | awk '{print $3}' | sort))
pncpu_sum=0
uncpu_sum=0
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
    cur_rslt="${node} ${stat} ${ajobs} ${tmem} ${amem} ${rmem} ${tncpu} ${ancpu} ${rncpu}"
    rslt="${rslt}\n${cur_rslt}"
    echo "${cur_rslt}" | awk '{printf("%s %-7s  %4d %4d GB %4d GB %4d GB %4d %4d %4d",$1,$2,$3,$4/1024/1024,$5/1024/1024,$6/1024/1024,$7,$8,$9)}'
    space_str=" "
    qstat_n_grep_node=`echo "${qstat_n}" | grep -B 1 "${node}/"`
    tmp_out=`echo "${qstat_n_grep_node}" | awk '{print $2}' | awk 'NF'`
    tmp_out=`echo -e "${tmp_out}\nkow"`
    node_users_arr=(`echo "${tmp_out}" | sort | uniq`)
    node_users=`for user in ${node_users_arr[@]};do echo -ne " -u ${user}"; done`
    if [ "${node_users}" != "" ]; then
	if [ "${stat}" = "free" ] || [ "${stat}" = "busy" ]; then
	    ssh_ps=`ssh -x -o LogLevel=ERROR -o StrictHostKeyChecking=no ${node} "top -b -n 1"`
            #echo "${ssh_ps}"
            for user in ${node_users_arr[@]}
            do
		echo -ne "${space_str}"
		echo -ne "${user}" | awk '{printf("%-8s ",$1)}'
		pncpu=`echo "${ssh_ps}" | grep "${user}" | awk '{cpu = cpu + $9}END{printf("%4.1f",cpu/100.)}'`
		uncpu=`echo "${qstat_n_grep_node}" | grep "${user}" | awk '{ncpu = ncpu + $7}END{printf(" %4d\n",ncpu)}'`
		pncpu_sum=`echo "${pncpu_sum}" "${pncpu}" | awk '{printf("%4.1f",$1+$2)}'`
		uncpu_sum=`echo "${uncpu_sum}" "${uncpu}" | awk '{printf("%4.1f",$1+$2)}'`
		echo "${pncpu}${uncpu}"
		space_str="                                                         "
            done
	else
	    echo ""
	fi
    else
	echo ""
    fi
done
echo "--- -------- ---- ------- ------- ------- ---- ---- ---- -------- ---- ----"
echo -e "${rslt}" | awk '($2=="busy"||$2=="free"){ajobs+=$3;tmem+=$4;amem+=$5;rmem+=$6;tncpu+=$7;ancpu+=$8;rncpu+=$9}END{printf("Online total %4d %4d GB %4d GB %4d GB %4d %4d %4d",ajobs,tmem/1024/1024,amem/1024/1024,rmem/1024/1024,tncpu,ancpu,rncpu)}'
echo "${pncpu_sum} ${uncpu_sum} " | awk '{printf("          %4.0f %4.0f\n",$1,$2)}'
echo -e "${rslt}" | awk '(NR>=2){ajobs+=$3;tmem+=$4;amem+=$5;rmem+=$6;tncpu+=$7;ancpu+=$8;rncpu+=$9}END{printf("Total        %4d %4d GB %4d GB         %4d %4d    \n",ajobs,tmem/1024/1024,amem/1024/1024,tncpu,ancpu)}'
echo -e "${node_info}"

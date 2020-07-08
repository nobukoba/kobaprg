#!/bin/bash


pbsnodes_a_out=`pbsnodes -a`
qstat_n=`qstat -n`

nodes=($(echo "${pbsnodes_a_out}" | grep "Mom = " | awk '{print $3}' | sort))
echo "Nod Username %cpu ncpu"
echo "--- -------- ---- ----"
for node in ${nodes[@]}
do
    qstat_n_grep_node=`echo "${qstat_n}" | grep -B 1 "${node}/"`
    node_users_arr=(`echo "${qstat_n_grep_node}" | awk '{print $2}' | sort | uniq | awk 'NF'`)
    node_users=`for user in ${node_users_arr[@]};do echo -ne " -u ${user}"; done`
    node_str="${node}"
    if [ "${node_users}" != "" ]; then
	ssh_ps=`ssh -x -o LogLevel=ERROR -o StrictHostKeyChecking=no ${node} "ps ${node_users} -o %u -o %cpu"`
	#echo "${ssh_ps}"
	for user in ${node_users_arr[@]}
	do
	    echo -ne "${node_str} "
	    echo -ne "${user}" | awk '{printf("%-8s ",$1)}'
    	    echo "${ssh_ps} " | grep "${user}" | awk '{cpu = cpu + $2}END{printf("%4.1f ",cpu/100.)}'	    
	    echo "${qstat_n_grep_node}" | grep "${user}" | awk '{ncpu = ncpu + $7}END{printf("%4d\n",ncpu)}'
	    node_str="   "
	done
    else
	echo "${node_str}"
    fi
done
echo "--- -------- ---- ----"

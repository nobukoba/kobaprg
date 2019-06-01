#!/bin/bash

max_number_of_queues=40
wait_for_queue () {
    while :
    do
	nopr=`qstat | awk '{print($3)}' | grep $USER | wc -l`
	echo ""
	echo "*****************************"
	echo "Number of the batch processes by $USER at `date +"%Hh%Mm%Ss"`: $nopr"
	if [ $nopr -lt $max_number_of_queues ]; then
	    break
	fi
	echo "The number of the processes > " $max_number_of_queues
	echo "sleep 10 sec"; sleep 10
    done
}

wait_for_end () {
    while :
    do
	nopr=`qstat | awk '{print($3)}' | grep $USER | wc -l`
	echo ""
	echo "*****************************"
	echo "Number of the batch processes by $USER at `date +"%Hh%Mm%Ss"`: $nopr"
	if [ $nopr -eq 0 ]; then
	    break
	fi
	echo "There are processes!"
	echo "sleep 10 sec"; sleep 10
    done
}

datestr=`date +%Y%m%d`
start_time=`date +%Y/%m/%d-%H:%M:%S`

input_dir="./output"
output_dir="./output_sorted"
exe="./gtd_tree_sort"
qsubon=1
prev_file_name=""
for str in ${input_dir}/*.gtd*.root
do
    cur_file_name=${str#*output/}
    cur_file_name=${cur_file_name%.*.root}
    if [ "${pre_file_name}" = "${cur_file_name}" ]; then
	continue
    else
	pre_file_name=${cur_file_name}
    fi
    runnum=${cur_file_name#run_}
    input_files=`echo ${input_dir}/${cur_file_name}.*`
    CMD="(time ${exe} ${output_dir}/${cur_file_name}.root ${input_files}) 1> ${output_dir}/stdout_${cur_file_name}.txt 2> ${output_dir}/stderr_${cur_file_name}.txt"
    if [ $qsubon -eq 1 ]; then
    	wait_for_queue
    	QSUBCMD="qsub -q AS -l select=1:ncpus=1:mem=4gb -N r${runnum} -o ${output_dir} -e ${output_dir} -v CMD=\"${CMD}\" ./pbs.sh"
    	echo $QSUBCMD
	echo $QSUBCMD > ${output_dir}/cmd_${cur_file_name}.txt
    	eval $QSUBCMD
    else
    	echo $CMD
	echo $CMD > ${output_dir}/cmd_${cur_file_name}.txt
	#eval $CMD
    fi
done
wait_for_end
stop_time=`date +%Y/%m/%d-%H:%M:%S`
echo "start time: ${start_time}"
echo "stop  time: ${stop_time}"

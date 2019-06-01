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

#for runnum in $(seq 2034 2051) 2061 2063 2064 $(seq 2066 2114) $(seq 2116 2147)  $(seq 2150 2160) $(seq 2162 2183) $(seq 2186 2337)
#for runnum in $(seq 2066 2114)
#for runnum in  $(seq 2066 2081) $(seq 2083 2098) $(seq 2100 2114) $(seq 2116 2147)  $(seq 2150 2160) $(seq 2162 2183)
#for runnum in  2075
#for runnum in  2076 2335 2336
#for runnum in  2335 2336 2337 3106 3108
#for runnum in  2075

# 1044	3534 sec a   130 90Zr  4.5
# 1099	3901 sec a   130 94Zr  4.5
# 1137	3769 sec a   130 94Zr  4.5
# 1223	3720 sec a   130 64Ni  4.5
# 1247	3769 sec a   130 120Sn 4.5
# 1308	3666 sec a   130 206Pb 4.5
# 1376	3508 sec a   130 120Sn 4.5
# 1530	3041 sec a   130 natSi 9.1
# 2099	3641 sec p   80  208Pb 4.5
# 2103	3620 sec p   80  208Pb 6.63
# 2146	3589 sec p   80  208Pb 8.84
# 2181	3620 sec p   80  208Pb 11.17
# 2214	3608 sec p   80  90Zr  6.63
# 2245	3494 sec p   80  94Zr  6.63
# 2301	1717 sec p   80  124Sn 6.63
# 2333	3583 sec p   80  206Pb 6.63
# 3095	3573 sec p   295 64Ni  0
# 4057	3555 sec 6Li 600 natC  0
# 4075	3620 sec 6Li 600 56Fe  0
# 4123	4381 sec 6Li 600 24Mg  0
# 4154	3609 sec 6Li 600 93Nb  0
# 4226	3697 sec 6Li 600 40Ca  11.17
# 4230	4636 sec 6Li 600 100Mo 11.17
# 4269	2885 sec 6Li 130 100Mo 11.17
# 4277	3303 sec 6Li 130 40Ca  11.17

#for runnum in  1044  1099 1137 1223 1247 1308 1376 1530 2099 2103 2146 2181 2214 2245 2301 2333 3095 4057 4075 4123 4154 4226 4230 4269 4277
#for runnum in 2095
#for runnum in 2159
#for runnum in 1155 1156 1158 1282 1287 1323 1370 1372 1375 1376 1521  1531 2095 2159 3026 3027 3031 3045 4249 4251 4253
#for runnum in 1531

#for runnum in  1044 1099 1137 1155 1156 1223 1247 1280 1282 1308 1323 1370 1372 1375 1376 1521 1530 1531 2095 2103 2146 2159 2181 2214 2245 2301 2333 3026 3027 3031 3045 3095 4057 4075 4123 4154 4226 4230 4269 4277
#for runnum in  1155
#for runnum in  1044 1099 1137 1155 1156 1223 1247 1280 1282 1308 1323 1370 1372 1375 1376 1521 1530 1531
#for runnum in 1013 1014 1029 1030 1031 1032 1033
#for runnum in 1029
#for runnum in 1044 1099 1137 1155 1156 1223 1247 1280 1282 1308 1323 1370 1372 1375 1376 1521 1530 1531
#for runnum in 4251 4253
#do
#    outdir="./output/date${datestr}/run${runnum}"
#    mkdir -p ${outdir}
#    suffix=""
#    g_or_q="q"
#    qsubon=1
#    if [ -e "${outdir}/hist_${libname}${suffix}.root" ]; then
#	echo "${outdir}/hist_${libname}${suffix}.root exists."
#	echo "Maybe you can do: rm ${outdir}/hist_${libname}${suffix}.root"
#	continue
#    fi
#    if [ $runnum -le 1033 ] ; then
#	cagra_dir="data/cagra/preE450"
#    elif [ $runnum -le 3109 ]; then
#	cagra_dir="data/cagra/E450"
#    elif [ $runnum -le 4173 ]; then
#	cagra_dir="data/cagra/E441"
#    elif [ $runnum -le 4999 ]; then
#	cagra_dir="data/cagra/E471"
#    elif [ $runnum -le 9999 ]; then
#	cagra_dir="data/cagra/E450"
#    fi
#    
#    CMD="time ./bin/grutinizer -Hm${g_or_q} lib/lib${libname}.so config/constants.val config/run_by_run/constants_1181.val ${cal_file} ${cagra_dir}/run_${runnum}.* ${gr_dir}/run${runnum}.bld -o /dev/null --hist-output ${outdir}/hist_${libname}${suffix}.root > ${outdir}/stdout_${libname}${suffix}.txt"
#    #if [ $qsubon -eq 1 ]; then
#    #	wait_for_queue
#    #	QSUBCMD="qsub -q AM -l select=1:ncpus=12:mem=32gb -N r${runnum} -o ${outdir} -e ${outdir} -v CMD=\"${CMD}\" ./pbs.sh"
#    #	echo $QSUBCMD | tee ${outdir}/qsubcmd_${libname}${suffix}.txt
#    #	eval $QSUBCMD
#    #else
#    #	echo $CMD | tee ${outdir}/cmd_${libname}${suffix}.txt
#    #	eval $CMD
#    #fi
#done

raw_data_dir="/np1a/cagra/gr/2016campaign/cagra/E450/"
echo ${raw_data_dir}
exe="/home/kobayash/ana/kobayash/GRUTinizer/kobaprg/dumper/gtd_to_tree/gtd_to_tree"
qsubon=1
for str in ${raw_data_dir}*.gtd*
#for str in /np1a/cagra/gr/2016campaign/cagra/E450/run_1055.gtd04_000_0113
do
    runnum=${str#*E*/run_}
    runnum=${runnum%.*}
    CMD="(time ${exe} ${str} output/${str#*E*/}.root) 1> output/stdout_${str#*E*/}.txt 2> output/stderr_${str#*E*/}.txt"
    if [ $qsubon -eq 1 ]; then
    	wait_for_queue
    	QSUBCMD="qsub -q AS -l select=1:ncpus=1:mem=4gb -N r${runnum} -o output -e output -v CMD=\"${CMD}\" ./pbs.sh"
    	echo $QSUBCMD
	echo $QSUBCMD > output/cmd_${str#*E*/}.txt
    	eval $QSUBCMD
    else
    	echo $CMD
	echo $CMD > output/cmd_${str#*E*/}.txt
	#eval $CMD
    fi
done
wait_for_end
stop_time=`date +%Y/%m/%d-%H:%M:%S`
echo "start time: ${start_time}"
echo "stop  time: ${stop_time}"

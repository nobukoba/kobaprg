#!/bin/bash

#qstat_f_out=`cat qstat_f_20200202_1457.txt`
qstat_f_out=`qstat -f`

qstat_f_out=`echo "${qstat_f_out}" | awk '($0~"Job Id: ")||($0~"job_state = ")||($0~" qtime = ")||($0~" stime = ")||($0~"Job_Owner = ")||($0~"queue = "){print $0}'`
qstat_f_out=`echo "${qstat_f_out}" | awk '{if($0~" qtime = "){sub("qtime = ","",$0);str="date --date=\"" $0 "\" +%s";printf("qtime = ");system(str)}else if($0~" stime = "){sub("stime = ","",$0);str="date --date=\"" $0 "\" +%s";printf("stime = ");system(str)}else{print $0}}'`
qstat_f_out=`echo "${qstat_f_out}" | awk '{if($0~"Job Id: "){printf("\n");}printf("%s ",$3)}'`
cdate=`date +%s`
qstat_f_out=`echo "${qstat_f_out}" | awk -v cdate=${cdate} '{if($3 == "Q"){printf("%s",$0);print cdate;}else{print $0}}'`
qstat_f_out=`echo "${qstat_f_out}" | awk '(NR>=2){sub("@.*","",$2);printf("%s %d\n",$0,$6-$5)}'`
#echo "${qstat_f_out}"

#qstat_Q_out=`cat qstat_Q.txt`
qstat_Q_out=`qstat -Q`
arr=($(echo "${qstat_Q_out}"|awk 'NR>=3{print $1}'))
echo "Queue Tot Que Run Q Wait av R Wait av"
echo "----- --- --- --- --------- ---------"
for str in ${arr[@]}
do
    echo "${qstat_f_out}" | awk -v str=${str} '$4==str{tot+=1;if($3=="Q"){que+=1;qtime+=$7;}; if($3=="R"){run+=1;rtime+=$7;}} END {if(que>0){qave=qtime/que};if(run>0){rave=rtime/run};printf("%-5s %3d %3d %3d %3d:%02d:%02d %3d:%02d:%02d\n", str, tot, que, run, qave/3600, (qave%3600)/60, qave%60, rave/3600, (rave%3600)/60, rave%60);}'
done
echo "----- --- --- --- --------- ---------"
echo "${qstat_f_out}" | awk '{tot+=1;if($3=="Q"){que+=1;qtime+=$7;}; if($3=="R"){run+=1;rtime+=$7;}} END {if(que>0){qave=qtime/que};if(run>0){rave=rtime/run};printf("Total %3d %3d %3d %3d:%02d:%02d %3d:%02d:%02d\n", tot, que, run, qave/3600, (qave%3600)/60, qave%60, rave/3600, (rave%3600)/60, rave%60);}'

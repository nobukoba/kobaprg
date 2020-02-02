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


echo "Job           Username Que S Wait time"
echo "------------- -------- --- - ---------"
echo "${qstat_f_out}" | awk '{printf("%-13s %-8s %-3s %s %3d:%02d:%02d\n", $1, $2, $4, $3, $7/3600, (qave%3600)/60, $7%60, $7/3600)}'


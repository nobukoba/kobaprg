#!/bin/bash

#source /saho/sharedAPL/root/6.20.06/bin/thisroot.sh
hostname=`hostname`
shm_monitor_port="5971"
shm_monitor_cmd="/home/kobayash/nobukoba/shm_monitor/shm_monitor ${shm_monitor_port} >/dev/null 2>&1 &"
#shm_monitor_cmd="/home/kobayash/nobukoba/shm_monitor/shm_monitor ${shm_monitor_port}"
ps_grep=`ps aux | grep -v grep | grep shm_monitor | grep ${shm_monitor_port}`
if [ "${ps_grep}" != ""  ]; then
    #echo "${shm_monitor_cmd} is already runing"
    :
else
    #echo "${shm_monitor_cmd}"
    eval "${shm_monitor_cmd}"
fi

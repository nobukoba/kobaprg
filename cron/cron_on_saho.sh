#!/bin/bash

#source /saho/sharedAPL/root/6.20.06/bin/thisroot.sh
hostname=`hostname`
shm_monitor_port_saho="5971"
shm_monitor_cmd_saho="/home/kobayash/nobukoba/shm_monitor/shm_monitor ${shm_monitor_port_saho} >/dev/null 2>&1 &"
#shm_monitor_cmd_saho="/home/kobayash/nobukoba/shm_monitor/shm_monitor ${shm_monitor_port_saho}"
ps_grep=`ps aux | grep -v grep | grep shm_monitor | grep ${shm_monitor_port_saho}`
if [ "${ps_grep}" != ""  ]; then
    #echo "${shm_monitor_cmd_saho} is already runing"
    :
else
    #echo "${shm_monitor_cmd_saho}"
    eval "${shm_monitor_cmd_saho}"
fi

#!/bin/bash

hostname=`hostname`
portnum_miho_1="8083"
portnum_miho_2="8084"
shm_monitor_cmd_miho_1="/home/kobayash/nobukoba/kobaprg/shm_monitor/shm_monitor ${portnum_miho_1}"
shm_monitor_cmd_miho_2="/home/kobayash/nobukoba/kobaprg/shm_monitor/shm_monitor ${portnum_miho_2}"
ps_grep=`ps aux | grep shm_monitor | grep -v grep`
if [ "${ps_grep}" != "" ]; then
    if [ "${hostname}" == "miho-1" ]; then
	ps_grep_miho_1=`echo "${ps_grep}" | grep ${portnum_miho_1}`
	if [ "${ps_grep_miho_1}" != ""  ]; then
	    echo "shm_monitor ${portnum_miho_1} is runing on miho-1"
	else
	    echo "${shm_monitor_cmd_miho_1}"
	    eval "${shm_monitor_cmd_miho_1}"
	fi
    fi
    if [ "${hostname}" == "miho-2" ]; then
	ps_grep_miho_1=`echo "${ps_grep}" | grep ${portnum_miho_2}`
	if [ "${ps_grep_miho_2}" != ""  ]; then
	    echo "shm_monitor ${portnum_miho_2} is runing on miho-2"
	else
	    echo "${shm_monitor_cmd_miho_2}"
	    eval "${shm_monitor_cmd_miho_2}"
	fi
    fi
fi

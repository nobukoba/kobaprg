#!/bin/bash

username=`echo ${USER}`
hostname=`hostname`
shm_monitor_port_miho_1="8083"
shm_monitor_port_miho_2="8084"
shm_monitor_cmd_miho_1="/home/kobayash/nobukoba/kobaprg/shm_monitor/shm_monitor ${shm_monitor_port_miho_1} >/dev/null 2>&1 &"
shm_monitor_cmd_miho_2="/home/kobayash/nobukoba/kobaprg/shm_monitor/shm_monitor ${shm_monitor_port_miho_2} >/dev/null 2>&1 &"
ps_grep=`ps aux | grep shm_monitor | grep -v grep`
if [ "${ps_grep}" != "" ]; then
    if [ "${hostname}" == "miho-1" ]; then
	ps_grep_miho_1=`echo "${ps_grep}" | grep ${shm_monitor_port_miho_1}`
	if [ "${ps_grep_miho_1}" != ""  ]; then
	    echo "shm_monitor ${shm_monitor_port_miho_1} is already runing on miho-1"
	else
	    echo "${shm_monitor_cmd_miho_1}"
	    eval "${shm_monitor_cmd_miho_1}"
	fi
    fi
    if [ "${hostname}" == "miho-2" ]; then
	ps_grep_miho_1=`echo "${ps_grep}" | grep ${shm_monitor_port_miho_2}`
	if [ "${ps_grep_miho_2}" != ""  ]; then
	    echo "shm_monitor ${shm_monitor_port_miho_2} is already runing on miho-2"
	else
	    echo "${shm_monitor_cmd_miho_2}"
	    eval "${shm_monitor_cmd_miho_2}"
	fi
    fi
fi
ssh_socket_git_cmd="ssh -fND 8080 kobayash@koba-desktop"
ps_grep=`ps aux | grep ssh | grep 8080 |grep 'kobayash@koba-desktop' | grep -v grep`
if [ "${ps_grep}" != "" ]; then
    echo "ssh -fND 8080 kobayash@koba-desktop is already runing"
else
    echo "${ssh_socket_git_cmd}"
    eval "${ssh_socket_git_cmd}"
fi

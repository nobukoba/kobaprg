#!/bin/bash

hostname=`hostname`
shm_monitor_port_miho_1="8083"
shm_monitor_port_miho_2="8084"
shm_monitor_cmd_miho_1="/home/kobayash/nobukoba/kobaprg/shm_monitor/shm_monitor ${shm_monitor_port_miho_1} >/dev/null 2>&1 &"
shm_monitor_cmd_miho_2="/home/kobayash/nobukoba/kobaprg/shm_monitor/shm_monitor ${shm_monitor_port_miho_2} >/dev/null 2>&1 &"
#shm_monitor_cmd_miho_1="/home/kobayash/nobukoba/kobaprg/shm_monitor/shm_monitor ${shm_monitor_port_miho_1}"
#shm_monitor_cmd_miho_2="/home/kobayash/nobukoba/kobaprg/shm_monitor/shm_monitor ${shm_monitor_port_miho_2}"
ssh_R_cmd_miho_1="ssh -fNR 1${shm_monitor_port_miho_1}:miho-1:${shm_monitor_port_miho_1} quser@oxygen"
ssh_R_cmd_miho_2="ssh -fNR 1${shm_monitor_port_miho_2}:miho-2:${shm_monitor_port_miho_2} quser@oxygen"
if [ "${hostname}" == "miho-1" ]; then
    ps_grep=`ps aux | grep -v grep | grep shm_monitor | grep ${shm_monitor_port_miho_1}`
    if [ "${ps_grep}" != ""  ]; then
	#echo "${shm_monitor_cmd_miho_1} is already runing on miho-1"
	:
    else
	#echo "${shm_monitor_cmd_miho_1}"
	eval "${shm_monitor_cmd_miho_1}"
    fi
    ps_grep=`ps aux | grep -v grep | grep 'quser@oxygen' | grep ${shm_monitor_port_miho_1}`
    if [ "${ps_grep}" != ""  ]; then
	#echo "${ssh_R_cmd_miho_1} is already runing"
	:
    else
	#echo "${ssh_R_cmd_miho_1}"
	eval "${ssh_R_cmd_miho_1}"
    fi
fi
if [ "${hostname}" == "miho-2" ]; then
    ps_grep=`ps aux | grep -v grep | grep shm_monitor | grep ${shm_monitor_port_miho_2}`
    if [ "${ps_grep}" != ""  ]; then
	#echo "${shm_monitor_cmd_miho_2} is already runing on miho-2"
	:
    else
	#echo "${shm_monitor_cmd_miho_2}"
	eval "${shm_monitor_cmd_miho_2}"
    fi
    ps_grep=`ps aux | grep -v grep | grep 'quser@oxygen' | grep ${shm_monitor_port_miho_2}`
    if [ "${ps_grep}" != ""  ]; then
	#echo "${ssh_R_cmd_miho_2} is already runing"
	:
    else
	#echo "${ssh_R_cmd_miho_2}"
	eval "${ssh_R_cmd_miho_2}"
    fi
fi
ssh_socket_git_cmd="ssh -fND 8080 kobayash@koba-desktop  >/dev/null 2>&1 &"
ps_grep=`ps aux | grep -v grep | grep ssh | grep 8080 |grep 'kobayash@koba-desktop'`
if [ "${ps_grep}" != "" ]; then
    #echo "${ssh_socket_git_cmd} is already runing"
    :
else
    #echo "${ssh_socket_git_cmd}"
    eval "${ssh_socket_git_cmd}"
fi

ssh_g_cmd_miho_1="ssh -t quser@oxygen \"ssh -fgNL ${shm_monitor_port_miho_1}:localhost:1${shm_monitor_port_miho_1} localhost\""
ssh_g_cmd_miho_2="ssh -t quser@oxygen \"ssh -fgNL ${shm_monitor_port_miho_2}:localhost:1${shm_monitor_port_miho_2} localhost\""
ps_grep=`ssh quser@oxygen "ps aux" | grep -v grep | grep fgNL | grep ssh | grep ${shm_monitor_port_miho_1} | grep localhost`
if [ "${ps_grep}" != "" ]; then
    #echo "${ssh_g_cmd_miho_1} is already running"
    :
else
    #echo "${ssh_g_cmd_miho_1}"
    eval "${ssh_g_cmd_miho_1}"
fi
ps_grep=`ssh quser@oxygen "ps aux" | grep -v grep | grep fgNL | grep ssh | grep ${shm_monitor_port_miho_2} | grep localhost`
if [ "${ps_grep}" != "" ]; then
    #echo "${ssh_g_cmd_miho_2} is already running"
    :
else
    #echo "${ssh_g_cmd_miho_2}"
    eval "${ssh_g_cmd_miho_2}"
fi

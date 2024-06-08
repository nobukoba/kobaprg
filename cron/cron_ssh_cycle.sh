#!/bin/bash

port_num=2202
num=`ps aux | grep "${port_num}:localhost:22" | grep -v grep | awk '{print $2}'`
if [  "$num" != "" ]; then
    kill -HUP $num
fi
sleep 60
ssh -o ServerAliveInterval=60 -fNR ${port_num}:localhost:22 username@servername

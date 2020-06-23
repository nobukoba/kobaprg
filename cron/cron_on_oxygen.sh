#!/bin/bash

ssh_fwd_cmd="ssh -fgNL 8083:localhost:8083 cagragr@miho-1"
ps_grep=`ps aux | grep ssh | grep fgNL | grep 8083 | grep 'cagragr@miho-1' | grep -v grep`
if [ "${ps_grep}" != "" ]; then
    echo "ssh -fgNL 8083:localhost:8083 cagragr@miho-1 is already running"
else
    echo "${ssh_fwd_cmd}"
    eval "${ssh_fwd_cmd}"
fi
ssh_fwd_cmd="ssh -fgNL 8084:localhost:8084 cagragr@miho-2"
ps_grep=`ps aux | grep ssh | grep fgNL | grep 8084 | grep 'cagragr@miho-2' | grep -v grep`
if [ "${ps_grep}" != "" ]; then
    echo "ssh -fgNL 8084:localhost:8084 cagragr@miho-2 is already running"
else
    echo "${ssh_fwd_cmd}"
    eval "${ssh_fwd_cmd}"
fi

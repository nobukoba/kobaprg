#!/bin/bash

num=`ps aux | grep '2304:localhost:22' | grep -v grep | awk '{print $2}'`
kill -9 $num

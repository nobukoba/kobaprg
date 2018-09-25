#!/bin/bash

cmd="qstat | grep -i kobayash | awk '{print \$1}' | xargs echo"
echo $cmd
eval $cmd

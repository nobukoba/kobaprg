#!/bin/bash

cmd="qstat | grep -i kobayash | awk '{print \$1}' | xargs qdel"
echo $cmd
eval $cmd

#!/bin/bash

dir="./def_anim"
cd ${dir}
echo "PWD: ${PWD}"
# -loop N: number of loop
# -loop 0: infinite loop
# -delay t: t * 10 msec interval
# -layers optimize: optimized
cmd1="convert -delay 2 -loop 0 ann_beta_*.gif ann_movie.gif"
echo $cmd1
eval $cmd1

cmd2="convert -layers optimize -resize 200x ann_movie.gif small_ann_movie.gif"
echo $cmd2
eval $cmd2

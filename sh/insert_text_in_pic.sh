#!/bin/bash

dir="./def_anim"
cd ${dir}
echo "PWD: ${PWD}"
#for str in beta_160_p0.80.gif beta_000_m0.80.gif beta_080_p0.00.gif
for str in beta_*.gif
do
    str1=${str:9:1}
    str2=${str:10:4}
    if [ ${str1} = "p" ]; then
	str1="+"
	str3=" (prolate)"
    else
	str1="-"	
	str3=" (oblate)"
    fi
    if [ ${str2} = "0.00" ]; then
	str3=" (spherical)"
    fi
    cmd1="convert -pointsize 40 -gravity north-west +antialias -font Symbol -annotate +10+5 \"b\" ${str} ann_${str}"
    cmd2="mogrify -pointsize 40 -gravity north-west +antialias -font Times-Roman -annotate +34+2 \" = ${str1}\" ann_${str}"
    cmd3="mogrify -pointsize 40 -gravity north-west +antialias -font Times-Roman -annotate +90+5 \"${str2}\" ann_${str}"
    cmd4="mogrify -pointsize 40 -gravity north-west +antialias -font Times-Roman -annotate +170+5 \"${str3}\" ann_${str}"
    echo $cmd1; echo $cmd2; echo $cmd3; echo $cmd4
    eval $cmd1; eval $cmd2; eval $cmd3; eval $cmd4
done

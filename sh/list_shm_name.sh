#!/bin/bash

#ipcs -m | gawk '{if(NR<=3){print $0}else{printf "%c%c%c%c:%s\n", strtonum("0x" substr($1,9,2)), strtonum("0x" substr($1,7,2)), strtonum("0x" substr($1,5,2)), strtonum("0x" substr($1,3,2)), $0}}'
ipcs -m | perl -alne 'if($.<=3){print "$_";}else{printf("%-s: %s\n",chr(hex(substr(@F[0],8,2))).chr(hex(substr(@F[0],6,2))).chr(hex(substr(@F[0],4,2))).chr(hex(substr(@F[0],2,2))),$_);}'

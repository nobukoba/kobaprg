#!/bin/bash

#ipcs -m | gawk '{if(NR<=3){print $0}else{printf "%c%c%c%c:%s\n", strtonum("0x" substr($1,9,2)), strtonum("0x" substr($1,7,2)), strtonum("0x" substr($1,5,2)), strtonum("0x" substr($1,3,2)), $0}}'
ipcs -m | perl -alne 'if($.<=3){print "      $_";}else{if(@F[0] eq "0x00000000"){@F[0]="";}printf("%-4s: %s\n",pack("H*", substr(@F[0],8,2).substr(@F[0],6,2).substr(@F[0],4,2).substr(@F[0],2,2)),$_);}'

#ipcs -m | perl -alne 'if($.<=3){print "$_";}else{$str=chr(hex(substr(@F[0],8,2))).chr(hex(substr(@F[0],6,2))).chr(hex(substr(@F[0],4,2))).chr(hex(substr(@F[0],2,2)));printf("%-4s: %s\n",$str,$_);}'

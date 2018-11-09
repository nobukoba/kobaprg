#!/bin/bash

ipcs -m | awk '{if(NR<=3){print $0}else{printf "%c%c%c%c:%s\n", strtonum("0x" substr($1,9,2)), strtonum("0x" substr($1,7,2)), strtonum("0x" substr($1,5,2)), strtonum("0x" substr($1,3,2)), $0}}'

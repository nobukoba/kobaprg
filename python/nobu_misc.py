#!/usr/bin/env python

import sys
import os.path
import re

args = sys.argv

if len(args) == 1:
    print("No argument. A file name is required.")
    exit()

if not os.path.isfile(args[1]):
    print("No such file: " + args[1])
    exit()

with open(args[1]) as file:
    for row in file:
        match_obj = re.search( "\{", row)
        if match_obj:
            print("    matched: " + row),
        else:
            print("not matched: " + row),
            
            
#f = open('nobu_misc.py','r')
#
#for row in f:
#    print row.strip()
#
#f.close()
#
#for line in sys.stdin:
#   print("--> " + line),

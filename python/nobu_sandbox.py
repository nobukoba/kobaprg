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

lines = []

with open(args[1]) as file:
    for row in file:
        if 
        match_obj_0 = re.search( "\{", row)
        match_obj_1 = re.search( "\{", row)
        if match_obj0:
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

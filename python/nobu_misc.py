#!/usr/bin/env python

import sys
import os.path

args = sys.argv

if len(args) == 1:
    print("No argument.")
    exit()

if not os.path.isfile(args[1]):
    print("No such a file: " + args[1])
    exit()


#f = open('nobu_misc.py','r')
#
#for row in f:
#    print row.strip()
#
#f.close()
#
#for line in sys.stdin:
#   print("--> " + line),

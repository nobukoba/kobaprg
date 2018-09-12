#!/usr/bin/env python

import sys
import os.path
import re
from collections import OrderedDict
args = sys.argv

if len(args) == 1:
    print("No argument. A file name is required.")
    exit()
if not os.path.isfile(args[1]):
    print("No such file: " + args[1])
    exit()

pars = OrderedDict()
bracket_flag = 0
first_key_name = ""
with open(args[1]) as file:
    for row in file:
        if row.strip() == "":
            continue
        match_comment = re.match("^//",row.lstrip())
        if match_comment:
            continue
        strs = re.split("\s+",row)
        print(strs[0] + "{")
        print("  Address: " + strs[1])
        print("  EnergyCoeff: " + strs[2] + " " + strs[3])
        print("  PoleZero: " + strs[4])
        print("  System: " + strs[5])
        print("}")
        print("")

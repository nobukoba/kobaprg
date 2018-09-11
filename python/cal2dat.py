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
        match_obj_0 = re.search( "\{", row)
        match_obj_1 = re.search( "\}", row)
        if match_obj_0 and first_key_name == "":
            strs = re.split("\{",row.strip())
            first_key_name = strs[0]
            pars[first_key_name] = OrderedDict()
        elif match_obj_1 and first_key_name:
            strs = re.split("\}",row)
            first_key_name = ""
        else:
            strs1 = re.split(":",row)
            pars[first_key_name][strs1[0]] = OrderedDict()
            strs2 = re.split("\s+",strs1[1].lstrip().rstrip())
            pars[first_key_name][strs1[0]] = strs2

for first_key in pars:
    print(first_key),
    for second_key in pars[first_key]:
#        print(second_key)
        for par in pars[first_key][second_key]:
            print("" + par),
    print("")

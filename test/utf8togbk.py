#!/usr/bin/env python
#coding=gbk
# ==============================================================================
#          \file   utf8togbk.py
#        \author   chenghuige  
#          \date   2015-03-13 19:53:33.858613
#   \Description  
# ==============================================================================

import sys,os
import gezi 

for line in open(sys.argv[1]):
	print gezi.to_gbk(line),

 

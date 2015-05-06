#!/usr/bin/env python
#coding=gbk
# ==============================================================================
#          \file   deal.py
#        \author   chenghuige  
#          \date   2015-02-04 11:05:15.811653
#   \Description  
# ==============================================================================

import sys,os
import gezi

for line in open(sys.argv[1]):
	l = line.split('\t')
	title = l[-2]
	content = l[-1]
	content = gezi.normalize_str(gezi.strip_html(content))
	content = content.replace('#', '')
	if len(content) > 0:
		print title,'\n',content
	else:
		print title

/***************************************************************************
 * 
 * Copyright (c) 2013 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/



/**
 * @file include/rstree_def.h
 * @author weizheng(com@baidu.com)
 * @date 2013/06/13 16:47:23
 * @brief 
 *  
 **/




#ifndef  __RSTREE_DEF_H_
#define  __RSTREE_DEF_H_

#include <string>
#include <vector>
using namespace std;
const int MC_PACK_BUF_SIZE = 100000;
#define RSTREE_CONF_DIR "./conf"
#define RSTREE_CONF "rstree_strategy.conf"
#define LOCALE "zh_CN.GB18030"

enum SEG_POS
{
  SINGLE,
  LEFT,
  MIDDLE,
  RIGHT
};


typedef pair<string, int> Pair;
typedef pair<wstring, int> WPair;

struct INode
{

  INode()
  {
  }
  
  INode(string str_, int count_)
  :str(str_), count(count_)
  {
    
  }

  INode(string _str, wstring _wstr, int _count)
  : str(_str), wstr(_wstr), count(_count)
  {

  }
  string str;
  wstring wstr;
  int count;
};

struct ONode : public INode
{

  ONode()
  : black_count(0)
  {

  }
  ONode(string str_, int count_)
  :INode(str_, count_)
  {
    
  }

  ONode(string _str, wstring _wstr, int _count)
  : INode(_str, _wstr, _count), black_count(0)
  {

  }
  string filtered_str; // ¹ýÂËºóµÄ×Ö´®
  int black_count;
};


#endif  //__INCLUDE/RSTREE_DEF_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */

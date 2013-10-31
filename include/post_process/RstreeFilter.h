/**
 *  ==============================================================================
 *
 *          \file   RstreeFilter.h
 *
 *        \author   chenghuige
 *
 *          \date   2013-10-31 14:28:27.140145
 *
 *  \Description:
 *  ==============================================================================
 */

#ifndef RSTREE_FILTER_H_
#define RSTREE_FILTER_H_

#include "DSuffixTree.h"
#include <map>
using namespace std;
class RstreeFilter{
public:
  RstreeFilter()
  {
    
  }
  
  //对重复串进行过滤
  vector<pair<string, int> > filter(vector<pair<string, int> >& ivec, const int max_num)
  {
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    return std::move(ivec);
#else
    return ivec;
#endif
  }
  
  virtual ~RstreeFilter()
  {
    
  }
private:
  int max_num_;
};

#endif  //----end of RSTREE_FILTER_H_

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

#include <string>
#include <map>
#include <set>
#include <glog/logging.h>

#include "DSuffixTree.h"
#include "common_util.h"
#include "conf_util.h"
#include "hashmap_util.h"
#include "debug_util.h"
#include "ul_dictmatch.h"
#include "log_util.h"
using namespace std;
using namespace gezi;
//TODO generic Filter

class RstreeFilter
{
public:

  RstreeFilter()
  :distinct_ratio_(0.4), dict_(NULL)
  {
      
  }

  virtual ~RstreeFilter()
  {
    if (dict_)
    {
      dm_dict_del(dict_);
    }
  }
  
  void init()
  {
    comcfg::Configure conf = SharedConf::get_conf();
    string section = "RstreeFilter";
    CONF(distinct_ratio_);
    string white_file = "./data/white.dm";
    CONF(white_file);
    
//    dm_dict_load(const char* fullpath, int lemma_num);
  }

  bool is_white_format(const string& input)
  {
    if (input.empty())
    {
      return true;
    }
    int uniq_size = distinct_count(input.begin(), input.end());
    double ratio = (double)uniq_size / input.size();
//    LOG_DEBUG("ratio: %f", ratio);
//    
//    std::tr1::unordered_map<char, int> hashmap;
//    for (int i = 0; i < (int)input.size(); i++)
//    {
//      hashmap[input[i]] += 1;
//    }
//
//    int len = input.size();
//    double mi = 0.0;
//    for(auto &item : hashmap)
//    {
//      double pr = item.second / (double)len;
//      mi += -log(pr) * pr;
//    }
//    
//    Pval(mi);
//    double pr = 1.0 / len;
//    double max_mi = len *(-log(pr) * pr);
//
//    Pval(max_mi);
//    
//    Pval((mi / max_mi));

    if (ratio < distinct_ratio_)
    {
      return true;
    }
    return false;
  }

  bool contains_white_phrase(const string& input)
  {
    return false;
  }
  
  bool is_pass(const string& input)
  {
    if (is_white_format(input))
    {
      LOG_DEBUG("Is white format: %s", input.c_str());
      return false;
    }
    if (contains_white_phrase(input))
    {
      LOG_DEBUG("Contains white phrase: %s", input.c_str());
      return false;
    }
    return true;
  }

  //对重复串进行过滤

  vector<pair<string, int> > filter(const vector<pair<string, int> >& ivec)
  {
    vector<pair<string, int> > ret;
    for (int i = 0; i < (int) ivec.size(); i++)
    {
      if (is_pass(ivec[i].first))
      {
        ret.push_back(ivec[i]);
      }
    }

#ifdef __GXX_EXPERIMENTAL_CXX0X__
    return std::move(ret);
#else
    return ret;
#endif
  }

  vector<string> filter(const vector<string>& ivec)
  {
    vector<string> ret;
    for (int i = 0; i < (int) ivec.size(); i++)
    {
      if (is_pass(ivec[i]))
      {
        ret.push_back(ivec[i]);
      }
    }
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    return std::move(ret);
#else
    return ret;
#endif
  }

private:
  double distinct_ratio_;
  dm_dict_t* dict_;
};

#endif  //----end of RSTREE_FILTER_H_

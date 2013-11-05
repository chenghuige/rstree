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

#include "common_util.h"
#include "conf_util.h"
#include "hashmap_util.h"
#include "debug_util.h"
#include "log_util.h"
#include "MatchDict.h"
#include "RegexSearcher.h"
#include "rstree_def.h"
using namespace std;
using namespace gezi;
//TODO generic Filter
//对重复串进行过滤

class RstreeFilter
{
public:

  RstreeFilter()
  : distinct_ratio_(0.4)
  {

  }

  virtual ~RstreeFilter()
  {
  }

  void init()
  {
    comcfg::Configure& conf = SharedConf::get_conf();
    string section = "RstreeFilter";
    CONF(distinct_ratio_);
    {
      string white_file = "./data/white.dm";
      CONF(white_file);
      white_dict_.init(white_file);
    }
    {
      string white_pattern_file = "./data/white.pattern.txt";
      CONF(white_pattern_file);
      white_reg_searcher_.init2(white_pattern_file);
    }
  }

  bool is_white_format(const string& input)
  {
    if (input.empty())
    {
      return true;
    }
    int uniq_size = distinct_count(input.begin(), input.end());
    double ratio = (double) uniq_size / input.size();
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
    return white_dict_.has_word(input);
  }

  template<typename String>
  bool contains_white_pattern(const String& input)
  {
    return white_reg_searcher_.has_match(input);
  }

  bool is_pass(const string& input)
  {
    if (is_white_format(input))
    {
      DLOG(INFO) << "Is white format: " << input;
      return false;
    }
    if (contains_white_phrase(input))
    {
      DLOG(INFO) << "Contains white phrase: " << input;
      return false;
    }
    if (contains_white_pattern(input))
    {
      DLOG(INFO) << "Contains white pattern: " << input;
      return false;
    }
    return true;
  }

  bool is_pass(const Node& node)
  {
    if (is_white_format(node.str))
    {
      DLOG(INFO) << "Is white format: " << node.str;
      return false;
    }
    if (contains_white_phrase(node.filtered_str))
    {
      DLOG(INFO) << "Contains white phrase: " << node.filtered_str;
      return false;
    }
    if (contains_white_pattern(node.wstr))
    {
      DLOG(INFO) << "Contains white pattern: " << node.str;
      return false;
    }
    return true;
  }

  //对重复串进行过滤

  vector<Node > process(const vector<Node >& ivec)
  {
    vector<Node > ret;
    //for (auto &item : ivec)
    for(int i = 0; i < (int)ivec.size(); i++)
    {
      const Node& item = ivec[i];
      if (is_pass(item))
      {
        ret.push_back(item);
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
  static gezi::MatchDict white_dict_;
  RegexSearcher white_reg_searcher_;
};

#endif  //----end of RSTREE_FILTER_H_

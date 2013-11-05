/**
 *  ==============================================================================
 *
 *          \file   PostProcessor.h
 *
 *        \author   chenghuige
 *
 *          \date   2013-10-31 21:28:41.545779
 *
 *  \Description:  后处理包括
 *  TODO  在DSuffixTree模块增加分词(utf8分词)判断，即对于文本同时存储一个是否分词边界位判断 这样抽出的串符合分词 避免截断 配置开关
 *  另外的策略是不受分词影响 那取决与检索系统 可以对我的前后两个词做过滤 
 *  另外很长的未被截断的话 一般是水串
 * 
 *  或者最后只对串分词
 * 
 *  1. 文本修整
 *     前后空格处理 trim  如果之后长度不符合过滤  //输入前已经处理
 *  2. 过滤 
 *     2.1 特定格式的串豁免 比如重复度高的  啊阿阿阿啊  在一起在一起在一起
 *         要先过滤掉其它所有字符 只要中文？
 *     2.2 白词豁免 匹配到白库里面的词 那么过滤掉 不处理  比如 十五字 混经验 乔粱宇 大水怪
 *  3. 排序
 *     TODO 按照串文本打分 排序 -> 按照长度排序 -> 按照freq排序
 *     现在
 *     判断串是否包含嫌疑词 比如 淘宝 兼职 头像 美容 双眼皮 交流群
 *     按照嫌疑词命中数目多少排序  -> 按照长度排序(或者分词后的term数目) -> 按照freq排序
 * 
 *     TODO 嫌疑模式 比如 到××吧 
 *     另外考虑串的相似度  比如   ABCDEFG    BCDEFGH 选取一个就可以了 选黑词多的  TODO
 *     分别去头尾部  判断是否一样 反过来 再判断一次
 * 
 * 问题 多个相同文本 每个都要再走这个流程 速度
 * 另外rsc模块 打分可能还要再单独使用更多文本特征 而不是文本打分作为一个单独特征 重复计算
 * 是否都在rsc模块进行后处理？ TODO
 * 合并并且采用openmpi ?
 *  ==============================================================================
 */

#ifndef POST_PROCESSOR_H_
#define POST_PROCESSOR_H_

#include "RstreeFilter.h"
#include "common_util.h"
#include "conf_util.h"
#include "debug_util.h"
#include "string_util.h"
#include <glog/logging.h>
#include "MatchDict.h"
#include "RegexSearcher.h"
#include <functional>
#include <boost/bind.hpp>
#include "rstree_def.h"

class PostProcessor
{
public:

  PostProcessor()
  : max_match_count_(1000)
  {

  }

  virtual ~PostProcessor()
  {
    if (result_)
    {
      dm_pack_del(result_);
    }
  }

  bool init()
  {
    return init(SharedConf::conf());
  }

  //如果是同一个类 多个线程只读 如果内存占用不大 就是成员变量 如果内存占用大 就用static init_static,如果是多个类都需要访问的
  //用指针 或者 全局 或者 单例子
  static bool init_static()
  {
    return init_static(SharedConf::conf());
  }
  //进程加载公共资源  TODO多线程环境需要2个init分开 init 线程级别 init_dict(or init_static()) 进程级别
  static bool init_static(comcfg::Configure& conf, string section = "PostProcessor")
  {
    string black_file = "./data/black.dm";
    CONF(black_file);
    black_dict_.init(black_file);
  }

  bool init(comcfg::Configure& conf, string section = "PostProcessor")
  {
    {
      CONF(max_match_count_);
      result_ = dm_pack_create(max_match_count_);
      CHECK_NOTNULL(result_);
    }
    {
      string black_pattern_file = "./data/black.pattern.txt";
      CONF(black_pattern_file);
      black_reg_searcher_.init2(black_pattern_file);
    }
    {
      white_filter_.init();
    }

    return true;
  }

  string filter(string& input)
  {
    boost::to_lower(input);
    return gezi::filter_str(input);
  }

  int black_process(const vector<INode>& ivec, vector<Node>& ovec)
  {
    int black_count = 0;
    //for (auto &item : ivec)
    for(int i = 0; i < (int)ivec.size(); i++)
    {
      const INode& item = ivec[i];
      Node node(item.str, item.wstr, item.count);
      node.filtered_str = filter(node.str);
      //--------------------黑词匹配
      Pval(node.str);
      Pval(node.filtered_str);
      node.black_count = black_dict_.search_count(node.filtered_str, result_);
      Pval(node.black_count);

      //--------------------黑模版匹配
      if (black_reg_searcher_.has_match(node.wstr))
      {
        node.black_count++;
      }

      if (node.black_count > 0)
      {
        black_count++;
      }

      Pval(node.black_count);
      ovec.push_back(node);
    }
    return black_count;
  }

  void process(const vector<INode>& ivec, int max_count, vector<Node>& vec)
  {
    int black_count = black_process(ivec, vec);

    if (black_count < max_count)
    {
      vec = white_filter_.process(vec);
    }

    if (vec.size() > max_count)
    {
      std::partial_sort(vec.begin(), vec.begin() + max_count, vec.end(),
              boost::bind(&Node::black_count, _1) >
              boost::bind(&Node::black_count, _2));
      vec.resize(max_count);
    }
    else
    {
      std::sort(vec.begin(), vec.end(),
              boost::bind(&Node::black_count, _1) >
              boost::bind(&Node::black_count, _2));
    }

  }

  //for test

  void process(const vector<Pair>& ivec, int max_count, vector<Node>& vec)
  {
    vector<INode> rvec;
//    for (auto &item : ivec)
    for(int i = 0; i < (int)ivec.size(); i++)
    {
      const Pair& item = ivec[i];
      rvec.push_back(INode(item.first, str_to_wstr(item.first), item.second));
    }
    process(rvec, max_count, vec);
  }

  vector<Pair> process(const vector<INode>& ivec, int max_count)
  {
    vector<Pair> ret;

    vector<Node> vec;
    process(ivec, max_count, vec);
    //for (auto &item : vec)
    for(int i = 0; i < (int)vec.size(); i++)
    {
      Node& item = vec[i];
      ret.push_back(make_pair(item.str, item.count));
    }
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    return std::move(ret);
#else
    return ret;
#endif
  }
private:
  //--------------------------黑名单匹配 
  static MatchDict black_dict_;
  dm_pack_t* result_;
  int max_match_count_; //最多可能匹配的个数
  //--------------------------黑模式匹配
  RegexSearcher black_reg_searcher_;
  //--------------------------黑未匹配的情况下 匹配白名单或者白模式豁免过滤
  RstreeFilter white_filter_;
};


#endif  //----end of POST_PROCESSOR_H_

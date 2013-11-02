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
#include "conf_util.h"
#include "ul_dictmatch.h"
#include <glog/logging.h>
#include "MatchDict.h"

class PostProcessor
{
public:

  PostProcessor()
  {

  }

  virtual ~PostProcessor()
  {
  }

  bool init()
  {
    comcfg::Configure& conf = SharedConf::get_conf();
    return init(conf);
  }

  bool init(comcfg::Configure& conf, string section = "PostProcessor")
  {
    string black_file = "./data/black.dm";
    CONF(black_file);

    dict_.init(black_file);
    return true;
  }

  //对重复串进行过滤

  struct Node
  {

    Node()
    : black_count(0)
    {

    }
    string substr;
    int count;
    int black_count;
  };

  void process(const vector<pair<string, int> >& ivec, vector<Node >& ovec)
  {
    dm_pack_t* result = dm_pack_create(10);
    for (auto &item : ivec)
    {
      Node node;
      node.substr = item.first;
      node.count = item.second;
      int ret = dict_.search(node.substr, result);
      CHECK_EQ(ret, 0);
      node.black_count = result->ppseg_cnt;

      Pval(node.substr);
      Pval(node.black_count);
    }

  }

  vector<pair<string, int> > process(const vector<pair<string, int> >& ivec)
  {
    vector<pair<string, int> > ret;

    vector<Node> vec;
    process(ivec, vec);
#ifdef __GXX_EXPERIMENTAL_CXX0X__
            return std::move(ret);
#else
            return ret;
#endif
  }
private:
  static MatchDict dict_;
};


#endif  //----end of POST_PROCESSOR_H_

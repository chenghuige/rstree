/**
 *  ==============================================================================
 *
 *          \file   PostProcessor.h
 *
 *        \author   chenghuige
 *
 *          \date   2013-10-31 21:28:41.545779
 *
 *  \Description:  �������
 *  TODO  ��DSuffixTreeģ�����ӷִ�(utf8�ִ�)�жϣ��������ı�ͬʱ�洢һ���Ƿ�ִʱ߽�λ�ж� ��������Ĵ����Ϸִ� ����ض� ���ÿ���
 *  ����Ĳ����ǲ��ִܷ�Ӱ�� ��ȡ�������ϵͳ ���Զ��ҵ�ǰ�������������� 
 *  ����ܳ���δ���ضϵĻ� һ����ˮ��
 * 
 *  �������ֻ�Դ��ִ�
 * 
 *  1. �ı�����
 *     ǰ��ո��� trim  ���֮�󳤶Ȳ����Ϲ���  //����ǰ�Ѿ�����
 *  2. ���� 
 *     2.1 �ض���ʽ�Ĵ����� �����ظ��ȸߵ�  ����������  ��һ����һ����һ��
 *         Ҫ�ȹ��˵����������ַ� ֻҪ���ģ�
 *     2.2 �״ʻ��� ƥ�䵽�׿�����Ĵ� ��ô���˵� ������  ���� ʮ���� �쾭�� ������ ��ˮ��
 *  3. ����
 *     TODO ���մ��ı���� ���� -> ���ճ������� -> ����freq����
 *     ����
 *     �жϴ��Ƿ�������ɴ� ���� �Ա� ��ְ ͷ�� ���� ˫��Ƥ ����Ⱥ
 *     �������ɴ�������Ŀ��������  -> ���ճ�������(���߷ִʺ��term��Ŀ) -> ����freq����
 * 
 *     TODO ����ģʽ ���� �������� 
 *     ���⿼�Ǵ������ƶ�  ����   ABCDEFG    BCDEFGH ѡȡһ���Ϳ����� ѡ�ڴʶ��  TODO
 *     �ֱ�ȥͷβ��  �ж��Ƿ�һ�� ������ ���ж�һ��
 * 
 * ���� �����ͬ�ı� ÿ����Ҫ����������� �ٶ�
 * ����rscģ�� ��ֿ��ܻ�Ҫ�ٵ���ʹ�ø����ı����� �������ı������Ϊһ���������� �ظ�����
 * �Ƿ���rscģ����к��� TODO
 * �ϲ����Ҳ���openmpi ?
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

  //�����ͬһ���� ����߳�ֻ�� ����ڴ�ռ�ò��� ���ǳ�Ա���� ����ڴ�ռ�ô� ����static init_static,����Ƕ���඼��Ҫ���ʵ�
  //��ָ�� ���� ȫ�� ���� ������
  static bool init_static()
  {
    return init_static(SharedConf::conf());
  }
  //���̼��ع�����Դ  TODO���̻߳�����Ҫ2��init�ֿ� init �̼߳��� init_dict(or init_static()) ���̼���
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
      //--------------------�ڴ�ƥ��
      Pval(node.str);
      Pval(node.filtered_str);
      node.black_count = black_dict_.search_count(node.filtered_str, result_);
      Pval(node.black_count);

      //--------------------��ģ��ƥ��
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
  //--------------------------������ƥ�� 
  static MatchDict black_dict_;
  dm_pack_t* result_;
  int max_match_count_; //������ƥ��ĸ���
  //--------------------------��ģʽƥ��
  RegexSearcher black_reg_searcher_;
  //--------------------------��δƥ�������� ƥ����������߰�ģʽ�������
  RstreeFilter white_filter_;
};


#endif  //----end of POST_PROCESSOR_H_

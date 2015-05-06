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
//#include <boost/bind.hpp>
#include "rstree_def.h"

class PostProcessor
{
public:

  PostProcessor()
  : _max_match_count(1000)
  {

  }

  virtual ~PostProcessor()
  {
    if (_result)
    {
      dm_pack_del(_result);
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
    _black_dict.init(black_file);
  }

  bool init(comcfg::Configure& conf, string section = "PostProcessor")
  {
    {
      CONF(_max_match_count);
      _result = dm_pack_create(_max_match_count);
      CHECK_NOTNULL(_result);
    }
    {
      string black_pattern_file = "./data/black.pattern.txt";
      CONF(black_pattern_file);
      _black_reg_searcher.init2(black_pattern_file);
    }
    {
      _white_filter.init();
    }

    return true;
  }

  string filter(string input)
  {
    string s = input;
    boost::to_lower(s);
    return gezi::filter_str(s);
  }

  int black_process(const vector<INode>& ivec, vector<ONode>& ovec)
  {
    int black_count = 0;
    //for (auto &item : ivec)
    for (int i = 0; i < (int) ivec.size(); i++)
    {
      const INode& item = ivec[i];
      ONode node(item.str, item.wstr, item.count);
      node.filtered_str = filter(node.str);
      //--------------------�ڴ�ƥ��
      //      Pval(node.str);
      //      Pval(node.filtered_str);
      node.black_count = _black_dict.search_count(node.filtered_str, _result);
      //      Pval(node.black_count);
      //      if (node.black_count > 0)
      //      {
      //        Pval(node.str);
      //        Pval(_result->ppseg_cnt);
      //        Pval(_result->poff[0]);
      //        Pval(_result->ppseg[0]->len);
      //        Pval(_result->ppseg[0]->pstr);
      //      }

      //--------------------��ģ��ƥ��
      if (_black_reg_searcher.has_match(node.wstr))
      {
        node.black_count++;
      }

      if (node.black_count > 0)
      {
        black_count++;
      }

      //      Pval(node.black_count);
      ovec.push_back(node);
    }
    return black_count;
  }

  struct Cmp
  {

    bool operator() (const ONode& left, const ONode& right)
    {
      if (left.black_count == right.black_count)
      {
        if (left.str.length() >= MIN_GOOD_LENGTH && right.str.length() >= MIN_GOOD_LENGTH)
        {
          if (left.count == right.count)
          {
            return left.str.length() > right.str.length();
          }
          else
          {
            return left.count > right.count;
          }
        }
        else
        {
          if (left.str.length() == right.str.length())
          {
            return left.count > right.count;
          }
          else
          {
            return left.str.length() > right.str.length();
          }
        }
      }
      else
      {
        return left.black_count > right.black_count;
      }
    }
  };

  void process(const vector<INode>& ivec, vector<ONode>& vec, int max_count)
  {
    int black_count = black_process(ivec, vec);

    if (black_count < max_count)
    {
      vec = _white_filter.process(vec);
    }

    PrintVec(vec, str);
    if (vec.size() > max_count)
    {
      std::partial_sort(vec.begin(), vec.begin() + max_count, vec.end(), Cmp());
      vec.resize(max_count);
    }
    else
    {
      std::sort(vec.begin(), vec.end(), Cmp());
    }
  }

  //for test

  void process(const vector<Pair>& ivec, int max_count, vector<ONode>& vec)
  {
    vector<INode> rvec;
    //    for (auto &item : ivec)
    for (int i = 0; i < (int) ivec.size(); i++)
    {
      const Pair& item = ivec[i];
      rvec.push_back(INode(item.first, str_to_wstr(item.first), item.second));
    }
    process(rvec, vec, max_count);
  }

  //��ȡ��һ��spam

  string get_spam(string input)
  {
    int count = _black_dict.search_count(input, _result);
    if (count > 0)
    {
      return _result->ppseg[0]->pstr;
    }

    return "";
  }

  ////  //��ȡ�������spam�������Ӧ��string
  // string get_spampart(string input, int max_length)
  // {
  //   
  // }

  static gezi::MatchDict& black_dict()
  {
    return _black_dict;
  }
private:
  //--------------------------������ƥ�� 
  static gezi::MatchDict _black_dict;
  dm_pack_t* _result;
  int _max_match_count; //������ƥ��ĸ���
  //--------------------------��ģʽƥ��
  RegexSearcher _black_reg_searcher;
  //--------------------------��δƥ�������� ƥ����������߰�ģʽ�������
  RstreeFilter _white_filter;
  static const int MIN_GOOD_LENGTH = 20;
};


#endif  //----end of POST_PROCESSOR_H_

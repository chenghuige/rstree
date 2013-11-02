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

  //���ظ������й���

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

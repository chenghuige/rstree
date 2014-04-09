/** 
 *  ==============================================================================
 * 
 *          \file   dsuffix_tree.h
 *
 *        \author   chenghuige  
 *          
 *          \date   2013-08-20 22:03:00.758982
 *  
 *  \Description:  ��suffix tree�������ṩ��ѯ���������Ĵ�
 *                1. Ƶ������ 2. �������� �������̣�
 *                3. ��ǰ2��������а�����ϵ����δ���
 *                   3.1 ֻ������   ����1
 *                   3.2 ���Ƶ����ͬ ֻ������ ����Ҫ   ����2
 *                 ����ֻ�����������ƣ��������Ʊ��� ���պ��������ˣ���Ŀ���ƣ�����ѡ�ţ� ����ģ�鴦��
 * 
 * TODO ���Ӷ���һ���ı� ģ����� ������count �����������Ĵ�
 * 
 * ע��ﵽƵ��Ҫ�� ���ǳ���������� ֻȡһ��
 * ���� abcdefghigk ����100�� ����Ҫ��<=5 ��ֻѡһ�� abcde����
 *  ==============================================================================
 */

#ifndef DSUFFIX_TREE_H_
#define DSUFFIX_TREE_H_

#include <algorithm>
#include "SuffixTree.h"
#include "rstree_def.h"
#include "hashmap_util.h"
#include "log_util.h"
#include "conf_util.h"

//Generalized Suffix tree with dynamic count support
namespace gezi
{

class DSuffixTree : public SuffixTree
{
public:

  enum STRATEGY
  {
    FREQ_FIRST, //Ƶ������ �ο�ͷע�� ����1
    LENGTH_FIRST //��������
  };

  int min_substr_len_; //�ظ��Ӵ���̳���
  int max_substr_len_; //�ظ��Ӵ���󳤶�
  int min_frequency_; //��������Ƶ������
  int max_tree_size_; //���Ĵ�С����

  int strategy_;

  DSuffixTree()
  : SuffixTree(L"\n")
  {
    min_substr_len_ = 8;
    max_substr_len_ = 20;
    min_frequency_ = 15;
    max_tree_size_ = 300000;
    strategy_ = 1;
  }

  bool init()
  {
    return init(SharedConf::get_conf());
  }

  bool init(const comcfg::Configure& conf,
          string section = "DSuffixTree")
  {
    CONF(min_substr_len_);
    CONF(max_substr_len_);
    CONF(min_frequency_);
    CONF(max_tree_size_);
    CONF(strategy_);
    return true;
  }

  bool read_config(const comcfg::Configure& conf,
          string section = "DSuffixTree")
  {
    CONF(min_substr_len_);
    CONF(max_substr_len_);
    CONF(min_frequency_);
    CONF(max_tree_size_);
    CONF(strategy_);
    return true;
  }

  bool read_config(string path = "./conf", string config_file = "DSuffixTree.conf",
          string section = "DSuffixTree")
  {
    return read_config(path.c_str(), config_file.c_str(), section);
  }

  bool init(string path, string config_file,
          string section = "DSuffixTree")
  {
    return read_config(path.c_str(), config_file.c_str(), section);
  }

  bool read_config(const char* path, const char* config_file,
          string section = "DSuffixTree")
  {
    comcfg::Configure conf;
    if (conf.load(path, config_file) != 0)
    {
      LOG_WARNING("dsuffix tree conf path:%s name:%s", path, config_file);
      return false;
    }
    return read_config(conf);
  }

  void set_strategy(int strategy)
  {
    strategy_ = strategy;
  }

  int strategy()
  {
    return strategy_;
  }

  int get_strategy()
  {
    return strategy_;
  }

  void set_min_frequency(int freq)
  {
    min_frequency_ = freq;
  }

  int min_frequency()
  {
    return min_frequency_;
  }

  int get_min_frequency()
  {
    return min_frequency_;
  }

  void set_min_substr_len(int len)
  {
    min_substr_len_ = len;
  }

  int min_substr_len()
  {
    return min_substr_len_;
  }

  int get_min_substr_len()
  {
    return min_substr_len_;
  }

  void set_max_substr_len(int len)
  {
    max_substr_len_ = len;
  }

  int max_substr_len()
  {
    return max_substr_len_;
  }

  int get_max_substr_len()
  {
    return max_substr_len_;
  }

  void set_tree_size(int size)
  {
    max_tree_size_ = size;
  }

  int max_tree_size()
  {
    return max_tree_size_;
  }

  int get_max_tree_size()
  {
    return max_tree_size_;
  }

  typedef std::pair<wstring, int> SubStrFreq; //first is substring, second is freq

  vector<SubStrFreq> add_text(const wstring& text)
  {
    vector<SubStrFreq> result_vec;
    wstring s = text;
    boost::replace_all(s, end_mark_, L"");
    s = s + end_mark_;
    texts_.push_back(s);
    add_(s);
    //if (strategy_ == (int) STRATEGY::FREQ_FIRST)
    if (strategy_ == (int) FREQ_FIRST)
    {
      find_substrs_freqfirst(s, result_vec);
    }
    else
    {
      find_substrs_lengthfirst(s, result_vec);
    }
    current_text_id_++;
    if (tree_size() >= max_tree_size_)
    {
      remove();
    }
#if __GNUC__ > 3
    return std::move(result_vec);
#else
    return result_vec;
#endif
  }

  //  //TODO
  //  void search_text(const wstring& text, vector<SubStrFreq>& result_vec)
  //  {
  //   
  //  }

  bool is_node_ok(Node* node)
  {
    return node->freq >= min_frequency_
            && node->length >= min_substr_len_
            && node->length <= max_substr_len_;
  }

  bool is_node_ok(Node* node, bool& need_up)
  {
    if (node->length <= min_substr_len_)
    {
      need_up = false;
    }

    return node->freq >= min_frequency_
            && node->length >= min_substr_len_
            && node->length <= max_substr_len_;
  }


  //�ο�ע�Ͱ��ղ���2
  //���˵ı��� ����abcdefg���� �������ڳ��� ������û�ж̱��������� ������ ��������������ٷ��� �ڴ�������к����������

  void find_substrs_freqfirst(const wstring& text, vector<SubStrFreq>& result_vec)
  {
    Node* leaf_node;
    std::unordered_map < Node*, bool> internal_nodes_map;
    typedef std::unordered_map < Node*, bool>::iterator Iter;
    for (leaf_node = first_leafs_[current_text_id_ - oldest_text_id_]; leaf_node != NULL;
            leaf_node = leaf_node->suffix_link)
    {
      bool need_up = true;
      for (Node* node = leaf_node->parent; node != root_; node = node->parent)
      {
        if (internal_nodes_map.find(node) == internal_nodes_map.end())
        {
          if (is_node_ok(node, need_up))
          {
            internal_nodes_map[node] = true;
          }
          if (!need_up)
          {
            break;
          }
        }
        else
        {
          break;
        }
      }
    }

    for (Iter iter = internal_nodes_map.begin(); iter != internal_nodes_map.end(); ++iter)
    {
      Node* node = iter->first;
      Node* shorter_node = node->suffix_link;

      if (shorter_node != root_ && node->freq == shorter_node->freq)
      {
        Iter iter = internal_nodes_map.find(shorter_node);
        if (iter != internal_nodes_map.end())
        {
          iter->second = false;
        }
      }
    }

    for (Iter iter = internal_nodes_map.begin(); iter != internal_nodes_map.end(); ++iter)
    {
      Node* node = iter->first;
      if (iter->second == true)
      {
        wstring substr = text.substr(node->end - node->length,
                node->length);
        result_vec.push_back(SubStrFreq(substr, node->freq));
      }
    }

  }

  void find_substrs_lengthfirst(const wstring& text, vector<SubStrFreq>& result_vec)
  {
    Node* leaf_node;
    std::unordered_map < Node*, bool> internal_nodes_map;
    typedef std::unordered_map < Node*, bool>::iterator Iter;
    for (leaf_node = first_leafs_[current_text_id_ - oldest_text_id_]; leaf_node != NULL;
            leaf_node = leaf_node->suffix_link)
    {
      for (Node* node = leaf_node->parent; node != root_; node = node->parent)
      {
        if (internal_nodes_map.find(node) == internal_nodes_map.end())
        {
          if (is_node_ok(node))
          {
            internal_nodes_map[node] = true;
            break;
          }
        }
        else
        {
          break;
        }
      }
    }

    for (Iter iter = internal_nodes_map.begin(); iter != internal_nodes_map.end(); ++iter)
    {
      Node* node = iter->first;
      Node* shorter_node = node->suffix_link;

      if (shorter_node != root_)
      {
        Iter iter = internal_nodes_map.find(shorter_node);
        if (iter != internal_nodes_map.end())
        {
          iter->second = false;
        }
      }
    }

    for (Iter iter = internal_nodes_map.begin(); iter != internal_nodes_map.end(); ++iter)
    {
      Node* node = iter->first;
      if (iter->second == true)
      {
        wstring substr = text.substr(node->end - node->length,
                node->length);
        result_vec.push_back(SubStrFreq(substr, node->freq));
      }
    }
  }

};
}

#endif  //__DSUFFIX_TREE_H_

/**
 *  ==============================================================================
 *
 *          \file   Rstree.h
 *
 *        \author   chenghuige
 *
 *          \date   2013-11-04 13:07:50.306547
 *
 *  \Description:
 *  ==============================================================================
 */

#ifndef RSTREE_H_
#define RSTREE_H_
#include "SuffixTree.h"
#include "debug_util.h"
namespace gezi
{

class Rstree : public SuffixTree
{
public:
  int min_substr_len_; //重复子串最短长度
  int max_substr_len_; //重复子串最大长度
  int min_frequency_; //满足的最低频次限制
  int max_tree_size_; //树的大小限制

  Rstree(const wstring& end_mark = L"\n")
  : SuffixTree(end_mark)
  {
    min_substr_len_ = 8;
    max_substr_len_ = 20;
    min_frequency_ = 15;
    max_tree_size_ = 300000;
  }

  bool init()
  {
    return init(SharedConf::get_conf());
  }

  bool init(const comcfg::Configure& conf,
          const string& section = "Rstree")
  {
    CONF(min_substr_len_);
    CONF(max_substr_len_);
    CONF(min_frequency_);
    CONF(max_tree_size_);
    return true;
  }

  bool read_config(const comcfg::Configure& conf,
          const string& section = "Rstree")
  {
    CONF(min_substr_len_);
    CONF(max_substr_len_);
    CONF(min_frequency_);
    CONF(max_tree_size_);
    return true;
  }

  bool read_config(const string& path = "./conf", const string& config_file = "Rstree.conf",
          const string& section = "Rstree")
  {
    return read_config(path.c_str(), config_file.c_str(), section);
  }

  bool init(const string& path, const string& config_file,
          const string& section = "Rstree")
  {
    return read_config(path.c_str(), config_file.c_str(), section);
  }

  bool read_config(const char* path, const char* config_file,
          const string& section = "Rstree")
  {
    comcfg::Configure conf;
    if (conf.load(path, config_file) != 0)
    {
      LOG_WARNING("dsuffix tree conf path:%s name:%s", path, config_file);
      return false;
    }
    return read_config(conf);
  }

  inline void set_min_frequency(int freq)
  {
    min_frequency_ = freq;
  }

  inline int min_frequency()
  {
    return min_frequency_;
  }

  inline void set_min_substr_len(int len)
  {
    min_substr_len_ = len;
  }

  inline int min_substr_len()
  {
    return min_substr_len_;
  }

  inline void set_max_substr_len(int len)
  {
    max_substr_len_ = len;
  }

  inline int max_substr_len()
  {
    return max_substr_len_;
  }

  void set_tree_size(int size)
  {
    max_tree_size_ = size;
  }

  inline int max_tree_size()
  {
    return max_tree_size_;
  }

  typedef std::pair<wstring, int> Pair; //first is substring, second is freq

  vector<Pair> add(const wstring& text, const vector<int>* pos_types = NULL)
  {
    vector<Pair> result_vec;
    wstring s = text;
//    boost::trim(s);
    boost::replace_all(s, end_mark_, L"");
    s = s + end_mark_;
    
    wcout << s << endl;
    
    texts_.push_back(s);
    add_(s);
    find_substrs(s, result_vec, pos_types);
    current_text_id_++;
    if (tree_size() >= max_tree_size_)
    {
      remove();
    }
    
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    return std::move(result_vec);
#else
    return result_vec;
#endif
  }


  //TODO 提供search功能 查询给定文本在后缀树中有哪些满足长度和频次要求的子串,不影响当前后缀树
  //可以按照加入 然后删除最新文本 进行， 其它方法？ 如果提供search 可否多线程查询？ 
  //  vector<Pair> search(const wstring& text)
  //  {
  //  
  //  }

  bool is_node_ok(const Node* node)
  {
    return node->freq >= min_frequency_
            && node->length >= min_substr_len_
            && node->length <= max_substr_len_;
  }

  bool is_node_ok(const Node* node, bool& need_up)
  {
    if (node->length <= min_substr_len_)
    {
      need_up = false;
    }

    return node->freq >= min_frequency_
            && node->length >= min_substr_len_
            && node->length <= max_substr_len_;
  }

  inline wstring trim(const wstring& text, const Node* node, const vector<int>* pos_types)
  {
    if (!pos_types)
    {
      return text.substr(node->end - node->length,
              node->length);
    }
    else
    {
      LOG_DEBUG("Need trim with seg");
      int start = node->end - node->length;
      int end = start + node->length - 1;
      const vector<int>& vec = *pos_types;
      while (vec[start] != LEFT && vec[start] != SINGLE)
      {
        start++;
      }
      while (vec[end] != RIGHT && vec[end] != SINGLE)
      {
        end--;
      }
      if (start < end)
      {
        return text.substr(start, end - start);
      }
      else
      {
        LOG_DEBUG("Trim with seg fail");
        return text.substr(node->end - node->length,
              node->length);
      }
    }
  }

  //满足长度 频次要求 情况下 长度优先   长度要求包括最长限制
  //极端情况下 限制长度不超过某个阈值 选取 可能会取不到 比如abcdefghigk...重复n次 但是大语料中概率极小
  //如果不限制最长 最后再截取  可能无法截取到黑词部分的串 综合权衡 采用下面策略

  void find_substrs(const wstring& text, vector<Pair>& result_vec,
          const vector<int>* pos_types = NULL)
  {
    Node* leaf_node;
    std::tr1::unordered_map < Node*, bool> internal_nodes;
    typedef std::tr1::unordered_map < Node*, bool>::iterator Iter;
    for (leaf_node = first_leafs_[current_text_id_ - oldest_text_id_]; leaf_node != NULL;
            leaf_node = leaf_node->suffix_link)
    {
      for (Node* node = leaf_node->parent; node != root_; node = node->parent)
      {
        if (internal_nodes.find(node) == internal_nodes.end())
        {
          if (is_node_ok(node))
          {
            internal_nodes[node] = true;
            break;
          }
        }
        else
        {
          break;
        }
      }
    }

    for (Iter iter = internal_nodes.begin(); iter != internal_nodes.end(); ++iter)
    {
      Node* node = iter->first;
      Node* shorter_node = node->suffix_link;

      if (shorter_node != root_)
      {
        Iter iter = internal_nodes.find(shorter_node);
        if (iter != internal_nodes.end())
        {
          iter->second = false;
        }
      }
    }

    for (Iter iter = internal_nodes.begin(); iter != internal_nodes.end(); ++iter)
    {
      if (iter->second == true)
      {
        Node* node = iter->first;
        wstring substr = trim(text, node, pos_types);
        if (substr.length() >= min_substr_len_)
        {
          result_vec.push_back(Pair(substr, node->freq));
        }
      }
    }
  }
  
};

} //----end of namespace gezi

#endif  //----end of RSTREE_H_

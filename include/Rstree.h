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
#include "rstree_def.h"
#include "debug_util.h"
namespace gezi
{

class Rstree : public SuffixTree
{
public:
  int _min_substr_len; //重复子串最短长度
  int _max_substr_len; //重复子串最大长度
  int _min_frequency; //满足的最低频次限制
  int _max_tree_size; //树的大小限制

  Rstree(const wstring& end_mark = L"\n")
  : SuffixTree(end_mark)
  {
    _min_substr_len = 8;
    _max_substr_len = 20;
    _min_frequency = 15;
    _max_tree_size = 300000;
  }

  bool init()
  {
    return init(SharedConf::get_conf());
  }

  bool init(const comcfg::Configure& conf,
          const string& section = "Rstree")
  {
    CONF(_min_substr_len);
    CONF(_max_substr_len);
    CONF(_min_frequency);
    CONF(_max_tree_size);
    return true;
  }

  bool read_config(const comcfg::Configure& conf,
          const string& section = "Rstree")
  {
    CONF(_min_substr_len);
    CONF(_max_substr_len);
    CONF(_min_frequency);
    CONF(_max_tree_size);
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
    _min_frequency = freq;
  }

  inline int min_frequency()
  {
    return _min_frequency;
  }

  inline void set_min_substr_len(int len)
  {
    _min_substr_len = len;
  }

  inline int min_substr_len()
  {
    return _min_substr_len;
  }

  inline void set_max_substr_len(int len)
  {
    _max_substr_len = len;
  }

  inline int max_substr_len()
  {
    return _max_substr_len;
  }

  void set_tree_size(int size)
  {
    _max_tree_size = size;
  }

  inline int max_tree_size()
  {
    return _max_tree_size;
  }

  typedef std::pair<wstring, int> Pair; //first is substring, second is freq

  void add(const wstring& text, vector<Pair>& result_vec, const vector<int>* pos_types = NULL)
  {
    wstring s = text;
    s = s + _end_mark;
    _texts.push_back(s);
    add_(s);
    find_substrs(s, result_vec, pos_types);
    _current_text_id++;
    if (tree_size() >= _max_tree_size)
    {
      remove();
    }
  }

  vector<Pair> add(const wstring& text, const vector<int>* pos_types = NULL)
  {
    vector<Pair> result_vec;
    wstring s = text;
    boost::trim(s);
    //    boost::replace_all(s, _end_mark, L"");  //转string之前已经去掉换行
    s = s + _end_mark;
    _texts.push_back(s);
    add_(s);
    find_substrs(s, result_vec, pos_types);
    _current_text_id++;
    if (tree_size() >= _max_tree_size)
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
    if (node->next == NULL)
    { //叶子节点
      if (node->end - node->start == 1)
      {//只有末尾填充字符的情况
        return false;
      }
      return node->freq >= _min_frequency
              && node->length >= _min_substr_len + 1
              && node->length <= _max_substr_len + 1;
    }
    else
    {
      return node->freq >= _min_frequency
              && node->length >= _min_substr_len
              && node->length <= _max_substr_len;
    }
  }

  bool is_node_ok(const Node* node, bool& need_up)
  {
    if (node->next == NULL)
    {

      if (node->length <= _min_substr_len + 1)
      {
        need_up = false;
      }

      if (node->end - node->start == 1)
      {//只有末尾填充字符的情况
        return false;
      }

      return node->freq >= _min_frequency
              && node->length >= _min_substr_len + 1
              && node->length <= _max_substr_len + 1;
    }
    else
    {
      if (node->length <= _min_substr_len)
      {
        need_up = false;
      }
      return node->freq >= _min_frequency
              && node->length >= _min_substr_len
              && node->length <= _max_substr_len;
    }
  }

  inline wstring trim(const wstring& text, const vector<int>& splits, int start, int end)
  {
    if (start == 0)
    {
      for (int i = 0; start < end && splits[start] == SINGLE && i < 3; i++)
      {
        start++;
      }
    }
    while (start < end && splits[start] != LEFT && splits[start] != SINGLE)
    {
      start++;
    }
    if (end == (text.length() - 1))
    {
      for (int i = 0; start < end && splits[end - 1] == SINGLE && i < 3; i++)
      {
        end--;
      }
    }
    while (start < end && splits[end - 1] != RIGHT && splits[end - 1] != SINGLE)
    {
      end--;
    }
    if (start < end)
    {
      return text.substr(start, end - start);
    }
    else
    {
      return L"";
    }
  }

  inline wstring trim(const Node* node, const wstring& text, const vector<int>* pos_types = NULL)
  {
    if (pos_types)
    {
      int start = node->end - node->length;
      int end = node->next == NULL ? node->end - 1 : node->end;
      const vector<int>& vec = *pos_types;
      wstring ret = trim(text, vec, start, end);
      if (ret.empty())
      {
        LOG_WARNING("Trim with seg fail");
        if (node->next == NULL)
        {
          return text.substr(node->end - node->length,
                  node->length - 1);
        }
        else
        {
          return text.substr(node->end - node->length,
                  node->length);
        }
      }
      return ret;
    }
    else
    {
      if (node->next == NULL)
      {
        return text.substr(node->end - node->length,
                node->length - 1);
      }
      else
      {
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

    int idx = _current_text_id - _oldest_text_id;
    for (leaf_node = _first_leafs[idx]; leaf_node != NULL; leaf_node = leaf_node->suffix_link)
    {
      bool need_up = true;
      for (Node* node = leaf_node; node != _root; node = node->parent)
      {
        if (internal_nodes.find(node) == internal_nodes.end())
        {
          if (is_node_ok(node, need_up))
          {
            internal_nodes[node] = true;
            internal_nodes[node->suffix_link] = false;
            break;
          }
          else if (!need_up)
          {
            internal_nodes[node] = false;
            internal_nodes[node->suffix_link] = false;
            break;
          }
        }
        else
        {
          internal_nodes[node->suffix_link] = false;
          break;
        }
      }
    }

    for (Iter iter = internal_nodes.begin(); iter != internal_nodes.end(); ++iter)
    {
      if (iter->second == true)
      {
        Node* node = iter->first;
        wstring substr = trim(node, text, pos_types);
        if (substr.length() >= _min_substr_len)
        {
          result_vec.push_back(Pair(substr, node->freq));
        }
      }
    }


  }


};

} //----end of namespace gezi

#endif  //----end of RSTREE_H_

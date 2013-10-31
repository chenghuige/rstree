/** 
 *  ==============================================================================
 * 
 *          \file   dsuffix_tree.h
 *
 *        \author   chenghuige  
 *          
 *          \date   2013-08-20 22:03:00.758982
 *  
 *  \Description:  在suffix tree基础上提供查询满足条件的串
 *                1. 频次限制 2. 长度限制 （最长，最短）
 *                3. 在前2者情况下有包含关系的如何处理
 *                   3.1 只留长的   策略1
 *                   3.2 如果频次相同 只留长的 否则都要   策略2
 *                 这里只做这样的限制，其余限制比如 按照黑名单过滤，数目限制（排序选优） 后续模块处理
 * 
 * TODO 增加对于一个文本 模拟添加 不增加count 返回满足结果的串
 * 
 * 注意达到频次要求 但是超长的情况下 只取一个
 * 比如 abcdefghigk 连续100次 长度要求<=5 那只选一个 abcde即可
 *  ==============================================================================
 */

#ifndef DSUFFIX_TREE_H_
#define DSUFFIX_TREE_H_

#include <algorithm>
#include "SuffixTree.h"
#include "rstree_def.h"
#include "hashmap_util.h"
//#include "utils/string/wstring_utils.h"
#include "log_util.h"
#include "configure_util.h"

//Generalized Suffix tree with dynamic count support
namespace gezi
{

class DSuffixTree : public SuffixTree
{
public:

  enum STRATEGY
  {
    FREQ_FIRST, //频率优先 参考头注释 策略1
    LENGTH_FIRST //长度优先
  };

  int min_substr_len_; //重复子串最短长度
  int max_substr_len_; //重复子串最大长度
  int min_frequency_; //满足的最低频次限制
  int max_tree_size; //树的大小限制

  int strategy_;

  DSuffixTree()
  : SuffixTree(L"\n")
  {
    min_substr_len_ = 8;
    max_substr_len_ = 20;
    min_frequency_ = 15;
    max_tree_size = 300000;
    strategy_ = 1;
  }

  bool init()
  {
    return init(SharedConf::get_conf());
  }
  
  bool init(const comcfg::Configure& conf,
          const string& section = "DSuffixTree")
  {
    CONF_SIMPLE(min_substr_len_);
    CONF_SIMPLE(max_substr_len_);
    CONF_SIMPLE(min_frequency_);
    CONF_SIMPLE(max_tree_size);
    CONF_SIMPLE(strategy_);
    return true;
  }

  bool read_config(const comcfg::Configure& conf,
          const string& section = "DSuffixTree")
  {
    CONF_SIMPLE(min_substr_len_);
    CONF_SIMPLE(max_substr_len_);
    CONF_SIMPLE(min_frequency_);
    CONF_SIMPLE(max_tree_size);
    CONF_SIMPLE(strategy_);
    return true;
  }

  bool read_config(const string& path = "", const string& config_file = "",
          const string& section = "DSuffixTree")
  {
    return read_config(path.c_str(), config_file.c_str(), section);
  }

  bool init(const string& path, const string& config_file,
          const string& section = "DSuffixTree")
  {
    return read_config(path.c_str(), config_file.c_str(), section);
  }

  bool read_config(const char* path, const char* config_file,
          const string& section = "DSuffixTree")
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

  int get_strategy()
  {
    return strategy_;
  }

  void set_min_frequency(int freq)
  {
    min_frequency_ = freq;
  }

  int get_min_frequency()
  {
    return min_frequency_;
  }

  void set_min_substr_len(int len)
  {
    min_substr_len_ = len;
  }

  int get_min_substr_len()
  {
    return min_substr_len_;
  }

  void set_max_substr_len(int len)
  {
    max_substr_len_ = len;
  }

  int get_max_substr_len()
  {
    return max_substr_len_;
  }

  void set_tree_size(int size)
  {
    max_tree_size = size;
  }

  int get_max_tree_size()
  {
    return max_tree_size;
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
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    return std::move(result_vec);
#else
    return result_vec;
#endif
    return result_vec;
  }

  void add_text(const wstring& text, vector<SubStrFreq>& result_vec)
  {
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
  }

  void search_text(const wstring& text, vector<SubStrFreq>& result_vec)
  {
    wstring s = text;
    boost::replace_all(s, end_mark_, L"");
    s = s + end_mark_;
    texts_.push_back(s);
    add_silent(s);
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
  }

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


  //参考注释按照策略2
  //极端的比如 就是abcdefg这种 可能由于超长 而上面没有短边满足条件 被忽略 但是这种情况极少发生 在大的语料中忽略这种情况

  void find_substrs_freqfirst(const wstring& text, vector<SubStrFreq>& result_vec)
  {
    Node* leaf_node;
    std::tr1::unordered_map < Node*, bool> internal_nodes_map;
    typedef std::tr1::unordered_map < Node*, bool>::iterator Iter;
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
    std::tr1::unordered_map < Node*, bool> internal_nodes_map;
    typedef std::tr1::unordered_map < Node*, bool>::iterator Iter;
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

  //do not increase for seach only

  void add_silent(const wstring & text)
  {
    int text_length = text.length();
    Node* active_node = root_; //active_node记录要考虑插入的节点
    Node* next_node = NULL; //由于没有显示的边数据结构，active_node -> next_node 表示当前active edge
    Node* pre_leaf = NULL;
    int active_length = 0; //相对于active_node的length
    int remainder = 1; //需要处理的后缀数目
    //例如输入abcaf
    //依次完成如下子后缀树建立 a, ab, abc, abca ... abcabf
    for (int end_idx = 0; end_idx < text_length; end_idx++, remainder++)
    {
      //建立后缀树 [0, i]
      //循环 remainder 次 (不必考虑[0,end_idx] [1,end_idx] ...[end_idx,end_idx] 
      //只需要考虑[end_idx - remainder + 1, end_idx]..[end_idx,end_idx])
      //remainder >= 1  remainder == 1意味着active_node == root 从头开始插入
      Node* mid_node; //当前指向新建叶子节点时 in_node 通过边长为1的边指向该叶子节点 即分裂边上的新生节点  a -- mid_node -- b
      Node* old_node = root_; //记录循环中前一次的in_node位置，第一次设置为root_

      for (; remainder > 0; remainder--)
      {
        Node* leaf_node = NULL;

        int edge_length = 0;
        if (active_length > 0)
        {
          edge_length = shift_down(text, end_idx, active_node, next_node, active_length);
        }

        if (active_length == 0)
        {
          NIter iter = active_node->next->find(text[end_idx]);
          if (iter != active_node->next->end())
          { //2. 隐节点(在active_edge边上积累)
            if (end_idx != text_length - 1)
            { //最后一位 标记位#
              active_length++;
              next_node = iter->second;
              break;
            }
            else
            { //如果不是要计数 而是只判断是否出现过 那么可以不需要这种特殊处理结尾相同的情况 那样直接后续串被略过
              leaf_node = iter->second;
            }
          }
          else
          {//否则1. 第一种叶子节点加入方式，内部节点生成一个新边指向新生成的叶子
            mid_node = active_node;
          }
        }
        else
        {
          int test_pos = next_node->start + active_length;

          wchar_t test_char = texts_[next_node->text_id - oldest_text_id_][test_pos]; //边对应的text id由 next node 决定

          if (test_char == text[end_idx])
          {
            if (end_idx != text_length - 1)
            {
              active_length++;
              break; //2. 提前结束(early stop)   abc 在后缀树中 那么 bc 必然也已经在
            }
            else
            {//比如 ab# ab#匹配到# remainder = 3 active_length = 2 end_idx = text_length
              leaf_node = next_node;
            }
          }
          else
          { //3. 第二种叶子结点加入方式，内部边分裂，产生一个新的内部节点，并且指向新生成的叶子节点
            wchar_t first_char = texts_[next_node->text_id - oldest_text_id_][next_node->start];
            mid_node = split_edge(active_node, next_node, first_char, test_char, test_pos, edge_length, active_length);
          }
        }

        bool is_leaf_match = false;

        if (!leaf_node)
        {
          //生成新叶(内部节点插入,内部边分裂两种1情形的生成12新叶统一放到这里处理)
          //叶子节点始终会是叶子节点,所以结束位置问输入文本结束位置
          leaf_node = new Node(mid_node, end_idx, text_length, mid_node->length + (text_length - end_idx), current_text_id_, 0);
          mid_node->next->insert(Edges::value_type(text[end_idx], leaf_node));
        }
        else
        {
          is_leaf_match = true;
          leaf_node->text_id = current_text_id_; //更新节点信息为最新文本中的位置
          int edge_length = leaf_node->end - leaf_node->start;
          leaf_node->start = text_length - edge_length;
          leaf_node->end = text_length;
        }

        if (pre_leaf)
        {
          pre_leaf->suffix_link = leaf_node;
        }
        else
        {
          first_leafs_.push_back(leaf_node);
        }
        pre_leaf = leaf_node;

        //最后更新状态
        //由于是多次插入串 可能存在前面未匹配分裂 之后匹配直到最后叶子
        if (!is_leaf_match)
        { //注意如果是最后两个不同text 叶子匹配 这种 不要修改影响到正常的suffix link
          //对于连续生成新点的情况，创建suffix link
          if (old_node != root_)
          {
            old_node->suffix_link = mid_node;
          }
          old_node = mid_node;
        }
        else
        { //如果上次分裂 当前匹配到最后结束符都相同 注意suffix link要连上,再后续循环 必然都完全匹配后缀 suffix link已经配置好
          if (old_node != root_)
          {
            old_node->suffix_link = active_node;
            old_node = root_;
          }
        }

        if (active_node != root_)
        { //active_node->suffix_link == NULL 即是root
          active_node = active_node->suffix_link;
        }
        else if (active_length > 0)
        {
          active_length--;
        }
        next_node = NULL;
      }
      //最后一个old_node 需要处理suffix_link
      //最后一个old_node如果是最后一次产生的 一定对应old_node == active_node == root_
      //而如果不是那必然最后一次是匹配了然后break 这时候 前一次余下的 old_node需要suffix_link
      //处理完remainder个后缀后 注意最后一个结束有两种可能
      //1.回到原点old_node == root_ active_node == root_  考虑"xtptxy" 插入最后y的时候 xy y  
      //2.最后再某个位置匹配 然后break了  如果是在边中间非第一个位置break 那么 old_node == root_ 不需要处理什么
      //  如果是在边第一个位置break 同时old_node != root_ 也就是说 之前产生了一个新的内部点(old_node)需要一个 suffix_link
      //  比如前面  root -- .... --ab--ce 
      //                                       --d
      //active_node--next_node对应ab位置 之后 active_node = active_node->suffix_link  那么后续会有
      //... --ab -- d  
      //           -- d
      //因为active_length == edge_legnth 所以 active_node会继续向下走到  --d 因此 old_node->suffix_link = active_node 即可
      if (old_node != root_)
      {
        old_node->suffix_link = active_node;
      }
    }
  }

};
}

#endif  //__DSUFFIX_TREE_H_

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
    FREQ_FIRST, //Ƶ������ �ο�ͷע�� ����1
    LENGTH_FIRST //��������
  };

  int min_substr_len_; //�ظ��Ӵ���̳���
  int max_substr_len_; //�ظ��Ӵ���󳤶�
  int min_frequency_; //��������Ƶ������
  int max_tree_size; //���Ĵ�С����

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


  //�ο�ע�Ͱ��ղ���2
  //���˵ı��� ����abcdefg���� �������ڳ��� ������û�ж̱��������� ������ ��������������ٷ��� �ڴ�������к����������

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
    Node* active_node = root_; //active_node��¼Ҫ���ǲ���Ľڵ�
    Node* next_node = NULL; //����û����ʾ�ı����ݽṹ��active_node -> next_node ��ʾ��ǰactive edge
    Node* pre_leaf = NULL;
    int active_length = 0; //�����active_node��length
    int remainder = 1; //��Ҫ����ĺ�׺��Ŀ
    //��������abcaf
    //������������Ӻ�׺������ a, ab, abc, abca ... abcabf
    for (int end_idx = 0; end_idx < text_length; end_idx++, remainder++)
    {
      //������׺�� [0, i]
      //ѭ�� remainder �� (���ؿ���[0,end_idx] [1,end_idx] ...[end_idx,end_idx] 
      //ֻ��Ҫ����[end_idx - remainder + 1, end_idx]..[end_idx,end_idx])
      //remainder >= 1  remainder == 1��ζ��active_node == root ��ͷ��ʼ����
      Node* mid_node; //��ǰָ���½�Ҷ�ӽڵ�ʱ in_node ͨ���߳�Ϊ1�ı�ָ���Ҷ�ӽڵ� �����ѱ��ϵ������ڵ�  a -- mid_node -- b
      Node* old_node = root_; //��¼ѭ����ǰһ�ε�in_nodeλ�ã���һ������Ϊroot_

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
          { //2. ���ڵ�(��active_edge���ϻ���)
            if (end_idx != text_length - 1)
            { //���һλ ���λ#
              active_length++;
              next_node = iter->second;
              break;
            }
            else
            { //�������Ҫ���� ����ֻ�ж��Ƿ���ֹ� ��ô���Բ���Ҫ�������⴦���β��ͬ����� ����ֱ�Ӻ��������Թ�
              leaf_node = iter->second;
            }
          }
          else
          {//����1. ��һ��Ҷ�ӽڵ���뷽ʽ���ڲ��ڵ�����һ���±�ָ�������ɵ�Ҷ��
            mid_node = active_node;
          }
        }
        else
        {
          int test_pos = next_node->start + active_length;

          wchar_t test_char = texts_[next_node->text_id - oldest_text_id_][test_pos]; //�߶�Ӧ��text id�� next node ����

          if (test_char == text[end_idx])
          {
            if (end_idx != text_length - 1)
            {
              active_length++;
              break; //2. ��ǰ����(early stop)   abc �ں�׺���� ��ô bc ��ȻҲ�Ѿ���
            }
            else
            {//���� ab# ab#ƥ�䵽# remainder = 3 active_length = 2 end_idx = text_length
              leaf_node = next_node;
            }
          }
          else
          { //3. �ڶ���Ҷ�ӽ����뷽ʽ���ڲ��߷��ѣ�����һ���µ��ڲ��ڵ㣬����ָ�������ɵ�Ҷ�ӽڵ�
            wchar_t first_char = texts_[next_node->text_id - oldest_text_id_][next_node->start];
            mid_node = split_edge(active_node, next_node, first_char, test_char, test_pos, edge_length, active_length);
          }
        }

        bool is_leaf_match = false;

        if (!leaf_node)
        {
          //������Ҷ(�ڲ��ڵ����,�ڲ��߷�������1���ε�����12��Ҷͳһ�ŵ����ﴦ��)
          //Ҷ�ӽڵ�ʼ�ջ���Ҷ�ӽڵ�,���Խ���λ���������ı�����λ��
          leaf_node = new Node(mid_node, end_idx, text_length, mid_node->length + (text_length - end_idx), current_text_id_, 0);
          mid_node->next->insert(Edges::value_type(text[end_idx], leaf_node));
        }
        else
        {
          is_leaf_match = true;
          leaf_node->text_id = current_text_id_; //���½ڵ���ϢΪ�����ı��е�λ��
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

        //������״̬
        //�����Ƕ�β��봮 ���ܴ���ǰ��δƥ����� ֮��ƥ��ֱ�����Ҷ��
        if (!is_leaf_match)
        { //ע����������������ͬtext Ҷ��ƥ�� ���� ��Ҫ�޸�Ӱ�쵽������suffix link
          //�������������µ�����������suffix link
          if (old_node != root_)
          {
            old_node->suffix_link = mid_node;
          }
          old_node = mid_node;
        }
        else
        { //����ϴη��� ��ǰƥ�䵽������������ͬ ע��suffix linkҪ����,�ٺ���ѭ�� ��Ȼ����ȫƥ���׺ suffix link�Ѿ����ú�
          if (old_node != root_)
          {
            old_node->suffix_link = active_node;
            old_node = root_;
          }
        }

        if (active_node != root_)
        { //active_node->suffix_link == NULL ����root
          active_node = active_node->suffix_link;
        }
        else if (active_length > 0)
        {
          active_length--;
        }
        next_node = NULL;
      }
      //���һ��old_node ��Ҫ����suffix_link
      //���һ��old_node��������һ�β����� һ����Ӧold_node == active_node == root_
      //����������Ǳ�Ȼ���һ����ƥ����Ȼ��break ��ʱ�� ǰһ�����µ� old_node��Ҫsuffix_link
      //������remainder����׺�� ע�����һ�����������ֿ���
      //1.�ص�ԭ��old_node == root_ active_node == root_  ����"xtptxy" �������y��ʱ�� xy y  
      //2.�����ĳ��λ��ƥ�� Ȼ��break��  ������ڱ��м�ǵ�һ��λ��break ��ô old_node == root_ ����Ҫ����ʲô
      //  ������ڱߵ�һ��λ��break ͬʱold_node != root_ Ҳ����˵ ֮ǰ������һ���µ��ڲ���(old_node)��Ҫһ�� suffix_link
      //  ����ǰ��  root -- .... --ab--ce 
      //                                       --d
      //active_node--next_node��Ӧabλ�� ֮�� active_node = active_node->suffix_link  ��ô��������
      //... --ab -- d  
      //           -- d
      //��Ϊactive_length == edge_legnth ���� active_node����������ߵ�  --d ��� old_node->suffix_link = active_node ����
      if (old_node != root_)
      {
        old_node->suffix_link = active_node;
      }
    }
  }

};
}

#endif  //__DSUFFIX_TREE_H_

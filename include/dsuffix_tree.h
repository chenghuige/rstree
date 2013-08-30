/** 
*  ==============================================================================
* 
*          \file   dsuffix_tree.h
*
*        \author   chenghuige  
*          
*          \date   2013-08-20 22:03:00.758982
*  
*  \Description:   TODO  当前策略是  如果 频率相同才保留长串，这样后缀树由于没有单边 自然不需要 判断 叶子到root的前缀包含关系
*                           否则需要建立黑名单 叶子比如满足了 那么 短的向上的内部节点 就不需要了
*  ==============================================================================
*/

#ifndef DSUFFIX_TREE_H_
#define DSUFFIX_TREE_H_

#include <algorithm>
#include "suffix_tree.h"
#include "rstree_def.h"
#include "hashmap_util.h"

//Generalized Suffix tree with dynamic count support

class DSuffixTree : public SuffixTree
{
public:
	//    const int DEFAULT_MAX_SUBSTR_CNT = 500;
	//   const int DEFAULT_MIN_SUBSTR_LEN = 6;
	//    const int DEFAULT_MAX_SUBSTR_LEN = 20000;
	//    const int DEFAULT_MIN_FREQUENCY = 10;

	int _max_substr_cnt; //请求的重复子串最大数量限制
	int _min_substr_len;
	int _max_substr_len;
	int _min_frequency;
	int _tree_size; //树的大小限制

	int _min_str_len; //建树文本最短长度限制
	int _max_str_len; //建树文本最大长度限制

	DSuffixTree()
		: SuffixTree(L"\n")
	{
		_max_substr_cnt = DEFAULT_MAX_SUBSTR_CNT;
		_min_substr_len = DEFAULT_MIN_SUBSTR_LEN;
		_max_substr_len = DEFAULT_MAX_SUBSTR_LEN;
		_min_frequency = DEFAULT_MIN_FREQUENCY;
		_tree_size = DEFAULT_TREE_SIZE;
	}

	DSuffixTree(int max_substr_cnt, int min_substr_len, int max_substr_len, int min_frequency)
		: _max_substr_cnt(max_substr_cnt), _min_substr_len(min_substr_len),
		_max_substr_len(max_substr_len), _min_frequency(min_frequency)
	{

	}

	/**
	* @brief: 对字符串两边的空白字符做剪切
	* @param s: 待剪切字符串
	* @retval: 剪切后的字符串
	*
	*/
	wstring trim(const wstring & s)
	{
		wstring ret;
		int start = 0, end = s.size() - 1;
		while (start < (int) s.size() && (s[start] == L' ' || s[start] == L'\t' || s[start] == L'\n' || s[start] == L'\r'))
			start++;
		while (end >= 0 && (s[end] == L' ' || s[end] == L'\t' || s[end] == L'\n' || s[end] == L'\r'))
			end--;
		if (start > end)
			return L"";
		return s.substr(start, end - start + 1);
	}

	void set_min_frequency(int freq)
	{
		_min_frequency = freq;
	}

	int get_min_frequency()
	{
		return _min_frequency;
	}

	void set_min_substr_len(int len)
	{
		_min_substr_len = len;
	}

	int get_min_substr_len()
	{
		return _min_substr_len;
	}

	void set_max_substr_len(int len)
	{
		_max_substr_len = len;
	}

	int get_max_substr_len()
	{
		return _max_substr_len;
	}

	void set_tree_size(int size)
	{
		_tree_size = size;
	}

	int get_max_tree_size()
	{
		return _tree_size;
	}

	void set_min_str_len(int len)
	{
		_min_str_len = len;
	}

	int get_min_str_len()
	{
		return _min_str_len;
	}

	void set_max_str_len(int len)
	{
		_max_str_len = len;
	}

	int get_max_str_len()
	{
		return _max_str_len;
	}

	void set_max_substr_cnt(int cnt)
	{
		_max_substr_cnt = cnt;
	}

	int get_max_substr_cnt()
	{
		return _max_substr_cnt;
	}

	typedef std::pair<wstring, int> SubStrFreq; //first is substring, second is freq

	void add_text(const wstring& text, vector<SubStrFreq>& result_vec)
	{
		wstring s = text;
		replace_all(s, _end_mark, L"");
		s = s + _end_mark;
		_texts.push_back(s);
		add_(s);
		find_substr(s, result_vec);
		_current_text_id++;
	}

	struct CmpByLength
	{

		template<typename _Node>
		bool operator() (const _Node& i, const _Node & j)
		{
			return (i.first.length() > j.first.length());
		}
	};

	void add_text(const wstring& text, map<wstring, int>& result_map)
	{
		vector<SubStrFreq> result_vec;
		add_text(text, result_vec);

		int len = result_vec.size();
		for (int i = 0; i < len; i++)
		{
			result_vec[i].first = trim(result_vec[i].first);
		}

		if (len > _max_substr_cnt)
		{
			std::partial_sort(result_vec.begin(), result_vec.begin() + _max_substr_cnt, result_vec.end(), CmpByLength());
			result_map.insert(result_vec.begin(), result_vec.begin() + _max_substr_cnt);
		}
		else
		{
			result_map.insert(result_vec.begin(), result_vec.end());
		}
	}

	bool IsNodeOk(Node* node)
	{
		return node->freq >= _min_frequency
			&& node->length >= _min_substr_len
			&& node->length <= _max_substr_len;
	}

	bool IsLeafOk(Node* leaf)
	{
		int length = leaf->length - 1;
		return leaf->freq >= _min_frequency
			&& length >= _min_substr_len
			&& length <= _max_substr_len;
	}

	void find_substr(const wstring& text, vector<SubStrFreq>& result_vec)
	{
		Node* leaf_node;
		int pre_leaf_freq = 0;
		std::tr1::unordered_map < Node*, bool> internal_nodes_map;
		typedef std::tr1::unordered_map < Node*, bool>::iterator Iter;
		for (leaf_node = _first_leafs[_current_text_id - _oldest_text_id]; leaf_node != NULL; leaf_node = leaf_node->suffix_link)
		{
			if (leaf_node->freq > pre_leaf_freq
				&& IsLeafOk(leaf_node)
				)
			{
				wstring substr = text.substr(leaf_node->end - leaf_node->length, leaf_node->length - 1);
				result_vec.push_back(SubStrFreq(substr, leaf_node->freq));
			}

			for (Node* node = leaf_node->parent; node != _root; node = node->parent)
			{
				if (internal_nodes_map.find(node) == internal_nodes_map.end())
				{
					if (IsNodeOk(node))
					{
						internal_nodes_map[node] = true;
					}
				}
				else
				{
					break;
				}
			}

			pre_leaf_freq = leaf_node->freq;

		}

		for (Iter iter = internal_nodes_map.begin(); iter != internal_nodes_map.end(); ++iter)
		{
			Node* node = iter->first;
			Node* shorter_node = node->suffix_link;

			if (shorter_node != _root && node->freq == shorter_node->freq)
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
				wstring substr = text.substr(node->end - node->length, node->length);
				result_vec.push_back(SubStrFreq(substr, node->freq));
			}
		}

	}
};


#endif  //__DSUFFIX_TREE_H_

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
		int _min_substr_len; //�ظ��Ӵ���̳���
		int _min_substr_len2; //�ظ��Ӵ���̳���
		int _max_substr_len; //�ظ��Ӵ���󳤶�
		int _min_frequency; //��������Ƶ������
		int _max_tree_size; //���Ĵ�С����
		double _max_jump; //�����ظ��� ���⴦��

		Rstree(const wstring& end_mark = L"\n", int min_substr_len = 8, 
			int max_substr_len = 40, int min_frequency = 15, int max_tree_size = 300000)
			: SuffixTree(end_mark)
		{
			init(min_substr_len, max_substr_len, min_frequency, max_tree_size);
			_min_substr_len2 = 16;
			_max_jump = 5; //ƽ������5����֧��ʱ��ȡ����� Ҳ����˵Ƶ��n �½��� < n/_max_jump ��ôȡ����� 
		}

		bool init()
		{
			return init(SharedConf::get_conf());
		}

		bool init(const comcfg::Configure& conf,
			string section = "Rstree")
		{
			CONF(_min_substr_len);
			CONF(_max_substr_len);
			CONF(_min_frequency);
			CONF(_max_tree_size);
			CONF(_max_jump);
			return true;
		}

		void init(int min_substr_len, int max_substr_len, int min_freq, int tree_size)
		{
			_min_substr_len = min_substr_len;
			_max_substr_len = max_substr_len;
			_min_frequency = min_freq;
			_max_tree_size = tree_size;
		}

		bool read_config(const comcfg::Configure& conf,
			string section = "Rstree")
		{
			CONF(_min_substr_len);
			CONF(_max_substr_len);
			CONF(_min_frequency);
			CONF(_max_tree_size);
			return true;
		}

		bool read_config(string path = "./conf", string config_file = "Rstree.conf",
			string section = "Rstree")
		{
			return read_config(path.c_str(), config_file.c_str(), section);
		}

		bool init(string path, string config_file,
			string section = "Rstree")
		{
			return read_config(path.c_str(), config_file.c_str(), section);
		}

		bool read_config(const char* path, const char* config_file,
			string section = "Rstree")
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

		void add_find(const wstring& text, vector<Pair>& result_vec, const vector<int>* pos_types = NULL)
		{
			wstring s = text;
			s = s + _end_mark;
			_texts.push_back(s);
			add_(s);
			find_substrs(result_vec, pos_types);
			_current_text_id++;
			if (tree_size() >= _max_tree_size)
			{
				remove();
			}
		}

		vector<Pair> add_find(const wstring& text, const vector<int>* pos_types = NULL)
		{
			vector<Pair> result_vec;
			wstring s = text;
			boost::trim(s);
			//    boost::replace_all(s, _end_mark, L"");  //תstring֮ǰ�Ѿ�ȥ������
			s = s + _end_mark;
			_texts.push_back(s);
			add_(s);
			find_substrs(result_vec, pos_types);
			_current_text_id++;
			if (tree_size() >= _max_tree_size)
			{
				remove();
			}
			return result_vec;
		}

		//TODO �ṩsearch���� ��ѯ�����ı��ں�׺��������Щ���㳤�Ⱥ�Ƶ��Ҫ����Ӵ�,��Ӱ�쵱ǰ��׺��
		//���԰��ռ��� Ȼ��ɾ�������ı� ���У� ���������� ����ṩsearch �ɷ���̲߳�ѯ�� 
		//  vector<Pair> search(const wstring& text)
		//  {
		//  
		//  }

		bool is_node_ok(const Node* node)
		{
			if (node->next == NULL)
			{ //Ҷ�ӽڵ�
				if (node->end - node->start == 1)
				{//ֻ��ĩβ����ַ������
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
				{//ֻ��ĩβ����ַ������
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

		bool is_node_down_ok(const Node* node, bool& need_down)
		{
			if (node->next == NULL)
			{
				if (node->freq <= _min_frequency)
				{
					need_down = false;
				}

				if (node->end - node->start == 1)
				{//ֻ��ĩβ����ַ������
					return false;
				}

				return node->freq >= _min_frequency
					&& node->length >= _min_substr_len + 1
					&& node->length <= _max_substr_len + 1;
			}
			else
			{
				if (node->freq <= _min_frequency)
				{
					need_down = false;
				}
				return node->freq >= _min_frequency
					&& node->length >= _min_substr_len
					&& node->length <= _max_substr_len;
			}
		}


		inline wstring trim(const wstring& text, const vector<int>& splits, int start, int end)
		{
			while (start < end && splits[start] != LEFT && splits[start] != SINGLE)
			{
				start++;
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

		inline wstring get_str(const Node* node)
		{
			wstring& text = _texts[node->text_id - _oldest_text_id];
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

		//rstree ��������ظ������⴦��ġ���
		inline Node* get_node(Node* node)
		{
			Node* rnode = node;
			Node* node2 = node->parent;
			while (node2 != _root && node2->length >= _min_substr_len2)
			{
				if (node2->freq / (double)node->freq > _max_jump)
				{
					rnode = node2;
				}
				node = node2;
				node2 = node->parent;
			}
			return rnode;
		}

		void find_all_substrs(Node* node, vector<Pair>& result_vec)
		{
			if (!node)
			{
				return;
			}

			bool need_down = true; //must be true
			if (is_node_down_ok(node, need_down))
			{
				result_vec.push_back(Pair(get_str(node), node->freq));
			}
			else if (!need_down || !node->next)
			{
				return;
			}
			else
			{
				for (NIter iter = node->next->begin(); iter != node->next->end(); ++iter)
				{
					find_all_substrs(iter->second, result_vec);
				}
			}
		}

		vector<Pair> find_all_substrs()
		{
			vector<Pair> result_vec;
			find_all_substrs(_root, result_vec);
			return result_vec;
		}
		//���㳤�� Ƶ��Ҫ�� ����� ��������   ����Ҫ����������
		//��������� ���Ƴ��Ȳ�����ĳ����ֵ ѡȡ ���ܻ�ȡ���� ����abcdefghigk...�ظ�n�� ���Ǵ������и��ʼ�С
		//���������� ����ٽ�ȡ  �����޷���ȡ���ڴʲ��ֵĴ� �ۺ�Ȩ�� �����������

		void find_substrs(vector<Pair>& result_vec)
		{
			int idx = _current_text_id - _oldest_text_id;
			find_substrs(idx, result_vec);
		}

		vector<Pair>  find_substrs(int idx)
		{
			vector<Pair> results;
			find_substrs(idx, results);
			return results;
		}

		void find_substrs(int idx, vector<Pair>& result_vec)
		{
			Node* leaf_node;
			std::unordered_map < Node*, bool> internal_nodes;
			typedef std::unordered_map < Node*, bool>::iterator Iter;

			wstring text = _texts[idx];
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
					result_vec.push_back(Pair(get_str(node), node->freq));
				}
			}
		}

		//-----------------��������ظ��������⴦��
		void find_substrs(vector<Pair>& result_vec, const vector<int>* pos_types)
		{
			int idx = _current_text_id - _oldest_text_id;
			find_substrs(idx, result_vec, pos_types);
		}
		
		void find_substrs(int idx, vector<Pair>& result_vec, const vector<int>* pos_types)
		{
			Node* leaf_node;
			std::unordered_map < Node*, bool> internal_nodes;
			typedef std::unordered_map < Node*, bool>::iterator Iter;

			wstring text = _texts[idx];
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
					node = get_node(node);  //@TODO more general
					wstring substr = trim(node, text, pos_types); //@TODO more general Ӧ���ṩ�����ɵ�general�汾
					//Pval(wstr_to_str(trim(node, text)));
					//Pval(wstr_to_str(substr));
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

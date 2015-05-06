/** 
 *  ==============================================================================
 * 
 *          \file   suffix_tree.h
 *
 *        \author   chenghuige  
 *          
 *          \date   2013-08-15 05:42:41.872766
 *  
 *  \Description:  ֧�ֶ�̬��Ӻ�ɾ���ĺ�׺��  
 *                 TODO ֧�����л�
 *  ==============================================================================
 */

#ifndef SUFFIX_TREE_H_
#define SUFFIX_TREE_H_
#include <string>
#include <map>
#include <deque>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include "hashmap_util.h"
#include "string_util.h"

namespace gezi
{
using namespace std;
using std::wstring;
using std::vector;

class SuffixTree
{
public:
	//����һ����#��Ϊ����������ʾ��ʵ��һ��ѡ��\n����Ϊ��������

	SuffixTree(const wstring& end_mark = L"#")
	: _end_mark(end_mark)
	{
		init();
	}

	virtual ~SuffixTree()
	{
		free();
	}

	struct Node;
	//typedef std::map<wchar_t, Node*, less<wchar_t> > Edges; //�ڴ�ռ�ý��� ������map ���Ͽ�����hash map����ٶ�
	typedef std::unordered_map<wchar_t, Node*> Edges;
	typedef Edges::iterator NIter; //typename? ? TODO

	struct Node
	{
		Node()
		{
			Init();
			parent = NULL;
			start = 0;
			end = 0;
			length = 0;
			text_id = 0;
		}

		void Init()
		{
			next = NULL;
			freq = 0;
			suffix_link = NULL;
		}

		Node(Node* _parent, int _id = 0)
		{
			Init();
			parent = _parent;
			text_id = _id;
		}

		Node(Node* _parent, int _start, int _end, int _length, int _id = 0, int _freq = 0)
		{
			Init();
			parent = _parent;
			start = _start;
			end = _end;
			length = _length;
			text_id = _id;
			freq = _freq;
		}

		inline bool is_leaf()
		{
			return next == NULL;
		}

		int length; //��root����ǰ����ܱ߳�
		int start; //������text�е���ʼλ��
		int end; //������text�е�pass 1 ����λ�� end-start��ʾ�ߵĳ���
		int freq; //��root���ñ߶�Ӧ�Ĵ�����Ƶ��

		Node* parent; //ָ���׽ڵ� ����ͳ��Ƶ��
		Node* suffix_link; // v1 -> v2  v1 a(D)  v2 (D), v1 v2 ��Ϊ�ڲ��ڵ�  ��ʵ��Ҷ�ӽ�㲻��Ҫsuffix_link
		Edges* next; //Ҷ�ӽ��next == NULL

		int text_id; //node id ָ��text����texts�����е�λ��
	};

	void init()
	{
		_root = new Node;
		_root->next = new Edges;
		reset_status();
	}

	void reset_status()
	{
		_current_text_id = 0;
		_oldest_text_id = 0;
	}

	//���һ����׺������ͷ�ڵ����ݣ��������㣬���ڶ��clear add����

	void reset()
	{
		clear();
		reset_status();
	}
	//�ͷź�׺�����пռ�

	void free()
	{
		clear(_root);
		_root = NULL;
	}

	void clear()
	{
		for (NIter iter = _root->next->begin(); iter != _root->next->end(); ++iter)
		{
			clear(iter->second);
		}
		delete _root->next;
		_root->next = NULL;
	}

	void clear(Node* node)
	{
		if (node)
		{
			if (node->next)
			{
				for (NIter iter = node->next->begin(); iter != node->next->end(); ++iter)
				{
					clear(iter->second);
				}
				delete node->next;
			}
			delete node;
		}
	}

	inline wstring& end_mark()
	{
		return _end_mark;
	}

	inline size_t tree_size()
	{
		return _texts.size();
	}

	inline size_t size()
	{
		return _texts.size();
	}

protected:
	typedef unsigned long long uint64;
	Node* _root;
	uint64 _current_text_id;
	uint64 _oldest_text_id;
	std::deque<wstring> _texts;
	std::deque<Node*> _first_leafs;
	wstring _end_mark;
public:

	void build(const wstring& text)
	{
		wstring s = text + _end_mark;
		boost::replace_all(s, _end_mark, L"");
		_texts.push_back(s);
		build_(s);
		_current_text_id++;
	}

	void add(const wstring& text)
	{
		wstring s = text;
		boost::replace_all(s, _end_mark, L"");
		s = s + _end_mark;
		_texts.push_back(s);
		add_(s);
		_current_text_id++;
	}

	//remove the oldest one

	void remove()
	{
		if (_texts.size() == 0)
		{
			return;
		}

		remove_();

		_texts.pop_front();
		_first_leafs.pop_front();

		_oldest_text_id++;
	}

	//ɾ��ָ��λ�õ��ı� TODO ������� ��Ҫ�� texts_ first_leafs_�洢Ϊ map<int,..>��ʽ����ɾ������λ��

	void remove(int id)
	{

	}

	//���Ҵ�һ���ַ���ָ��λ��(start)��ʼ �ں�׺�����ƥ��Ĵ�

	Node* find_longest_node(const wstring& text, int start = 0)
	{
		int text_length = text.length() - start;
		if (text_length <= 0)
		{
			return _root;
		}

		Node* active_node = _root;
		int cur = start;

		while (cur < text_length)
		{
			NIter iter = active_node->next->find(text[cur]);
			if (iter == active_node->next->end())
			{ //�ҵ���ƥ��
				return active_node;
			}
			active_node = iter->second;
			int end = cur + (active_node->end - active_node->start);
			int j;
			for (j = cur + 1; j < end && j < text_length; j++)
			{
				if (_texts[active_node->text_id][active_node->start + j] != text[j])
				{//�ҵ���ƥ��
					return active_node;
				}
			}
			if (j == text_length)
			{ //�ı���ƥ����
				return active_node;
			}

			if (active_node->is_leaf())
			{//�ı���δƥ���� �����Ѿ���Ҷ�ӽڵ�
				return active_node;
			}

			cur = end;
		}

		return active_node;
	}

	//����ƥ�䵽�ĳ���

	int find_longest(const wstring& text, int start)
	{
		Node* node = find_longest_node(text, start);
		return node->length;
	}

	int find_longest(const wstring& text, int start, int & freq)
	{
		Node* node = find_longest_node(text, start);
		if (node == _root)
		{ //root_->freq ��¼�˺�׺��һ���ж��ٸ�Ҷ�ӽڵ� = ���ı�����
			freq = 0;
		}
		freq = node->freq;
		return node->length;
	}

	//�ں�׺���в���һ���ַ��� ���ҷ��ؼ�¼Ƶ����Ϣ�ȵ�Node�ڵ� ���·����ĳ�β���ʧ�ܻ����ߵ�Ҷ�ӽڵ�text��Ȼû������ ����root_

	Node* find_node(const wstring& text, int start = 0)
	{
		int text_length = text.length() - start;
		if (text_length <= 0)
		{
			return NULL;
		}

		Node* active_node = _root;
		int cur = start;

		while (cur < text_length)
		{
			NIter iter = active_node->next->find(text[cur]);
			if (iter == active_node->next->end())
			{ //�ҵ���ƥ��
				return NULL;
			}
			active_node = iter->second;
			int end = cur + (active_node->end - active_node->start);
			int j, idx = 1;
			for (j = cur + 1; j < end && j < text_length; j++, idx++)
			{
				if (_texts[active_node->text_id][active_node->start + idx] != text[j])
				{//�ҵ���ƥ��
					return NULL;
				}
			}
			if (j == text_length)
			{ //�ı���ƥ����
				return active_node;
			}

			if (active_node->is_leaf())
			{//�ı���δƥ���� �����Ѿ���Ҷ�ӽڵ�
				return NULL;
			}

			cur = end;
		}

		return active_node;
	}

	//�ж�һ���ַ����Ƿ��ں�׺����

	bool find(const wstring& text, int start = 0)
	{
		return find_node(text, start) != NULL;
	}

	int find_freq(const wstring& text, int start = 0)
	{
		Node* node = find_node(text, start);
		if (node)
		{
			return node->freq;
		}
		return 0;
	}

	//ȥ����ɵ��ı���ɾ��������Լ���Ϊ���Ǽٶ�ɾ�����ı�����ǰ��ӵ� �����κκ�׺�����ں�׺���д����Ѿ�
	//�����ҵ���Ӧ�����ı���Ҷ�ӽ�㣬��Ӧ active_node - next_node next_node��ӦҶ�ӽ��
	//����ֱ��rootƵ��-1��Ȼ�����Ҷ�ӽ��Ƶ�ʽ�Ϊ0 ɾ��Ҷ�ӽ�� ͬʱ�ж�active_node�Ƿ�next mapֻ��1����
	//�������next_node�ϲ���active_node, active_node��ΪҶ�ӽ��
	//����active_node = active_node->suffix_link
	//���ַ�������Ϊm �������̾���ɾ��m

	void remove_()
	{
		for (Node* leaf_node = _first_leafs[0]; leaf_node != NULL; leaf_node = leaf_node->suffix_link)
		{
			dec_freq(leaf_node);
		}
	}


	//������һ��Ҷ�ӽ��

	void dec_freq(Node* node)
	{
		if (node->freq == 1)
		{ //��Ҫɾ��Ҷ��
			Node* active_node = node->parent;
			wchar_t key = _texts[node->text_id - _oldest_text_id][node->start];
			delete node;
			active_node->next->erase(key);
			if (active_node != _root && active_node->next->size() == 1) //root_��㲻����������
			{ //�����������, active_node��ɾ�� up -- active -- low  -> up -- low
				key = _texts[active_node->text_id - _oldest_text_id][active_node->start];
				NIter iter = active_node->next->begin();
				Node* low_node = iter->second;
				Node* up_node = active_node->parent;
				(*(up_node->next))[key] = low_node;
				low_node->parent = up_node;
				int edge_length = active_node->end - active_node->start; //or son_node->length - active_node->length
				low_node->start -= edge_length;

				delete active_node->next;
				delete active_node;
				node = up_node;
			}
			else
			{
				node = active_node;
			}
		}

		//����ֱ��root Ƶ�ʼ�һ
		for (; node != NULL; node = node->parent)
		{
			node->freq--;
		}
	}

protected:

	//��ȫ����
	//http://stackoverflow.com/questions/9452701/ukkonens-suffix-tree-algorithm-in-plain-english/9513423#9513423
	//��ʵ��remainder����ȥ���õ� ��ʽ�ϸ����Ľ�� Ч��Ӧ��һ�� ��������Ա�� 

	void build_(const wstring& text)
	{
		int text_length = text.length();
		Node* active_node = _root; //active_node��¼Ҫ���ǲ���Ľڵ�
		Node* next_node = NULL; //����û����ʾ�ı����ݽṹ��active_node -> next_node ��ʾ��ǰactive edge
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
			Node* mid_node; //��ǰָ���½�Ҷ�ӽڵ��ʱ�� in_node ͨ���߳�Ϊ1�ı�ָ���Ҷ�ӽڵ� �����ѱ��ϵ������ڵ�  a -- mid_node -- b
			Node* old_node = _root; //��¼ѭ����ǰһ�ε�in_nodeλ�ã���һ������Ϊroot_
			for (; remainder > 0; remainder--)
			{
				int edge_length;
				if (active_length > 0)
				{
					//���洦��ʹ��active_length < edge_len ���ܻ�����ĳ��·��������������� 
					//����һ��[start_idx, end_idx]���� remainder������ѭ���У����������� < actvie_length <= remainder,�����㷨��Ȼ����
					int first_char_pos = end_idx - active_length;
					if (next_node == NULL)
					{
						next_node = active_node->next->find(text[first_char_pos])->second;
					}
					while (true)
					{
						edge_length = next_node->end - next_node->start;
						if (active_length >= edge_length)
						{
							active_length -= edge_length;
							active_node = next_node;
							if (active_length == 0)
							{
								next_node = NULL;
								break;
							}
							else
							{
								first_char_pos += (active_node->end - active_node->start);
								next_node = next_node->next->find(text[first_char_pos])->second;
							}
						}
						else
						{
							break;
						}
					}
				}

				if (active_length == 0)
				{
					NIter iter = active_node->next->find(text[end_idx]);
					if (iter != active_node->next->end())
					{ //2. ���ڵ�(��active_edge���ϻ���)
						active_length++;
						next_node = iter->second;
						break;
					}
					else
					{//����1. ��һ��Ҷ�ӽڵ���뷽ʽ���ڲ��ڵ�����һ���±�ָ�������ɵ�Ҷ��
						mid_node = active_node;
					}
				}
				else
				{
					int pos = next_node->start + active_length;
					if (text[pos] == text[end_idx])
					{
						active_length++;
						break; //2. ��ǰ����(early stop)   abc �ں�׺���� ��ô bc ��ȻҲ�Ѿ���
					}
					else
					{ //3. �ڶ���Ҷ�ӽ����뷽ʽ���ڲ��߷��ѣ�����һ���µ��ڲ��ڵ㣬����ָ�������ɵ�Ҷ�ӽڵ�
						int remaining_len = edge_length - active_length;
						mid_node = new Node(active_node, next_node->start, pos,
										next_node->length - remaining_len, _current_text_id, next_node->freq);
						(*(active_node->next))[text[next_node->start]] = mid_node;

						next_node->parent = mid_node;
						next_node->start = pos;
						next_node->length = remaining_len;

						mid_node->next = new Edges;
						mid_node->next->insert(Edges::value_type(text[pos], next_node));
					}
				}
				//������Ҷ(�ڲ��ڵ����,�ڲ��߷����������ε�������Ҷͳһ�ŵ����ﴦ��)
				//Ҷ�ӽ��ʼ����Ҷ�ӽڵ����λ���������ı�����λ��  
				Node* leaf_node = new Node(mid_node, end_idx, text_length, mid_node->length + (text_length - end_idx), _current_text_id, 1);
				mid_node->next->insert(Edges::value_type(text[end_idx], leaf_node));
				//�������������µ�����������suffix link
				if (old_node != _root)
				{
					old_node->suffix_link = mid_node;
				}
				old_node = mid_node;

				if (active_node != _root)
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
			if (old_node != _root)
			{
				old_node->suffix_link = active_node;
			}
		}
	}

	//��Ӧgeneralized suffix tree ����������Ϊ����ַ����ı�) ���һ�� ����Ҫע��text_id��ʹ��
	//Adding b
	//|-#[0 1:2]  1 0x17d0f30->0
	//|-b#[0 0:2]  1 0x17d0eb0->0
	//Adding bc
	//|-#[0 1:2]  1 0x17d0f30->0
	//|-b[0 0:1]  1 0x17d17c0->0x17cd740
	//   |-#[0 1:2]  1 0x17d0eb0->0
	//   |-c#[1 1:3]  1 0x17d1880->0
	//|-c#[1 1:3]  1 0x17d1900->0
	//Adding bcd
	//|-#[0 1:2]  1 0x17d0f30->0
	//|-b[0 0:1]  1 0x17d17c0->0x17cd740
	//   |-#[0 1:2]  1 0x17d0eb0->0
	//   |-c[1 1:2]  1 0x17d19f0->0x17d1b30
	//      |-#[1 2:3]  1 0x17d1880->0
	//      |-d#[2 2:4]  1 0x17d1ab0->0
	//|-c[1 1:2]  1 0x17d1b30->0x17cd740
	//   |-#[1 2:3]  1 0x17d1900->0
	//   |-d#[2 2:4]  1 0x17d1bf0->0
	//|-d#[2 2:4]  1 0x17d1c70->0
	//���⿼�Ǽ������� �������� b ע���β���Ŷ�����ͬ�ģ��������������ͨ���� ab# ab# ƥ�䵽����ֹͣ�� �����˺����b#��
	//��ʵ�൱��ģ�� ab# ab$ ������
	//#  2
	//b# 2

	//���������ƶ� �������ձߵĳ���
	//���洦��ʹ��active_length < edge_len ���ܻ�����ĳ��·��������������� 
	//����һ��[start_idx, end_idx]���� remainder������ѭ���У����������� < actvie_length <= remainder,�����㷨��Ȼ����

	int shift_down(const wstring& text, int end_idx, Node*& active_node, Node*& next_node, int & active_length)
	{
		int edge_length;

		int first_char_pos = end_idx - active_length;
		if (next_node == NULL)
		{
			next_node = active_node->next->find(text[first_char_pos])->second;
		}
		edge_length = next_node->end - next_node->start;

		while (active_length >= edge_length)
		{
			active_length -= edge_length;
			active_node = next_node;
			if (active_length > 0)
			{
				first_char_pos += (active_node->end - active_node->start);
				next_node = next_node->next->find(text[first_char_pos])->second;
				edge_length = next_node->end - next_node->start;
			}
		}
		return edge_length;
	}

	//�Ա߽��з��� ���������ɵ��м���

	Node* split_edge(Node* active_node, Node* next_node, wchar_t first_char, wchar_t test_char,
					int split_pos, int edge_length, int active_length)
	{
		int remaining_len = edge_length - active_length;

		Node* mid_node = new Node(active_node, next_node->start, split_pos,
						next_node->length - remaining_len, next_node->text_id, next_node->freq);

		(*(active_node->next))[first_char] = mid_node;

		next_node->parent = mid_node;
		next_node->start = split_pos;

		mid_node->next = new Edges;
		mid_node->next->insert(Edges::value_type(test_char, next_node));

		return mid_node;
	}

	void add_(const wstring & text)
	{
		int text_length = text.length();
		Node* active_node = _root; //active_node��¼Ҫ���ǲ���Ľڵ�
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
			Node* mid_node = NULL; //��ǰָ���½�Ҷ�ӽڵ�ʱ in_node ͨ���߳�Ϊ1�ı�ָ���Ҷ�ӽڵ� �����ѱ��ϵ������ڵ�  a -- mid_node -- b
			Node* old_node = _root; //��¼ѭ����ǰһ�ε�in_nodeλ�ã���һ������Ϊroot_

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

					wchar_t test_char = _texts[next_node->text_id - _oldest_text_id][test_pos]; //�߶�Ӧ��text id�� next node ����

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
						wchar_t first_char = _texts[next_node->text_id - _oldest_text_id][next_node->start];
						mid_node = split_edge(active_node, next_node, first_char, test_char, test_pos, edge_length, active_length);
					}
				}

				bool is_leaf_match = false;

				if (!leaf_node)
				{
					//������Ҷ(�ڲ��ڵ����,�ڲ��߷����������ε�������Ҷͳһ�ŵ����ﴦ��)
					//Ҷ�ӽڵ�ʼ�ջ���Ҷ�ӽڵ�,���Խ���λ���������ı�����λ��
					leaf_node = new Node(mid_node, end_idx, text_length, mid_node->length + (text_length - end_idx), _current_text_id, 0);
					mid_node->next->insert(Edges::value_type(text[end_idx], leaf_node));
				}
				else
				{
					is_leaf_match = true;
					leaf_node->text_id = _current_text_id; //���½ڵ���ϢΪ�����ı��е�λ��
					int edge_length = leaf_node->end - leaf_node->start;
					leaf_node->start = text_length - edge_length;
					leaf_node->end = text_length;
				}

				inc_freq(leaf_node);

				if (pre_leaf)
				{
					pre_leaf->suffix_link = leaf_node;
				}
				else
				{
					_first_leafs.push_back(leaf_node);
				}
				pre_leaf = leaf_node;

				//������״̬
				//�����Ƕ�β��봮 ���ܴ���ǰ��δƥ����� ֮��ƥ��ֱ�����Ҷ��
				if (!is_leaf_match)
				{ //ע����������������ͬtext Ҷ��ƥ�� ���� ��Ҫ�޸�Ӱ�쵽������suffix link
					//�������������µ�����������suffix link
					if (old_node != _root)
					{
						old_node->suffix_link = mid_node;
					}
					old_node = mid_node;
				}
				else
				{ //����ϴη��� ��ǰƥ�䵽������������ͬ ע��suffix linkҪ����,�ٺ���ѭ�� ��Ȼ����ȫƥ���׺ suffix link�Ѿ����ú�
					if (old_node != _root)
					{
						old_node->suffix_link = active_node;
						old_node = _root;
					}
				}

				if (active_node != _root)
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
			if (old_node != _root)
			{
				old_node->suffix_link = active_node;
			}
		}
	}

	void calc_freq()
	{
		_root->freq = get_leaf_num(_root);
	}

	int get_leaf_num(Node * node)
	{
		using namespace std;
		if (!node->next)
		{ //Ҷ�ӽڵ�ֱ�ӷ���
			return node->freq;
		}
		for (NIter iter = node->next->begin(); iter != node->next->end(); ++iter)
		{
			node->freq += get_leaf_num(iter->second);
		}
		return node->freq;
	}

	//�ӵ�ǰҶ�ӽڵ�����ֱ��root(����root) ִ��freq +1����
	//�����˸��±���ϢΪ���µ��ı��е�λ�� Ϊ�˺���ɾ����Ҫ
	//TODO ���һ���ı��е��ظ�������1 ������ ����һ��unique freq λ

	void inc_freq(Node * node)
	{
		Node* pre_node = node;
		for (; node != NULL; node = node->parent)
		{
			node->freq++;
			if (node->text_id != _current_text_id)
			{ //���±���ϢΪ���ı�������λ��
				int edge_length = node->end - node->start;
				node->text_id = _current_text_id;
				node->end = pre_node->start;
				node->start = node->end - edge_length;
			}
			pre_node = node;
		}
	}

public:
	//����ʹ��map��Ϊ�� ˳����������ĸ����ڵ� (text_id, start point, end point, edge length node freq, node depth)
	//������ı� ����ԱȲ����Ƿ�����ȷ

	void write_result(const string & file)
	{
		wofstream ofs(file.c_str());
		write_result(_root, 0, ofs);
		wcout << "Writed result" << endl;
	}

	void write_result(Node* node, int depth, wofstream & ofs)
	{
		using namespace std;
		if (!node || !node->next)
		{
			return;
		}

		for (NIter iter = node->next->begin(); iter != node->next->end(); ++iter)
		{
			//iter->second->freq = 0;
			wstring edge = this->_texts[iter->second->text_id - _oldest_text_id].
							substr(iter->second->start, iter->second->end - iter->second->start);
			ofs << iter->second->text_id << " " << iter->second->start << " " << iter->second->end << " " << edge << " "
							<< (!iter->second->next) << " " << iter->second->freq << "  " << depth << endl;

			//             wcout << iter->second->text_id << " " << iter->second->start << " " << iter->second->end << " " << edge << " "
			//                    << (!iter->second->next) << " " << iter->second->freq << "  " << depth << endl;

			write_result(iter->second, depth + 1, ofs);
		}
	}

	void print()
	{
		wcout << "The string is: " << _texts[_texts.size() - 1] << endl;
		wcout << "Current text id: " << _current_text_id << "  Oldest text id: " << _oldest_text_id << " texts_.size: " << _texts.size() << endl;
		wcout << L"root_ " << _root << endl;
		int leaf_freq = 0;
		print(_root, 0, leaf_freq);

		//        wcout << "The string length is " << texts_[0].length() << endl;
		wcout << "The total leaf freq is " << leaf_freq << endl;
	}

	//input from root , input is not NULL

	void print(Node* node, int depth, int & leaf_freq)
	{
		using namespace std;
		if (!node->next)
		{
			return;
		}

		for (NIter iter = node->next->begin(); iter != node->next->end(); ++iter)
		{
			for (int i = 0; i < depth; i++)
			{
				wcout << "   ";
			}

			//TODO why wrong
			//wstring span = boost::wformat(L"[%X %X]") % iter->second->start %iter->second->end;

			if (!iter->second->next)
			{
				leaf_freq += iter->second->freq;
			}

			//            wcout << iter->second->text_id << " " << oldest_text_id_ << endl;
			//                wcout << texts_[iter->second->text_id - oldest_text_id_] << endl;
			//                wcout << iter->second->start << endl;
			//                wcout << (iter->second->end - iter->second->start) << endl;
			//                wcout << iter ->first << endl;
			wstring edge = this->_texts[iter->second->text_id - _oldest_text_id].
							substr(iter->second->start, iter->second->end - iter->second->start);
			wcout << "|-" << edge << "[" << iter->second->text_id << " " << iter->second->start << ":"
							<< iter->second->end << "] " << " (" << iter->second->freq << " " << iter->second->length << ") "
							<< iter->second << "->" << iter->second->suffix_link
							//                    << " " << (iter->second->next == NULL) 
							<< endl;

			//              wcout << "|-"  << " "<< "[" << iter->second->id << " " << iter->second->start << ":"
			//                    << iter->second->end << "] " << iter->second << "->" << iter->second->suffix_link
			//                    << " " << (iter->second->next==NULL) << endl;
			print(iter->second, depth + 1, leaf_freq);
		}
	}


};
}
#endif  //----end of SUFFIX_TREE_H_

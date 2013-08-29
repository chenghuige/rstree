/** 
 *  ==============================================================================
 * 
 *          \file   dsuffix_tree.h
 *
 *        \author   chenghuige  
 *          
 *          \date   2013-08-20 22:03:00.758982
 *  
 *  \Description:   TODO  ��ǰ������  ��� Ƶ����ͬ�ű���������������׺������û�е��� ��Ȼ����Ҫ �ж� Ҷ�ӵ�root��ǰ׺������ϵ
 *                           ������Ҫ���������� Ҷ�ӱ��������� ��ô �̵����ϵ��ڲ��ڵ� �Ͳ���Ҫ��
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

    int _max_substr_cnt; //������ظ��Ӵ������������
    int _min_substr_len;
    int _max_substr_len;
    int _min_frequency;
    int _tree_size; //���Ĵ�С����

    int _min_str_len; //�����ı���̳�������
    int _max_str_len; //�����ı���󳤶�����

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
     * @brief: ���ַ������ߵĿհ��ַ�������
     * @param s: �������ַ���
     * @retval: ���к���ַ���
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
    //��ȡ���㳤�Ⱥ�Ƶ�����Ƶ������Ӵ���˼·�����������Ӵ��Ѿ�add ����Generalized��׺��
    //����Ҷ�ӽ�� ���ұ������е�Ҷ�ӵ�����
    //����Ҷ�ӽ��Ĺ��̻�������е��ڲ���� ����suffix link ��ô���Լ�¼�������ȵ��ڲ����
    //��Чָ����Ϊ������ϵ��Ч ���� a--bcd -> bcd ����һ���ڲ����ͨ��suffix link��ת ���Ƶ�β��� ��ôֻ����ǰ�泤�ļ���
    //TODO  �Ƿ���Ը�����󳤶����� ���м�֦�� ��ǰЧ�� O(n*log(n))

    void find_substr(const wstring& text, vector<SubStrFreq>& result_vec)
    {
        Node* leaf_node;
        int pre_leaf_freq = 0;
        std::tr1::unordered_map < Node*, bool> internal_nodes_map;
        typedef std::tr1::unordered_map < Node*, bool>::iterator Iter;
        //map< Node*, bool> internal_nodes_map;
        //typedef map < Node*, bool>::iterator Iter;
        for (leaf_node = _first_leafs[_current_text_id - _oldest_text_id]; leaf_node != NULL; leaf_node = leaf_node->suffix_link)
        {
            //Ҷ�ӽ�㴦��
            if (leaf_node->freq > pre_leaf_freq
                    && IsLeafOk(leaf_node)
                    )
            {//���ڽڵ㳤�ȳ��� ������һ�ڵ㳤�Ȳ���Ŀ��� ������ �����ڱ��ϴ���ĳ��λ�ó���ok ���ֲ����� 
                wstring substr = text.substr(leaf_node->end - leaf_node->length, leaf_node->length - 1);
                //                    wcout << "Push leaf: " << substr << endl;
                result_vec.push_back(SubStrFreq(substr, leaf_node->freq));
            }

            for (Node* node = leaf_node->parent; node != _root; node = node->parent)
            {
                internal_nodes_map[node] = true;
            }

            pre_leaf_freq = leaf_node->freq;

        }

        for (Iter iter = internal_nodes_map.begin(); iter != internal_nodes_map.end(); ++iter)
        {
            Node* node = iter->first;
            Node* shorter_node = node->suffix_link;

            if (shorter_node != _root && node->freq == shorter_node->freq)
            {//Ƶ����ͬ�Һ�׺���� ��   �����йش�(4)  �йش�(4) ֻ���� �����йش壬ǰ׺��������Ҫ���ǣ���׺�������Զ����� TODO can modify
                //���� �����йش�(3)  �йش�(4) �����йش�
                //����һ�ֲ���������ٶ� Ƶ�������� 3  ��ô �������������Ĵ����а�����ϵ ��������  �����йش�(3)  �йش�(4)  ���� �����йش�
                //��������ɸ�Ϊ node->freq >= min_freq_
                internal_nodes_map[shorter_node] = false;
                //                wcout << node << " " << shorter_node << endl;
                //                wcout << "Filtered shorter suffix " << "[ " << text.substr(shorter_node->end - shorter_node->length, shorter_node->length)
                //                        << " for " << text.substr(node->end - node->length, node->length) << " ]" << endl;
            }
        }

        for (Iter iter = internal_nodes_map.begin(); iter != internal_nodes_map.end(); ++iter)
        {
            Node* node = iter->first;
            if (iter->second == true)
            {
                //                 wcout << "The internal node is " << text.substr(node->start, node->end - node->start) << " of "
                //                        << text.substr(node->end - node->length, node->length)
                //                        << "[ " << node->start << " : " << node->end << " ]" << endl;
                //                 wcout << node->freq << " " << node->length << " " << node->parent->length << endl;
                //                 wcout << min_frequency_ << " " << min_substr_len_ << " " << max_substr_len_ << endl;
                if (IsNodeOk(node))
                {
                    wstring substr = text.substr(node->end - node->length, node->length);
                    //wcout << "Push internal: " << substr << endl;
                    result_vec.push_back(SubStrFreq(substr, node->freq));
                }
            }
        }

    }

};


#endif  //__DSUFFIX_TREE_H_

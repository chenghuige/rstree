/***************************************************************************
 * 
 * Copyright (c) 2013 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file rstree.h
 * @author weizheng(com@baidu.com)
 * @date 2013/05/06 17:49:16
 * @brief 
 *  
 **/




#ifndef  __RSTREE_H_
#define  __RSTREE_H_


#include <map>
#include <string>
#include <ext/malloc_allocator.h>
#include <ext/pool_allocator.h>
#include "rstree_def.h"

using namespace std;


class suffix_edge_t;

typedef map<wchar_t, suffix_edge_t*, less<wchar_t>, 
      __gnu_cxx::__pool_alloc<pair<wchar_t, suffix_edge_t*> > > rstree_map_t;


/*
 * ��׺���Ľڵ�
 */
class suffix_node_t
{
public:
    suffix_node_t * suffix_link; //��׺����
    suffix_node_t * parent_node; //���ڵ�
    rstree_map_t  edge_map; //���еı�
    int frequency; //��root����ǰ�ڵ���Ӵ���Ƶ��
    int length; //��root����ǰ�ڵ���Ӵ��ĳ���
    suffix_edge_t * parent_edge; //���ױ�

    suffix_node_t(suffix_node_t* parent_node)
    {
        suffix_link = NULL;
        this->parent_node = parent_node;
        frequency = 0;
        length = -1;        
    }
    suffix_node_t()
    {
        suffix_link = NULL;
        parent_node = NULL;
        frequency = 0;
        length = -1;
    }

    ~suffix_node_t()
    {
        rstree_map_t().swap(edge_map);
    }

    void set_parent_edge(suffix_edge_t * parent_edge)
    {
        this->parent_edge = parent_edge;
    }

private:


};

/**
 * ��׺���ı�
 */
class suffix_edge_t
{
public:
    int text_id; //�������������ı�id
    suffix_node_t * next; //�������ӵ��ӽڵ�
    int start; //�����ı��еĿ�ʼindex
    int end; //�����ı��еĽ���index
    wchar_t  key; //�����������ı��ĵ�һ����

    suffix_edge_t(int text_id, int start, int end, wchar_t key, suffix_node_t *next)
    {
        this->text_id = text_id;
        this->start = start;
        this->end = end;
        this->key = key;
        this->next = next;
    } 

    void set_text_id(int text_id)
    {
        this->text_id = text_id;
    }

};

class rstree_t
{
public:
    rstree_t()
    {
        min_frequency = DEFAULT_MIN_FREQUENCY;
        min_substr_len = DEFAULT_MIN_SUBSTR_LEN;
        max_substr_len = DEFAULT_MAX_SUBSTR_LEN;
        max_substr_cnt = DEFAULT_MAX_SUBSTR_CNT;
        max_str_len = DEFAULT_MAX_STR_LEN;
        min_str_len = DEFAULT_MIN_STR_LEN;
        tree_size = DEFAULT_TREE_SIZE;
        end_string = L"\n";

        text_id = 0;
        remove_id = 0;

        root = NULL;
        seed = NULL;
    }

    ~rstree_t()
    {
        free_tree();
    }

    void set_min_frequency(int freq)
    {
        min_frequency = freq;
    }

    void set_min_substr_len(int len)
    {
        min_substr_len = len;
    }

    int get_min_substr_len()
    {
        return min_substr_len;
    }

    void set_max_substr_len(int len)
    {
        max_substr_len = len;
    }

    int get_max_substr_len()
    {
        return max_substr_len;

    }

    void set_min_str_len(int len)
    {
        min_str_len = len;
    }

    int get_min_str_len()
    {
        return min_str_len;
    }

    void set_max_str_len(int len)
    {
        max_str_len = len;
    }

    int get_max_str_len()
    {
        return max_str_len;
    }

    void set_max_substr_cnt(int cnt)
    {
        max_substr_cnt = cnt;
    }

    int get_max_substr_cnt()
    {
        return max_substr_cnt;
    }

    void set_end_string(const wstring& s)
    {
        end_string = s;
    }

    void set_tree_size(int size)
    {
        tree_size = size;
    }

    size_t get_tree_size()
    {
        return text_map.size();
    }

    size_t get_max_tree_size()
    {
        return tree_size;
    }

    map<string, int> get_substr_freq();

    void print_tree();

    map<wstring, int> add_text(wstring text);

    int remove_text();

    void free_tree();


private:
    map<long long, wstring> text_map;
    suffix_node_t * root;
    suffix_node_t * seed;

    int min_frequency; //�����ظ��Ӵ�����СƵ������
    int min_substr_len; //�����ظ��Ӵ���̳�������
    int max_substr_len; //�����ظ��Ӵ���󳤶�����
    int tree_size; //���Ĵ�С����
    long long text_id; //��ǰ���ı�id
    long long remove_id; //��ǰ��ɵ��ı�id
 
    int min_str_len; //�����ı���̳�������
    int max_str_len; //�����ı���󳤶�����
    int max_substr_cnt; //������ظ��Ӵ������������

    wstring end_string; //

    map<wstring, int> inc_freq(suffix_node_t *lowest_node, int id, int start);
    wstring inc_freq_recursive(map<wstring, int> & ret, suffix_node_t * node, suffix_node_t * lowest_node, int id, int start, bool up);
    void dec_freq(suffix_node_t * lowest_node, int &cnt);

    void dfs_print(int level, suffix_node_t *node);

    void filter_result(map<wstring, int> & m);

    map<wstring, int> build_tree(const wstring & text);

    void free_tree_recursive(int level, suffix_node_t *node);

};




#endif  //__RSTREE_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */

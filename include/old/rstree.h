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

/**
 * ��׺���ڵ��б���ߵ�map��ʹ��gnu_cxx��pool_allocator��Ϊ�ڴ������
 */
typedef map<wchar_t, suffix_edge_t*, less<wchar_t>, 
      __gnu_cxx::__pool_alloc<pair<wchar_t, suffix_edge_t*> > > rstree_map_t;


/*
 * ��׺���Ľڵ�
 */
struct suffix_node_t
{
    suffix_node_t * suffix_link; //��׺����
    suffix_node_t * parent_node; //���ڵ�
    rstree_map_t*  edge_map; //���еı�
    int frequency; //��root����ǰ�ڵ���Ӵ���Ƶ��
    int length; //��root����ǰ�ڵ���Ӵ��ĳ���
    suffix_edge_t * parent_edge; //���ױ�

};

/**
 * @brief: ������׺���ڵ�
 * @param: ��ǰ�ڵ�ĸ��ڵ�
 * @retval: �����ĺ�׺�ڵ�ָ��
 *
 */
suffix_node_t * construct_suffix_node(suffix_node_t *);


/**
 * �ͷź�׺���ڵ�
 *
 */
void free_suffix_node(suffix_node_t *);

/**
 * ��׺���ı�
 */
struct suffix_edge_t
{
    long long text_id; //�������������ı�id
    suffix_node_t * next; //�������ӵ��ӽڵ�
    int start; //�����ı��еĿ�ʼindex
    int end; //�����ı��еĽ���index
    wchar_t  key; //�����������ı��ĵ�һ����

};

/**
 * @brief: ������׺���ı�
 * @param text_id: �������ı���id
 * @param start: �ߵĵ�һ���ַ����ı��е�index
 * @param end: �ߵ����һ���ַ����ı��е�index
 * @param key: �ߵĵ�һ���ַ�����Ϊ���������ߵ�map��key
 * @param next: �������ӵĺ��ӽڵ�
 * @retval: �����ĺ�׺����
 *
 */
suffix_edge_t * construct_suffix_edge(long long text_id, int start, int end, wchar_t key, suffix_node_t *next);

/**
 * �ͷź�׺����
 *
 */
void free_suffix_edge(suffix_edge_t *);

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

    map<wstring, int> inc_freq(suffix_node_t *lowest_node, long long id, int start);
    wstring inc_freq_recursive(map<wstring, int> & ret, suffix_node_t * node, suffix_node_t * lowest_node, long long id, int start, bool up);
    void dec_freq(suffix_node_t * lowest_node);

    void dfs_print(int level, suffix_node_t *node);

    void filter_result(map<wstring, int> & m);

    map<wstring, int> build_tree(const wstring & text);

    void free_tree_recursive(int level, suffix_node_t *node);

};




#endif  //__RSTREE_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */

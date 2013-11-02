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
 * 后缀树节点中保存边的map，使用gnu_cxx的pool_allocator作为内存分配器
 */
typedef map<wchar_t, suffix_edge_t*, less<wchar_t>, 
      __gnu_cxx::__pool_alloc<pair<wchar_t, suffix_edge_t*> > > rstree_map_t;


/*
 * 后缀树的节点
 */
struct suffix_node_t
{
    suffix_node_t * suffix_link; //后缀链接
    suffix_node_t * parent_node; //父节点
    rstree_map_t*  edge_map; //所有的边
    int frequency; //从root到当前节点的子串的频率
    int length; //从root到当前节点的子串的长度
    suffix_edge_t * parent_edge; //父亲边

};

/**
 * @brief: 构建后缀树节点
 * @param: 当前节点的父节点
 * @retval: 构建的后缀节点指针
 *
 */
suffix_node_t * construct_suffix_node(suffix_node_t *);


/**
 * 释放后缀树节点
 *
 */
void free_suffix_node(suffix_node_t *);

/**
 * 后缀树的边
 */
struct suffix_edge_t
{
    long long text_id; //这条边所属的文本id
    suffix_node_t * next; //边所连接的子节点
    int start; //边在文本中的开始index
    int end; //边在文本中的结束index
    wchar_t  key; //边所包含的文本的第一个字

};

/**
 * @brief: 构建后缀树的边
 * @param text_id: 边所在文本的id
 * @param start: 边的第一个字符在文本中的index
 * @param end: 边的最后一个字符在文本中的index
 * @param key: 边的第一个字符，作为保存这条边的map的key
 * @param next: 边所连接的孩子节点
 * @retval: 构建的后缀树边
 *
 */
suffix_edge_t * construct_suffix_edge(long long text_id, int start, int end, wchar_t key, suffix_node_t *next);

/**
 * 释放后缀树边
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

    int min_frequency; //请求重复子串的最小频率限制
    int min_substr_len; //请求重复子串最短长度限制
    int max_substr_len; //请求重复子串最大长度限制
    int tree_size; //树的大小限制
    long long text_id; //当前的文本id
    long long remove_id; //当前最旧的文本id
 
    int min_str_len; //建树文本最短长度限制
    int max_str_len; //建树文本最大长度限制
    int max_substr_cnt; //请求的重复子串最大数量限制

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

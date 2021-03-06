/***************************************************************************
 * 
 * Copyright (c) 2013 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file rstree.cpp
 * @author weizheng(com@baidu.com)
 * @date 2013/05/06 19:00:06
 * @brief 后缀树操作，提供了建树，添加文本，删除文本，打印等功能
 *  
 **/

#include "rstree.h"
#include "rstree_util.h"
#include <iostream>
#include <vector>

#include <ub.h>

#include <boost/pool/pool.hpp>

//内存池，用来提升内存反复申请和释放的性能
boost::pool<> g_node_mempool(sizeof(suffix_node_t));
boost::pool<> g_edge_mempool(sizeof(suffix_edge_t));

/**
 * @brief: 构建后缀树节点
 * @param: 当前节点的父节点
 * @retval: 构建的后缀节点指针
 *
 */
suffix_node_t * construct_suffix_node(suffix_node_t * parent_node)
{
    suffix_node_t * p_node = (suffix_node_t *) g_node_mempool.malloc();
    p_node->suffix_link = NULL;
    p_node->parent_node = parent_node;
    p_node->frequency = 0;
    p_node->length = -1;

    p_node->edge_map = new rstree_map_t;

    return p_node;
}

/**
 * 释放后缀树节点
 *
 */
void free_suffix_node(suffix_node_t * p)
{
    delete p->edge_map;
    g_node_mempool.free(p);
}

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
suffix_edge_t * construct_suffix_edge(long long text_id, int start, int end, wchar_t key, suffix_node_t *next)
{
    suffix_edge_t * p_edge = (suffix_edge_t *) g_edge_mempool.malloc();
    p_edge->text_id = text_id;
    p_edge->start = start;
    p_edge->end = end;
    p_edge->key = key;
    p_edge->next = next;

    return p_edge;
}

/**
 * 释放后缀树边
 *
 */
void free_suffix_edge(suffix_edge_t *p)
{
    g_edge_mempool.free(p);
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
    while(start < (int)s.size() && (s[start] == L' ' || s[start] == L'\t' || s[start] == L'\n' || s[start] == L'\r'))
        start++;
    while(end >= 0 && (s[end] == L' ' || s[end] == L'\t' || s[end] == L'\n' || s[end] == L'\r'))
        end--;
    if(start > end)
        return L"";
    return s.substr(start, end - start + 1);
}

/*
 * @brief: 合并map，将m2中的key-value对合并入m1，当m2中不包含这个key，或者对应的value值比m1中的大时
 *
 */
void merge_map(map<wstring, int> & m1, const map<wstring ,int> & m2)
{
    for(map<wstring, int>::const_iterator iter = m2.begin(); iter != m2.end(); iter++)
    {
        map<wstring, int>::iterator iter1 = m1.find(iter->first);
        if(iter1 == m1.end() || iter1->second < iter->second)
        {
            m1[iter->first] = iter->second;
        }
    }
}

/**
 * @brief: 打印树
 *
 */
void rstree_t::print_tree()
{
    wstring tree_string = L"";
    for(map<long long, wstring>::iterator iter = text_map.begin(); iter != text_map.end(); iter++)
    {
        tree_string += iter->second;
    }
    wcout << L"suffix tree for string[" << tree_string << L"]" << endl;
    wcout << L"-----------------------------------------------" << endl;

    if(root != NULL)
    {
        wcout << L"root=" << root << endl;
        dfs_print(0, root);
    }
    else
    {
        wcout << L"NULL" << endl;
    }
}

/**
 * @brief: 递归打印树
 * @param level: 当前节点的树高
 * @param node: 当前节点
 *
 */
void rstree_t::dfs_print(int level, suffix_node_t * node)
{
    rstree_map_t  m = *(node->edge_map);

    for(rstree_map_t::iterator iter = m.begin(); iter != m.end(); iter++)
    {
        suffix_edge_t *gt = iter->second;
        wstring src_text = text_map[gt->text_id];
        if(gt->start == (int)src_text.size() - 1 && gt->end == (int)src_text.size())
        {
            continue;
        }
        for(int i = 0; i < level; i++)
        {
            wcout << L"   ";
        }
        wcout << L"|-";

        for(int i = gt->start; i <= gt->end && i < (int)src_text.size() - 1; i++)
        {
            wcout << src_text[i];
        }
        wcout << L"[" << gt->text_id << L" " << gt->start << L":"  << gt->end <<  L"]";
        wcout << L" " << gt->next->frequency;
        wcout << L" " << (gt->next) << L"->" << (gt->next->suffix_link);

        wcout << endl;
        dfs_print(level + 1, gt->next);
    }

}

/**
 * @brief: 递归释放，包括删除边以及节点
 * @param level: 当前节点的树高
 * @param node: 当前节点
 *
 */
void rstree_t::free_tree_recursive(int level, suffix_node_t * node)
{
    rstree_map_t  m = *(node->edge_map);
    for(rstree_map_t::iterator iter = m.begin(); iter != m.end(); iter++)
    {
        suffix_edge_t *gt = iter->second;
        free_tree_recursive(level + 1, gt->next);
        //delete gt;
        g_edge_mempool.free(gt);
    }
    //delete node;
    g_node_mempool.free(node);

}

/**
 * @brief: 释放树
 *
 */
void rstree_t::free_tree()
{
    free_tree_recursive(0, root);
    text_map.clear();
    text_id = remove_id = 0;
}

/**
 * @brief: 从当前节点开始，递归向上，对父亲节点的频率做加一操作
 * @param ret: 保存所有满足条件的子串及其频率
 * @param node: 当前节点
 * @param lowest_node: 第一次调用时的最低节点
 * @param id: 当前所添加文本的id
 * @param start: 当前节点在文本中start index
 * @param up: 是否本次增加的节点已经属于当前的文本id
 * @retval: 本次操作所得到的满足条件的文本 
 *
 */
wstring rstree_t::inc_freq_recursive(map<wstring, int> & ret, suffix_node_t *node, suffix_node_t *lowest_node, long long id, int start, bool up)
{
    if(node == root)
    {
        return L"";
    }
    if(id >= 0)
    {
        if(id == node->parent_edge->text_id)
        {
            start = node->parent_edge->start;
            up = true;
        }
        else
        {
            int length = min(node->length, node->parent_edge->end - node->parent_edge->start + 1);
            if(length <= 0)
            {
                int t_end = node->parent_edge->end;
                wstring & src_text = text_map[node->parent_edge->text_id];
                if(t_end >= (int)src_text.size())
                {
                    t_end = (int)src_text.size();
                }
                length = t_end - node->parent_edge->start;
            }

            if(!up)
            {
                node->parent_edge->text_id = id;
                if(node == lowest_node)
                {
                    node->parent_edge->start = start;
                    node->parent_edge->end = start + length + 1;

                }

                up = true;
            }
            else
            {
                node->parent_edge->text_id = id;
                start -= length;
                node->parent_edge->start = start;
                node->parent_edge->end = start + length - 1;

            }
        }
    }

    node->frequency++;

    int end_index = node->parent_edge->end + 1;
    int start_index = node->parent_edge->start;
    wstring & src = text_map[node->parent_edge->text_id];
    if(end_index >= (int)src.size())
    {
        end_index = src.size();
    }


    wstring cur_str = trim(src.substr(start_index, end_index - start_index));

    wstring ret_str = inc_freq_recursive(ret, node->parent_node, lowest_node, id, start_index, up) + cur_str;

    if(node->frequency >= this->min_frequency && (int)ret_str.size() >= this->min_substr_len && (int)ret_str.size() <= this->max_substr_len)
    {
        map<wstring, int>::iterator iter = ret.find(ret_str);
        if(iter == ret.end() || iter->second < node->frequency)
        {
            UB_LOG_DEBUG("add substring [%s] freq[%d]" , w2c(ret_str.c_str()).c_str(), node->frequency);
            ret[ret_str] = node->frequency;
        }
    }

    return ret_str;
} 

/**
 * @brief: 排序所用的比较函数
 *
 */
bool freq_map_comparator_inc(const pair<wstring, int> &p1, const pair<wstring, int> &p2)
{
    return p1.first.size() < p2.first.size();
}

/**
 * @brief: 排序所用的比较函数
 *
 */
bool freq_map_comparator_dec(const pair<wstring, int> &p1, const pair<wstring, int> &p2)
{
    return p1.first.size() > p2.first.size();
}


/**
 * @brief: 从当前节点开始，递归向上，对父亲节点的频率做加一操作
 * @param lowest_node: 当前节点
 * @param id: 当前所添加文本的id
 * @param start: 当前节点在文本中start index
 * @retval: 本次操作所得到的满足条件的文本及其频率 
 *
*/
map<wstring, int> rstree_t::inc_freq(suffix_node_t * lowest_node, long long id, int start)
{
    suffix_node_t *node = lowest_node;
    bool up = false;
    map<wstring, int> freq_map;
    inc_freq_recursive(freq_map, node, lowest_node, id, start, up);
    filter_result(freq_map);
    map<wstring, int> ret;
    
    vector< pair<wstring,int> > vec(freq_map.begin(), freq_map.end());
    sort(vec.begin(), vec.end(), freq_map_comparator_dec);
    for(int i = 0; i < (int)vec.size(); i++)
    {
        if(vec[i].second >= this->min_frequency)
        {
            map<wstring, int>::iterator iter = ret.find(vec[i].first);
            if(iter == ret.end() || iter->second < vec[i].second)
            {
                ret[ vec[i].first ] = vec[i].second;
            }
        //    break;
        }
    }

    return ret;

}

/**
 * @brief: 建树
 * @param text: 建树所用文本
 * @retval: 满足条件的子串及其频率
 *
 */
map<wstring, int> rstree_t::build_tree(const wstring & text)
{
    map<wstring, int> ret;

    int length = (int)text.size();

    root = construct_suffix_node(NULL);
    root->length = 0;

    seed = construct_suffix_node(NULL);
    root->suffix_link = seed;

    suffix_node_t *s = root;
    int k = 0;
    suffix_edge_t *tag = construct_suffix_edge(-1, -2, -2, L'0', root);

    for(int i = 0; i < length; i++)
    {
        wchar_t t = text[i];
        if(this->seed->edge_map->count(t) == 0)
        {
            (*(this->seed->edge_map))[t] = tag;
        }
    
        suffix_node_t * oldr = this->root;
        while(true)
        {
            while(i > k)
            {
                suffix_edge_t *tt = (*(s->edge_map))[text[k]];
                int kk = tt->start;
                int pp = tt->end;
                suffix_node_t *ss = tt->next;
                if (pp - kk < i - k) {
                    k = k + pp - kk + 1;
                    s = ss;
                } 
                else 
                {
                    break;
                }
            }

            suffix_node_t *r;
            if(i > k)
            {
                wchar_t tk = text[k];
                suffix_edge_t *tt = (*(s->edge_map))[tk];
                int kp = tt->start;
                int pp = tt->end;
                suffix_node_t *sp = tt->next;
                if ( t == text[kp + i - k])
                {
                    break;
                }
                else
                {
                    r = construct_suffix_node(s);
                    sp->parent_node = r;
                    int j = kp + i - k;
                    wchar_t tj = text[j];

                    suffix_edge_t *rg = construct_suffix_edge(0, j, pp, tj, sp);
                    (*(r->edge_map))[tj] = rg;
                    sp->parent_edge = rg;

                    suffix_edge_t *sg = construct_suffix_edge(0, kp, j-1, text[kp], r);
                    (*(s->edge_map))[ text[kp] ] = sg;
                    r->parent_edge = sg;
                    r->frequency = sp->frequency;
                    r->length = s->length + j - kp;

                }
            }
            else if(s->edge_map->count(t) > 0)
            {
                break;
            }
            else
            {
                r = s;
            }

            suffix_node_t *tmp = construct_suffix_node(r);

            suffix_edge_t *rg = construct_suffix_edge(0, i, length, t, tmp);
            (*(r->edge_map))[t] = rg;
            tmp->parent_edge = rg;

            map<wstring, int>  t_ret = inc_freq(tmp, -1, 0);
            merge_map(ret, t_ret);

            if(oldr != root)
            {
                oldr->suffix_link = r;
            }
            oldr = r;
            s = s->suffix_link;

        }

        if(oldr != root)
        {
            oldr->suffix_link = s;
        }
    }

    return ret;

}

/**
 * @brief: 替换字符串操作
 * @param str: 原始字符串
 * @param old_value: 被替换的子串
 * @param new_value: 新的子串
 * @retval: 替换后的字符串
 *
 */
wstring wstr_replace_all(wstring& str,const wstring& old_value,const wstring& new_value)
{
    while(true) {
        wstring::size_type pos(0);
        if( (pos=str.find(old_value))!=wstring::npos )
            str.replace(pos,old_value.length(),new_value);
        else break;
    }
    return str;
}

/**
 * @brief: 对子串结果做过滤，如果子串a的频率小于等于b，且a是b的子串，则把a过滤
 * @param m: 原始子串的map
 *
 */
void rstree_t::filter_result(map<wstring, int> & m)
{
    UB_LOG_DEBUG("before filter map size=[%d]", (int)m.size());
    vector< pair<wstring, int> > vec(m.begin(), m.end());
    sort(vec.begin(), vec.end(), freq_map_comparator_inc);

    if((int)vec.size() > get_max_substr_cnt())
    {
        int pre_size = (int)vec.size();
        vec.erase(vec.begin() + get_max_substr_cnt(), vec.end());
        m = map<wstring, int>(vec.begin(), vec.end());
        UB_LOG_DEBUG("remove substr from map, before size=[%d] after size=[%d]", pre_size, (int)vec.size());
    }


    int n = (int)vec.size();
    for(int i = 0; i < n - 1; i++)
    {
        for(int j = i+1; j < n; j++)
        {
            if(vec[j].first.find(vec[i].first) != wstring::npos && vec[i].second <= vec[j].second)
            {
                UB_LOG_DEBUG("remove key [%s] freq [%d] longer key [%s] freq [%d]", w2c(vec[i].first.c_str()).c_str(), vec[i].second, w2c(vec[j].first.c_str()).c_str(), vec[j].second );
                m.erase(vec[i].first);
                break;
            }
        }
    }

    UB_LOG_DEBUG("after filter map size=[%d]", (int)m.size());
}

/**
 * @brief: 增加字符串
 * @param min_freq: 最低频率限制
 * @param min_substr_len: 最短子串长度限制
 * @param max_substr_len: 最大子串长度限制
 * @retval: 满足上述三个条件的子串及其频率
 *
 */
map<wstring, int> rstree_t::add_text(wstring text)
{

    UB_LOG_DEBUG("begin add text [%s]", w2c(text.c_str()).c_str());

    map<wstring, int> ret;
    if( text.size() == 0)
    {
        return ret;
    }

    text = wstr_replace_all(text, end_string, L"");
    text += end_string;
    long long current_text_id = text_id;
    text_map[text_id++] = text;

    if(root == NULL)
    {
        map<wstring, int> t_ret = build_tree(text);
        merge_map(ret, t_ret);
        return ret;
    }

    int length = (int)text.size();
    suffix_node_t *u = root;
    int index = 0;


    while(index < length - 1)
    {
        bool flag = false;
        suffix_node_t * oldr = root;
        while(true)
        {
            suffix_node_t *r = u;
            bool split = false;
            int temp_index = -1;
            int matched_char_count = 0;

            while(u->edge_map->count(text[index]) > 0)
            {
                temp_index = index;

                suffix_edge_t * tt = (*(u->edge_map))[ text[index] ];
                wstring src_text = text_map[tt->text_id];
                int i = tt->start;

                int tt_end_bk = tt->end;
                suffix_node_t * tt_next_bk = tt->next;

                while( i <= tt->end && index < (int)text.size() )
                {

                    if(text[index] == src_text[i])
                    {
                        index++;
                        matched_char_count++;
                        i++;
                    }
                    else
                    {
                        suffix_node_t * v = tt->next;
                        r = construct_suffix_node(u);
                        v->parent_node = r;

                        suffix_edge_t * rg = construct_suffix_edge(tt->text_id, i, tt->end, src_text[i], v);
                        (*(r->edge_map))[ src_text[i] ] = rg;
                        v->parent_edge = rg;

                        suffix_edge_t * ug = construct_suffix_edge(tt->text_id, tt->start, i - 1, src_text[tt->start], r);

                        (*(u->edge_map))[ src_text[tt->start] ] = ug;
                        r->parent_edge = ug;
                        r->frequency = v->frequency;
                        r->length = u->length + i - tt->start;
                        split = true;

                        if(oldr != root && oldr->length == r->length + 1)
                        {
                            oldr->suffix_link = r;
                            oldr = root;
                        }

                        if(NULL != tt)
                        {

                            //delete tt;
                            free_suffix_edge(tt);
                            tt = NULL;
                        }

                        break;
                    }
                }

                if(index >= (int)text.size())
                {
                    map<wstring, int> t_ret = inc_freq(tt_next_bk, current_text_id, temp_index);

                    merge_map(ret, t_ret);
                }

                if(i > tt_end_bk && index < (int)text.size())
                {
                    temp_index = -1;
                    u = tt_next_bk;
                    r = u;

                    if(oldr != root && oldr->length == u->length + 1)
                    {
                        oldr->suffix_link = u;
                        oldr = root;
                    }
                }
                else
                {
                    break;
                }
            }


            if(index >= (int)text.size())
            {
                if(u != root && temp_index < (int)text.size())
                {
                    u = u->suffix_link;
                    if(temp_index != -1)
                    {
                        index = temp_index;
                    }
                    continue;
                }
                else if(u == root && temp_index + 1 < (int)text.size())
                {
                    index = temp_index + 1;
                    continue;
                }
                else
                {
                    flag = true;
                    break;
                }
            }
            
            suffix_node_t * tmp = construct_suffix_node(r);

            suffix_edge_t * rg = construct_suffix_edge(current_text_id, index, length, text[index], tmp);
            (*(r->edge_map))[ text[index] ] = rg;
            tmp->parent_edge = rg;
            map<wstring, int> t_ret = inc_freq(tmp, current_text_id, index);

            merge_map(ret, t_ret);

            if(r == root)
            {
                index++;
                break;
            }

            if(oldr != root)
            {
                oldr->suffix_link = r;
            }

            if(u == root && matched_char_count == 1)
            {
                r->suffix_link = root;
                oldr = root;
                break;
            }

            if(u != root)
            {
                u = u->suffix_link;
                if(temp_index != -1)
                {
                    index = temp_index;
                }
            }
            else
            {
                if(temp_index != -1 && temp_index + 1 < (int)text.size())
                {
                    index = temp_index + 1;
                }
            }

            oldr = (!split) ? root : r;

        }
        
        if(oldr != root)
        {
            oldr->suffix_link = u;
        }

        if(flag)
        {
            break;
        }

    }

    filter_result(ret);

    return ret;

}

/**
 * @brief: 从当前节点开始，将其父节点频率减一，直至root
 * @param lowest_node: 当前节点
 */
void rstree_t::dec_freq(suffix_node_t * lowest_node )
{
    suffix_node_t * node = lowest_node;
    while(node != root)
    {
        if(node->frequency > 1)
        {
            node->frequency--;
        }
        else if(node->frequency == 1)
        {
            node->frequency = 0;
            node->parent_node->edge_map->erase(node->parent_edge->key);
        }
        suffix_node_t *tmp = node;
        node = node->parent_node;

        if(tmp->frequency == 0)
        {
           //delete tmp->parent_edge;
           free_suffix_edge(tmp->parent_edge);
           tmp->parent_edge = NULL;
           //delete tmp;
           free_suffix_node(tmp);
           tmp = NULL;

        }
    }
}

/**
 * @brief: 删除最旧的字符串
 *
 */
int rstree_t::remove_text()
{
    map<long long, wstring>::iterator iter = text_map.find(remove_id);
    if(iter == text_map.end() || root == NULL)
    {
        return -1;
    }

    wstring text = iter->second;
    UB_LOG_DEBUG("begin remove text [%s]", w2c(text.c_str()).c_str());

    int length = text.size();
    suffix_node_t *u = root;
    suffix_node_t *us = u->suffix_link;
    int index = 0;

    while(index < length)
    {
        bool flag = false;

        while(true)
        {
            int temp_index = -1;

            while(u->edge_map->find(text[index]) != u->edge_map->end())
            {

                temp_index = index;

                suffix_edge_t * tt = (*(u->edge_map))[ text[index] ];
                wstring src_text = text_map[tt->text_id];
                int i = tt->start;

                /*
                while(i <= tt->end && index < (int)text.size())
                {
                    inner_cnt_4++;

                    if(text[index] == src_text[i])
                    {
                        index++;
                        i++;
                    }
                    else
                    {
                        return -2;
                    }
                }

                */

                i += tt->end - tt->start + 1;
                index += tt->end - tt->start + 1;

                int tt_end_bk = tt->end;
                if(index >= (int)text.size())
                {
                    dec_freq(tt->next);
                }


                if(i > tt_end_bk && index < (int)text.size())
                {
                    temp_index = -1;
                    u = tt->next;
                    us = u->suffix_link;
                }
                else
                {
                    break;
                }
            }


            if(index >= (int)text.size())
            {
                if(u != root && temp_index < (int)text.size())
                {
                    u = us;
                    us = u->suffix_link;
                    if(temp_index != -1)
                    {
                        index = temp_index;
                    }
                    continue;
                }
                else if(u == root && temp_index + 1 < (int)text.size())
                {
                    index = temp_index + 1;
                    continue;
                }
                else
                {
                    flag = true;
                    break;
                }
            }
            else if(index == (int)text.size() - 1 && u->edge_map->count(text[index]) == 0)
            {
                flag = true;
                break;
            }
        }

        if(flag)
        {
            break;
        }
    }

    text_map.erase(remove_id++);

    return 0;
}



/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */

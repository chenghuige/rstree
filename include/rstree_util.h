/***************************************************************************
 * 
 * Copyright (c) 2013 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file include/rstree_util.h
 * @author weizheng(com@baidu.com)
 * @date 2013/06/14 12:05:32
 * @brief 
 *  
 **/




#ifndef  __RSTREE_UTIL_H_
#define  __RSTREE_UTIL_H_



#include <string>

using namespace std;


/**
 * @brief 字符串转化成宽string
 *
 */
wstring c2w(const char* pc);


/**
 * @brief 将宽字符串转化为string 
 *
 */
string w2c(const wchar_t *pw);


/**
 * @brief: 替换字符串操作
 * @param str: 原始字符串
 * @param old_value: 被替换的子串
 * @param new_value: 新的子串
 * @retval: 替换后的字符串
 *
 */
string str_replace_all(const string& tstr, const string& old_value,const string& new_value);


/**
 * @brief: 同上面的函数
 *
 */
wstring wstr_replace_all(const wstring& tstr,const wstring& old_value,const wstring& new_value);



#endif  //__INCLUDE/RSTREE_UTIL_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */

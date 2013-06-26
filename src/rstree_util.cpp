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


#include "rstree_util.h"
#include "rstree_def.h"


/**
 * @brief 字符串转化成宽string
 *
 */
wstring c2w(const char* pc)
{
    wstring val = L"";
    if(NULL == pc)
    {
        return val;
    }
    size_t size_of_wc;
    size_t destlen = mbstowcs(0,pc,0);
    if(destlen == (size_t)-1)
    {
        return val;
    }
    size_of_wc = destlen+1; 
    wchar_t  pw[DEFAULT_MAX_STR_LEN];;
    mbstowcs(pw,pc,DEFAULT_MAX_STR_LEN); 
    val = pw;  
    return val;
}


/**
 * @brief 将宽字符串转化为string 
 *
 */
string w2c(const wchar_t *pw)
{
    string val = "";
    if(!pw)
    {
        return val;
    }
    size_t size = wcslen(pw) * sizeof(wchar_t);
    char pc[DEFAULT_MAX_STR_LEN];

    size_t destlen = wcstombs(pc,pw,DEFAULT_MAX_STR_LEN);

    if(destlen == 0)
    {
        return val;
    }
    val = pc;

    return val;
}


/**
 * @brief: 替换字符串操作
 * @param str: 原始字符串
 * @param old_value: 被替换的子串
 * @param new_value: 新的子串
 * @retval: 替换后的字符串
 *
 */
string str_replace_all(const string& tstr, const string& old_value,const string& new_value)
{
    string str = tstr;
    while(true) {
        string::size_type pos(0);
        if( (pos=str.find(old_value)) != string::npos )
            str.replace(pos,old_value.length(),new_value);
        else break;
    }
    return str;
}

/**
 * @brief: 同上面的函数
 *
 */
wstring wstr_replace_all(const wstring& tstr,const wstring& old_value,const wstring& new_value)
{
    wstring str = tstr;
    while(true) {
        wstring::size_type pos(0);
        if( (pos=str.find(old_value))!=wstring::npos )
            str.replace(pos,old_value.length(),new_value);
        else break;
    }
    return str;
}



/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */

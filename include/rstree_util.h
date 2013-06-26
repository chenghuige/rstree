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
 * @brief �ַ���ת���ɿ�string
 *
 */
wstring c2w(const char* pc);


/**
 * @brief �����ַ���ת��Ϊstring 
 *
 */
string w2c(const wchar_t *pw);


/**
 * @brief: �滻�ַ�������
 * @param str: ԭʼ�ַ���
 * @param old_value: ���滻���Ӵ�
 * @param new_value: �µ��Ӵ�
 * @retval: �滻����ַ���
 *
 */
string str_replace_all(const string& tstr, const string& old_value,const string& new_value);


/**
 * @brief: ͬ����ĺ���
 *
 */
wstring wstr_replace_all(const wstring& tstr,const wstring& old_value,const wstring& new_value);



#endif  //__INCLUDE/RSTREE_UTIL_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */

/**
 *  ==============================================================================
 *
 *          \file   string_util.h
 *
 *        \author   chenghuige
 *
 *          \date   2013-10-27 15:40:58.907643
 *
 *  \Description:
 *  ==============================================================================
 */

#ifndef STRING_UTIL_H_
#define STRING_UTIL_H_
#include <string>
#include <iconv.h>
#include "log_util.h"
#include "chinese.h"
#include <boost/algorithm/string.hpp>

namespace gezi
{

inline bool is_gb2312(unsigned char ch1, unsigned char ch2)
{ //already gbk chinese,judge if gb2312 chinese
  return (ch1 >= 176) && (ch1 <= 247) && (ch2 >= 161);
}

inline bool is_gbk_ch(unsigned char ch1, unsigned char ch2)
{
  return ( (ch2 >= 64) && (ch2 <= 254) && (ch2 != 127) &&
          ((ch1 >= 129 && ch1 <= 160) || (ch1 >= 170 && ch1 < 254) ||
          (ch1 == 254 && ch2 <= 160)));
}

inline bool is_gbk_ch(const string& phrase)
{
  if (phrase.size() != 2)
    return false;
  return is_gbk_ch((unsigned char) phrase[0], (unsigned char) phrase[1]);
}


//en means single byte
inline bool all_en(const std::string& phrase)
{
  for (size_t i = 0; i < phrase.size();i++)
  {
    //双字节截断 不算single
    if (((unsigned char) phrase[i]) >= 0x80)
    {
      return false;
    }
  }

  return true;
}

//这里的en是single btye cn 限定gbk ch
inline bool is_en_dominate(const std::string& phrase, int var = 3)
{
  int cn_count = 0;
  int en_count = 0;
  for (size_t i = 0; i < phrase.size();)
  {
    if (((unsigned char) phrase[i]) >= 0x80 && i + 1 < phrase.size())
    {
      if (is_gbk_ch((unsigned char) phrase[i], (unsigned char) phrase[i + 1]))
      {
        cn_count++;
      }
      else
      {
        en_count++;
      }
      i += 2;
    }
    else
    {
      en_count++;
      i++;
    }
  }

  return en_count > cn_count * var;
}

//提取中文
string extract_chinese(const string& temp){
	char out[temp.size() + 1];
	memset(out, 0, sizeof(out));
	int index = 0;
	for(size_t i = 0; i < temp.size(); i++)
	{
		unsigned high = (unsigned)(0xff & temp[i]);
		if(high >= 0x81)
		{
			if((high > 0xa9 || high >= 0x81 && high <= 0xa0)&& i < temp.size() - 1)
			{
				out[index] = temp[i];
				out[index + 1] =  temp[i+1];
				index += 2;
			}
			i++;
		} 

	}
	string ret(out);
	return ret;
}
//提取符号
string extract_suspect_symb(const string& temp){
	char out[temp.size() + 1];
	memset(out, 0, sizeof(out));
	int index = 0;
	unsigned lastcode=0;
	for(size_t i = 0; i < temp.size(); i++)
	{
		unsigned code = (unsigned)(((0xff & temp[i])<<8)+(0xff & temp[i+1]));
		if(code >= 0x8100)
		{   
		 	if(code > 0xa100&&code<0xaa00&&
				code!=0xa3bf&&code!=0xa1a0&&
				code!=0xa3ac&&code!=0xa3a0&&
				code!=0xa1a3&&code!=0xa3a1&&
				code!=0xa1a4&&code!=0xa1a2&&
				code!=0xa3a8&&code!=0xa3a9&&
				code!=0xa1a2&&code!=0xa1a1&&
				code!=0xa3ba&&code!=0xa3bb&&
				code!=0xa1b0&&code!=0xa1b1&&
				code!=0xa1ad)
			{
				if(code!=lastcode){
				out[index] = temp[ i];
				out[index + 1] =  temp[i+1];
				index += 2;
				}
				lastcode=code;
				char logtmp[3];
				logtmp[0]=temp[i];
				logtmp[1]=temp[i+1];
				logtmp[2]=0;
	            ul_writelog(UL_LOG_DEBUG,"extract suspect [%s] [%x]",logtmp,code);
			}
		 	i++;
		} 

	}
	string ret(out);
	return ret;

}

inline string remove_space_cn(const string& phrase)
{
  if (phrase.empty())
  {
    return phrase;
  }
  char *buf = new char[phrase.size() + 1];
  int j = 0;
  bool pre_ch = false;
  for (size_t i = 0; i < phrase.size();)
  {
    if (((unsigned char) phrase[i]) >= 0x80 && i + 1 < phrase.size())
    {
      buf[j++] = phrase[i];
      buf[j++] = phrase[i + 1];
      pre_ch = true;
      i += 2;
    }
    else
    {
      if (phrase[i] != ' ' ||
              (!pre_ch && i + 1 < phrase.size() &&
              (((unsigned char) phrase[i + 1]) < 0x80)))
      {
        buf[j++] = phrase[i];
      }
      pre_ch = false;
      i++;
    }
  }
  buf[j] = '\0';
  string rs = buf;
  delete [] buf;
  return rs;
}

inline string remove_space_cnonly(const string & phrase)
{
  char *buf = new char[phrase.size() + 1];
  int j = 0;
  bool pre_ch = false;
  for (size_t i = 0; i < phrase.size();)
  {
    if (((unsigned char) phrase[i]) >= 0x80)
    {
      buf[j++] = phrase[i];
      buf[j++] = phrase[i + 1];
      pre_ch = true;
      i += 2;
    }
    else
    {
      if ((phrase[i] == ' ') && (pre_ch == true)
              && (i + 2 < phrase.size())
              && (((unsigned char) phrase[i + 1]) >= 0x80)
              )
      {
        buf[j++] = phrase[i + 1];
        buf[j++] = phrase[i + 2];
        pre_ch = true;
        i = i + 3;
      }
      else
      {
        buf[j++] = phrase[i++];
        pre_ch = false;
      }
    }
  }
  buf[j] = '\0';
  string rs = buf;
  delete [] buf;
  return rs;
}

inline int word_num(const string& phrase)
{
  int num = 0;
  bool not_ch = false;
  for (size_t i = 0; i < phrase.size();)
  {
    if (((unsigned char) phrase[i]) >= 0x80 && i + 1 < phrase.size())
    {
      if (not_ch)
      {
        num++;
        not_ch = false;
      }
      num++;
      i += 2;
    }
    else
    {
      if (phrase[i] != ' ')
        not_ch = true;
      else if (not_ch)
      {
        num++;
        not_ch = false;
      }
      i += 1;
    }
  }

  if (not_ch)
    num++;

  return num;
}

//vector<string> split(const string& src, const string & sep)
//{
//  vector<string> ret;
//  string::size_type index;
//  while ((index = src.find(sep)) != string::npos)
//  {
//    ret.push_back(src.substr(0, index));
//    src = src.substr(index + sep.size());
//  }
//  ret.push_back(src);
//#ifdef __GXX_EXPERIMENTAL_CXX0X__
//  return std::move(ret);
//#else
//  return ret;
//#endif
//}

inline int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
  iconv_t cd;
  char **pin = &inbuf;
  char **pout = &outbuf;

  cd = iconv_open(to_charset, from_charset);
  if (cd == 0)
  {
    return -1;
  }

  memset(outbuf, 0, outlen);

  if (int(iconv(cd, pin, &inlen, pout, &outlen)) == -1)
  {
    return -1;
  }

  iconv_close(cd);

  return 0;
}

inline string gbk_to_utf8(char* src)
{
  int outlen = strlen(src) * 3 + 1;
  char* outbuf = new char[outlen];
  memset(outbuf, 0, sizeof (outbuf));

  if (code_convert("gbk", "utf-8", src, strlen(src), outbuf, outlen) < 0)
  {
    LOG_WARNING("Convert from gbk_to_utf8 fail:%s", src);
    return "";
  }

  string rs = outbuf;
  delete [] outbuf;
  return rs;
}

inline string gbk_to_utf8(const string & src)
{
  int outlen = src.length()* 3 + 1;
  char* outbuf = new char[outlen];
  memset(outbuf, 0, sizeof (outbuf));

  if (code_convert("gbk", "utf-8", const_cast<char*> (src.c_str()), src.length(), outbuf, outlen) < 0)
  {
    LOG_WARNING("Convert from gbk_to_utf8 fail:%s", src.c_str());
    return "";
  }

  string rs = outbuf;
  delete [] outbuf;
  return rs;
}

inline string utf8_to_gbk(char* src)
{
  int outlen = strlen(src) * 2 + 1;
  char* outbuf = new char[outlen];
  memset(outbuf, 0, sizeof (outbuf));

  if (code_convert("utf-8", "gbk", src, strlen(src), outbuf, outlen) < 0)
  {
    LOG_WARNING("Convert from utf8_to_gbk fail:%s", src);
    return "";
  }

  string rs = outbuf;
  delete [] outbuf;
  return rs;
}

inline string utf8_to_gbk(const string & src)
{
  int outlen = src.length()* 2 + 1;
  char* outbuf = new char[outlen];
  memset(outbuf, 0, sizeof (outbuf));

  if (code_convert("utf-8", "gbk", const_cast<char*> (src.c_str()), src.length(), outbuf, outlen) < 0)
  {
    LOG_WARNING("Convert from utf8_to_gbk fail:%s", src.c_str());
    return "";
  }

  string rs = outbuf;
  delete [] outbuf;
  return rs;
}

inline string remove_dupspace(const string& input)
{
  if (input.size() == 0)
  {
    return input;
  }

  char* buf = new char[input.size() + 1];
  bool before_is_space = true;
  int j = 0;
  for (int i = 0; i < (int) input.size(); i++)
  {
    if (input[i] == ' ')
    {
      if (!before_is_space)
      { //the first space
        buf[j++] = input[i];
      }
      before_is_space = true;
    }
    else
    {
      buf[j++] = input[i];
      before_is_space = false;
    }
  }
  if (j > 1 && (buf[j - 1] == ' '))
  { //remove the last single space if exists
    j--;
  }
  buf[j] = '\0';

  string rs = buf;
  delete [] buf;
  return rs;
}

}

#endif  //----end of STRING_UTIL_H_

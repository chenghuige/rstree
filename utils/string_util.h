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

inline string remove_space_ch(const string& phrase)
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
    if (((unsigned char) phrase[i]) >= 0x81 && ((unsigned char) phrase[i]) <= 0xFE && i + 1 < phrase.size())
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
              (((unsigned char) phrase[i + 1]) < 0x81 || ((unsigned char) phrase[i]) > 0xFE)))
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

inline string remove_space_chonly(const string & phrase)
{
  char *buf = new char[phrase.size() + 1];
  int j = 0;
  bool pre_ch = false;
  for (size_t i = 0; i < phrase.size();)
  {
    if (((unsigned char) phrase[i]) >= 0x81 &&
            ((unsigned char) phrase[i]) <= 0xFE && i + 1 < phrase.size())
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
              && (((unsigned char) phrase[i + 1]) >= 0x81)
              && (((unsigned char) phrase[i + 1]) <= 0xFE)
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
    if (((unsigned char) phrase[i]) >= 0x81 && ((unsigned char) phrase[i]) <= 0xFE && i + 1 < phrase.size())
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
    LOG_WARNING("Convert from utf8_to_gbk fail:%s",src);
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
    LOG_WARNING("Convert from utf8_to_gbk fail:%s",src.c_str());
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
  for (int i = 0; i < (int)input.size(); i++)
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

/**
 *  ==============================================================================
 *
 *          \file   RstreeWorker.h
 *
 *        \author   chenghuige
 *
 *          \date   2013-11-04 19:41:36.956520
 *
 *  \Description:
 *  ==============================================================================
 */

#ifndef RSTREE_WORKER_H_
#define RSTREE_WORKER_H_
#include "Rstree.h"
//#include "dsuffix_tree.h"
#include "post_process/PostProcessor.h"
#include "word_seg.h"
#include "all_util.h"
#include "rstree_def.h"

namespace gezi
{

class RstreeWorker
{
public:

  RstreeWorker()
  : _min_text_length(8),
  _max_text_length(200),
  _max_result_count(5),
  _use_seg(true),
  _simple_sort(false)
  {

  }

  static void init_static()
  {
    setlocale(LC_ALL, LOCALE);
    SharedConf::init(RSTREE_CONF_DIR, RSTREE_CONF);
    PostProcessor::init_static();

    string seg_data_dir = "./data/wordseg";
    string section = "";
    SCONF(seg_data_dir);
    //  Seg::instance()->init(seg_data_dir);
    seg_init(seg_data_dir);
  }

  void init()
  {
    _rstree.init();
    _post_processor.init();
    _seg_handle.init();

    string section = "RstreeWorker";
    SCONF(_min_text_length);
    SCONF(_max_text_length);
    SCONF(_max_result_count);
    SCONF(_use_seg);
    SCONF(_simple_sort);
  }

  /*
   * @brief: 合并map，将m2中的key-value对合并入m1，当m2中不包含这个key，或者对应的value值比m1中的大时
   *
   */
  void merge_map(map<wstring, int> & m1, const map<wstring, int> & m2)
  {
    for (map<wstring, int>::const_iterator iter = m2.begin(); iter != m2.end(); ++iter)
    {
      map<wstring, int>::iterator iter1 = m1.find(iter->first);
      if (iter1 == m1.end() || iter1->second < iter->second)
      {
        m1[iter->first] = iter->second;
      }
    }
  }

  bool get_postype(const SegHandle& handle, int len, vector<int>& vec)
  {
    vec.resize(len, MIDDLE);
    int start = 0;
    for (int i = 0; i < handle.nresult; i++)
    {
      int end = start + wchar_count(handle.tokens[i].buffer, handle.tokens[i].length);
      if (end > len)
      {
        return false;
      }
      if (end - start > 1)
      {
        vec[start] = LEFT;
        vec[end - 1] = RIGHT;
      }
      else
      {
        vec[start] = SINGLE;
      }
      start = end;
    }
    if (start == len)
    {
      return true;
    }
    else
    {
      return false;
    }
  }

  inline void print_seg_result(const SegHandle& handle)
  {
    Pval(handle.nresult);
    for (int i = 0; i < handle.nresult; i++)
    {
      cout << handle.tokens[i].buffer << " " << handle.tokens[i].length << endl;
    }
    cout << std::endl;
  }

  bool get_postype(const wstring& sub_wc, vector<int>& splits)
  {
    string sub_c = wstr_to_str(sub_wc);
    if (sub_c.empty())
    {
      LOG_WARNING("Get postype to wstr to str fail");
      return false;
    }
    bool ret = segment(sub_c, _seg_handle);
    if (!ret)
    {
      LOG_WARNING("Seg fail %s", sub_c.c_str());
      return false;
    }
    ret = get_postype(_seg_handle, sub_wc.size(), splits);
    if (!ret)
    {
      LOG_WARNING("getpos fail for %s %d", sub_c.c_str(), sub_wc.size());
    }
    return ret;
  }

  bool get_postype(const string& sub_c, int len, vector<int>& splits)
  {
    bool ret = segment(sub_c, _seg_handle);
    if (!ret)
    {
      LOG_WARNING("Seg fail %s", sub_c.c_str());
      return false;
    }
    ret = get_postype(_seg_handle, len, splits);
    if (!ret)
    {
      LOG_WARNING("getpos fail for %s %d", sub_c.c_str(), len);
    }
    return ret;
  }
  //0 

  struct Cmp
  {

    bool operator() (const ONode& left, const ONode& right)
    {
      return left.str.length() > right.str.length();
    }
  };

  vector<ONode> get_substrs(const string& content, int min_freq, int min_len, int max_len)
  {

    _rstree.set_min_frequency(min_freq);
    _rstree.set_min_substr_len(min_len);
    _rstree.set_max_substr_len(max_len);

    wstring wc = str_to_wstr(content);
    map<wstring, int> ret_map;
    if ((int) wc.size() >= _min_text_length)
    {
      vector<wstring> svec = cut_wstring(wc, _max_text_length, max_len);

      LOG_DEBUG("Cut to %d parts", (int) svec.size());
      //for (auto &sub_wc : svec)
      for (int i = 0; i < (int) svec.size(); i++)
      {
        wstring& sub_wc = svec[i];
        if ((int) sub_wc.size() < _min_text_length)
        {
          break;
        }

        vector<WPair > rvec;
        if (_use_seg)
        {
          vector<int> splits;
          bool sucess;
          if (svec.size() == 1)
          {//不需要再转换回gbk了
            sucess = get_postype(content, sub_wc.size(), splits);
          }
          else
          {
            sucess = get_postype(sub_wc, splits);
          }
          if (sucess)
          {
            _rstree.add(sub_wc, rvec, &splits);
          }
          else
          {
            LOG_WARNING("Fail in get segment split");
            _rstree.add(sub_wc, rvec);
          }
        }
        else
        {
          _rstree.add(sub_wc, rvec);
        }
        LOG_DEBUG("After add got %d result", (int) rvec.size());
        PostAdjustor::filter(rvec, _rstree.min_substr_len());
        map<wstring, int> t_ret_map(rvec.begin(), rvec.end());

        //        map<wstring, int> t_ret_map;
        //        _rstree.add_text(sub_wc, t_ret_map);
        //        if (_rstree.get_tree_size() >= _rstree.get_max_tree_size())
        //        {
        //          _rstree.remove_text();
        //        }
        merge_map(ret_map, t_ret_map);
      }
    }

    //-----------------post deal trim filter
    vector <INode> vec;
    for (map<wstring, int>::iterator iter = ret_map.begin(); iter != ret_map.end(); ++iter)
    {
      string substr = wstr_to_str(iter->first);
      if (!substr.empty())
      {
        vec.push_back(INode(substr, iter->first, iter->second));
      }
    }

    //-----------------post black white deal filter and sort choose top max_result_count
    vector<ONode> result_vec;
    if (!_simple_sort)
    {
      _post_processor.process(vec, result_vec, _max_result_count);
    }
    else
    {
      for (int i = 0; i < (int)vec.size(); i++)
      {
        result_vec.push_back(ONode(vec[i].str, vec[i].count));
      }
      if (result_vec.size() <= _max_result_count)
      {
        std::sort(result_vec.begin(),result_vec.end(), Cmp());
      }
      else
      {
        std::partial_sort(result_vec.begin(), result_vec.begin() + _max_result_count,
                result_vec.end(), Cmp());
        result_vec.resize(_max_result_count);
      }
    }

#ifdef __GXX_EXPERIMENTAL_CXX0X__
    return std::move(result_vec);
#else
    return result_vec;
#endif
  }

  inline int tree_size()
  {
    return _rstree.tree_size();
  }

private:
  int _min_text_length;
  int _max_text_length;
  int _max_result_count;
  bool _use_seg;
  bool _simple_sort;

  Rstree _rstree;
  //  DSuffixTree _rstree;
  PostProcessor _post_processor;
  SegHandle _seg_handle;
};

} //----end of namespace gezi

#endif  //----end of RSTREE_WORKER_H_

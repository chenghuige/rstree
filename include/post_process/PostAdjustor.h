/* 
 * File:   PostAdjustor.h
 * Author: chenghuige
 *
 * Created on 2013年11月3日, 上午11:44
 */

#ifndef POST_ADJUSTOR_H_
#define	POST_ADJUSTOR_H_
#include <string>
#include <vector>
#include "string_util.h"
#include "rstree_def.h"
namespace gezi
{
using namespace std;

class PostAdjustor
{
private:
  wstring _puncs;
public:

  PostAdjustor()
  {
    _puncs = str_to_wstr("。？！，,?!");
  }

  void filter(vector<WPair >& vec, int min_len)
  {
    vector<WPair > temp;
    //for(auto &itme: vec)
    for (int i = 0; i < (int) vec.size(); i++)
    {
      WPair& item = vec[i];
      boost::trim(item.first);
      boost::trim_if(item.first, is_any_of(_puncs));
      if ((int) item.first.size() < min_len)
      {
        continue;
      }
      temp.push_back(item);
    }
    vec.swap(temp);
  }
};
}

#endif	/* POSTADJUSTOR_H */


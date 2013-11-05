/* 
 * File:   PostAdjustor.h
 * Author: chenghuige
 *
 * Created on 2013��11��3��, ����11:44
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
public:

  static void filter(vector<WPair >& vec, int min_len)
  {
    vector<WPair > temp;
    //for(auto &itme: vec)
    for(int i = 0; i < (int)vec.size(); i++)
    {
      WPair& item = vec[i];
      boost::trim(item.first);
      if ((int) item.first.size() < min_len)
      {
        continue;
      }
      temp.push_back(item);
    }
    vec.swap(temp);
  }
  
private:
//  string content; //rsc���ݹ����� ԭʼgbk�����ı� ���ڷִ�
};
}

#endif	/* POSTADJUSTOR_H */


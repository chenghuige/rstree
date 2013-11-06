/** 
 *  ==============================================================================
 * 
 *          \file   test/test_suffixtree.cc
 *
 *        \author   chenghuige   
 *
 *          \date   2013-11-04 23:03:06.680812
 *  
 *  \Description:
 *
 *  ==============================================================================
 */

#define private public
#define protected public
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include "common_util.h"
#include "debug_util.h"
#include "log_util.h"
#include "conf_util.h"
#include "SuffixTree.h"
#include "Rstree.h"
#include "string_util.h"
using namespace std;
using namespace gezi;
DEFINE_string(type, "simple", "");
DEFINE_bool(perf, false, "");
DEFINE_int32(num, 1, "");
DEFINE_string(i, "zhenxiai.txt", "input file");
DEFINE_string(o, "", "output file");

void run()
{
  setlocale(LC_ALL, "zh_CN.GB18030");
  //SuffixTree st;
  Rstree st(L"#");
  st.add(L"abcd");
  st.add(L"abde");
  //  ifstream ifs(FLAGS_i.c_str());
  //  string s;
  //  wstring ws;
  //  while (getline(ifs, s))
  //  {
  //    boost::trim(s);
  //    ws = str_to_wstr(s);
  //    wcout << ws << endl;
  //    st.add(ws);
  //  }
  //  
  //  for (int i = 0; i < 10; i++)
  //  {
  //    st.add(ws);
  //  }


  st.print();
}

int main(int argc, char *argv[])
{
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  int s = google::ParseCommandLineFlags(&argc, &argv, false);
  if (FLAGS_log_dir.empty())
    FLAGS_logtostderr = true;
  LogHelper log_helper;

  run();

  return 0;
}

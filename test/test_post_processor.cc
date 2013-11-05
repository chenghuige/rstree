/** 
 *  ==============================================================================
 * 
 *          \file   test/test_post_processor.cc
 *
 *        \author   chenghuige   
 *
 *          \date   2013-11-02 00:22:51.954303
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
#include "PostProcessor.h"
using namespace std;
DEFINE_string(type, "simple", "");
DEFINE_bool(perf, false, "");
DEFINE_int32(num, 1, "");
DEFINE_string(i, "", "input file");
DEFINE_string(o, "", "output file");

void run()
{
  
  {
    string src("既能么么哒，也能啪啪啪】");
    boost::regex reg("么么哒.*?啪啪啪", boost::regex::perl);
    bool ret = boost::regex_search(src, reg);
    Pval(ret);
    src = boost::regex_replace(src, reg, "***");
    Pval(src);
  }
  {
    string src("--来自iphone5s土豪金客户端");
    boost::regex reg("--来自.*", boost::regex::perl);
    bool ret = boost::regex_search(src, reg);
    Pval(ret);
    src = boost::regex_replace(src, reg, "***");
    Pval(src);
  }
  
  {
    string src("么么哒,也啪啪啪");
    Pval(src);
    Pval(src.length());
    setlocale(LC_ALL,"zh_CN.GB18030");
    wstring wsr = str_to_wstr(src);
    Pval(wsr.length());
    string src2 = wstr_to_str(wsr);
    Pval(src2);
    Pval(src2.length());
  }

  SharedConf::init();
  PostProcessor processor;
  processor.init_static();
  processor.init();

  vector<pair<string, int> > vec;
  vec.push_back(make_pair("十五字来我wang店悄悄qq在头像上", 1));
  vec.push_back(make_pair("QQ来了", 1));
  vec.push_back(make_pair("Q,Q来了", 3));
  vec.push_back(make_pair("Q Q来了", 1));
  vec.push_back(make_pair("歪 歪昵 称改 为 【 2 团情 儿邀 请 】", 2));
  vec.push_back(make_pair("有玩弩的朋友吗？？狩猎利器 QQ 742208700 免收定金 货到付款", 3));
  vec.push_back(make_pair("要不是我快十级了我都懒得回你的贴，数数够不够十五字。", 20));
  vec.push_back(make_pair("既能么么哒，也能啪啪啪】", 5));
  vec.push_back(make_pair("既能么么哒，也能啪啪啪", 5));
  vec.push_back(make_pair("198三四五六", 2));
  vec.push_back(make_pair("欢迎到那啥吧访问", 2));
  //  vec.push_back(make_pair("插插插插插插插插插插插插插插插———————————————", 5));
  {
    //auto vec2 = processor.process(vec, 100);
    vector<Node> vec2;
    processor.process(vec, 100, vec2);
    PrintVec2(vec2, str, black_count);
  }
  //  {
  //    //auto vec2 = processor.process(vec, 100);
  //    vector<Node> vec2;
  //    processor.process(vec, 5, vec2);
  //    PrintVec2(vec2,str, black_count);
  //  }
}

int main(int argc, char *argv[])
{
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  int s = google::ParseCommandLineFlags(&argc, &argv, false);
  if (FLAGS_log_dir.empty())
    FLAGS_logtostderr = true;
  LogHelper log_utiler;

  run();

  return 0;
}

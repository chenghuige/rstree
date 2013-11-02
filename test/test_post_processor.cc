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
#include "common_help.h"
#include "debug_help.h"
#include "log_util.h"
#include "configure_util.h"
#include "PostProcessor.h"
using namespace std;
DEFINE_string(type, "simple", "");
DEFINE_bool(perf,false, "");
DEFINE_int32(num, 1, "");
DEFINE_string(i, "", "input file");
DEFINE_string(o, "", "output file");
DECLARE_SHAREDCONF;

void run()
{
  SharedConf::init();
  PostProcessor processor;
  processor.init();
  
  vector<pair<string, int> > vec;
  vec.push_back(make_pair("十五字来我wang店悄悄qq在头像上", 1));
  processor.process(vec);
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

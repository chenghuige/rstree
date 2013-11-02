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
DEFINE_bool(perf,false, "");
DEFINE_int32(num, 1, "");
DEFINE_string(i, "", "input file");
DEFINE_string(o, "", "output file");

void run()
{
  SharedConf::init();
  PostProcessor processor;
  processor.init();
  
  vector<pair<string, int> > vec;
  vec.push_back(make_pair("ʮ��������wang������qq��ͷ����", 1));
  vec.push_back(make_pair("QQ����", 1));
  vec.push_back(make_pair("Q,Q����", 3));
  vec.push_back(make_pair("Q Q����", 1));
  vec.push_back(make_pair("�� ���� �Ƹ� Ϊ �� 2 ���� ���� �� ��",2));
  vec.push_back(make_pair("������������𣿣��������� QQ 742208700 ���ն��� ��������", 3));
  vec.push_back(make_pair("Ҫ�����ҿ�ʮ�����Ҷ����û������������������ʮ���֡�",20));
  vec.push_back(make_pair("����ôô�գ�Ҳ��žžž��",5));
  vec.push_back(make_pair("���������������塪����������������������������",5));
  {
    //auto vec2 = processor.process(vec, 100);
    vector<Node> vec2;
    processor.process(vec, 100, vec2);
    PrintVec2(vec2, substr, black_count);
  }
  {
    //auto vec2 = processor.process(vec, 100);
    vector<Node> vec2;
    processor.process(vec, 5, vec2);
    PrintVec2(vec2, substr, black_count);
  }
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

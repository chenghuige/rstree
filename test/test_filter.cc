/** 
 *  ==============================================================================
 * 
 *          \file   test/test_filter.cc
 *
 *        \author   chenghuige   
 *
 *          \date   2013-11-01 11:53:07.173597
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
#include "RstreeFilter.h"

using namespace std;
DEFINE_string(type, "simple", "");
DEFINE_bool(perf,false, "");
DEFINE_int32(num, 1, "");
DEFINE_string(i, "", "input file");
DEFINE_string(o, "", "output file");

RstreeFilter g_filter;

void test(string s)
{
  Pval(s);
  Pval(g_filter.is_pass(s));
}

void run()
{
  setlocale(LC_ALL, "zh_CN.GB18030");
  g_filter.init();
  test("十五字十五字十五字十五字十五字，ID:苏三说");
  test("楼楼可以加这个双眼皮交流群288090810问问群里的妹纸们呢");
  test("神州九号 神舟九号 我是北京 我是北京！");
  test("———啪啪啪啪啪啪啪啪啪啪啪——");
  test("全　。了　　　　　　~~~小尾巴；“我已经不知道爱情这东西到底奢侈的什么样的状态");
  test("萌萌的豆豆快来挽尊");
  test("插插插插插插插插插插插插插插插———————————————");
  test("点经验！请用指甲或小刀用力刮开黑条获得！██████████");
  test("　　　　　　 如果你爱");
  test("△△△△中奖楼层为88");
  test("有15字了没 15字了没 5字了没 字了没 了没 没");
  test("14楼来访，助攻助攻助攻助攻，求回访");
  test("一二三四 二二三四 三二三四 再水一次");
  test("快来人啊！帮我看看够不够十五字啊！");
  test("武 汉 Q Q");
  test("妹妹 我要和你爱爱~~~~~~~~好多水 水");
  test("爱爱爱爱爱爱爱爱爱爱爱爱爱爱爱");
  test("见过这样标准的十五");
  test("。2013年12月1日");
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

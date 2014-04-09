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
  test("ʮ����ʮ����ʮ����ʮ����ʮ���֣�ID:����˵");
  test("¥¥���Լ����˫��Ƥ����Ⱥ288090810����Ⱥ�����ֽ����");
  test("���ݾź� ���۾ź� ���Ǳ��� ���Ǳ�����");
  test("������žžžžžžžžžžž����");
  test("ȫ�����ˡ�����������~~~Сβ�ͣ������Ѿ���֪�������ⶫ�������ݳ޵�ʲô����״̬");
  test("���ȵĶ�����������");
  test("���������������塪����������������������������");
  test("�㾭�飡����ָ�׻�С�������ο�������ã���������������������");
  test("������������ ����㰮");
  test("���������н�¥��Ϊ88");
  test("��15����û 15����û 5����û ����û ��û û");
  test("14¥���ã���������������������ط�");
  test("һ������ �������� �������� ��ˮһ��");
  test("�����˰������ҿ���������ʮ���ְ���");
  test("�� �� Q Q");
  test("���� ��Ҫ���㰮��~~~~~~~~�ö�ˮ ˮ");
  test("������������������������������");
  test("����������׼��ʮ��");
  test("��2013��12��1��");
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

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
//#include <vector>
#include <fstream>
#include <algorithm>
#include <glog/logging.h>
#include <gflags/gflags.h>
//#include "common_util.h"
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
	SuffixTree st(L"#");
	//Rstree st(L"#");
	//  st.add(L"abcd");
	//  st.add(L"abde");
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

	string s = "���ݵ�Jeju Island ��ȥҲд�� Cheju Island���Ǻ������ĵ��죬��һ�����͵Ļ�ɽ�����������ߴ���Ȼ���֮һ��120����ǰ��ɽ����γɣ���������ͨ����ɽ�������γɵĺ���1951�׵ĺ�����߷庺��ɽ�����ݵ��ĺ��ͱ�������ȫ��ͬ-ɳ̲�Ǵ���ɫ�ģ���ˮȾ����ĸ�̱�ʯɫ���ͻ��������������Ƭ�￴������̫ƽ�����������޶������Լ��ݵ�����������ȥ���ٴ�Ҳ����������������ݵ���������ľ���һ��Ҳ��ѷɫ-���۹����Һ���ĸ�̱�ʯɫ�Ĵ󺣣���ʱ�򡰺�г����������ʱ�򡰱����������������Ը��ܵ������ڼ��ݵ��󺣵����������⣬���˼��ݵ�һ��Ҫȥ��ʶһ�¼��ݵ��Ķĳ�������������16�Ҷĳ�������8�Ҷ��ڼ��ݵ������Ƿֱ���8��5�Ǽ��Ƶ�����������ϸ������ɾ�Ӫ�����ֳ��������ҷ���Ҳ�ǰ����Ǳ�û�취����ġ�ֻҪ��ͨ��JCIרҵ�ĳ�ϴ���н����𣨵绰��������⣩�������Ϳ�����������VIP���Ĵ���������ṩ������Ʊ+��������+5�Ǽ��Ƶ���ס+������ʳ�۹�+��������ͬ��һϵ�����ʷ��񡣾��Կ����������˼��ɾ����ݵ���һ���������ó̡�";
	wstring ws = str_to_wstr(s);
	Pval(s);
	Pval(wstr2str(ws));
	st.add(ws);
	{
		string s2 = "������16";
		wstring ws2 = str_to_wstr(s2);
		int freq = st.find_freq(ws2);
		Pval(freq);
	}
	st.add(wstr("�޺콨"));
	st.add(wstr("÷�����ǵ��Ǻ������ṩ�÷���"));
//  {
//     string s2 = "ֻҪ��ͨ��JCIרҵ�ĳ�ϴ���н����𣨵绰��������⣩����";
//    wstring ws2 = str_to_wstr(s2);
//    int freq = st.find_freq(ws2);
//    Pval(freq);
//  }
//  
	{
		string s2 = FLAGS_i;
		 wstring ws2 = str_to_wstr(s2);
		int freq = st.find_freq(ws2);
		Pval(freq);
	
	}
	//st.print();
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

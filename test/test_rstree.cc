/**
 *  ==============================================================================
 *
 *          \file   test_rstree.cc
 *
 *        \author   chenghuige
 *
 *          \date   2015-02-03 17:51:58.694664
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
#include "common_util.h"
#include "debug_util.h"
#include "log_util.h"
#include "conf_util.h"
#include "SuffixTree.h"
#include "Rstree.h"
#include "string_util.h"

//#include <boost/locale.hpp>

using namespace std;
using namespace gezi;
DEFINE_string(type, "simple", "");
DEFINE_bool(perf, false, "");
DEFINE_int32(num, 1, "");
DEFINE_string(i, "input.txt", "input file");
DEFINE_string(o, "output.txt", "output file");
DEFINE_string(uids, "uids.txt", "");

DEFINE_int32(maxlen, 5, "");
DEFINE_int32(minlen, 2, "");
DEFINE_int32(minfreq, 3, "");

void run()
{
	//std::locale::global(std::locale(""));
	//string sSource = "gbk�ַ���";
	//wstring ws = boost::locale::conv::to_utf<wchar_t>(sSource, "GBK");

	//string  ss1 = boost::locale::conv::from_utf(ws, "GBK");

	//Pval2(sSource, ss1);

	//setlocale(LC_ALL, "zh_CN.GB18030");
	std::locale::global(std::locale("zh_CN.GB18030"));

	Rstree tree(L"#");

	tree.set_max_substr_len(FLAGS_maxlen);
	tree.set_min_substr_len(FLAGS_minlen);
	tree.set_min_frequency(FLAGS_minfreq);

	tree.add(wstr("�޺콨������"));
	tree.add(wstr("�޺콨��Ҫ��"));
	tree.add(wstr("˭��Ҫ����"));
	tree.add(wstr("cbg�����ս�Ϊʲô����ecom������?"));
	tree.add(wstr("��Ҳ��Ҫ50���µ����ս���������"));
	tree.add(wstr("�Ǹ����޺콨�ľ�Ȼ����5�������ս�"));

	Pval(tree.find_freq(wstr("��Ҫ��")));
	Pval(tree.find_freq(wstr("�޺콨")));
	Pval(tree.find_freq(wstr("����")));
	Pval(tree.find_freq(wstr("�ս�")));

	vector<Rstree::Pair> results = tree.find_all_substrs();

	for (size_t i = 0; i < results.size(); i++)
	{
		Pval3(i, wstr2str(results[i].first), results[i].second);
	}

	for (size_t i = 0; i < tree.size(); i++)
	{
		vector<Rstree::Pair> results = tree.find_substrs(i);
		for (size_t j = 0; j < results.size(); j++)
		{
			Pval4(i, j, wstr2str(results[j].first), results[j].second);
		}
	}
}

void run2()
{
	//std::locale::global(std::locale(""));
	//string sSource = "gbk�ַ���";
	//wstring ws = boost::locale::conv::to_utf<wchar_t>(sSource, "GBK");

	//string  ss1 = boost::locale::conv::from_utf(ws, "GBK");

	//Pval2(sSource, ss1);

	//setlocale(LC_ALL, "zh_CN.GB18030");
	std::locale::global(std::locale("zh_CN.GB18030"));

	Rstree tree(L"#");

	tree.set_max_substr_len(FLAGS_maxlen);
	tree.set_min_substr_len(FLAGS_minlen);
	tree.set_min_frequency(FLAGS_minfreq);

	tree.add(wstr("�޺콨������"));
	tree.add(wstr("�޺콨��Ҫ��"));
	tree.add(wstr("˭��Ҫ����"));
	tree.add(wstr("cbg�����ս�Ϊʲô����ecom������?"));
	tree.add(wstr("��Ҳ��Ҫ50���µ����ս���������"));
	tree.add(wstr("�Ǹ����޺콨�ľ�Ȼ����5�������ս�"));
	tree.add(wstr("��С��������"));
	tree.add(wstr("��С��������"));
	tree.add(wstr("��С��������"));

	Pval(tree.find_freq(wstr("��Ҫ��")));
	Pval(tree.find_freq(wstr("�޺콨")));
	Pval(tree.find_freq(wstr("����")));
	Pval(tree.find_freq(wstr("�ս�")));
	Pval(tree.find_freq(wstr("��С��")));
	Pval(tree.find_freq(wstr("�콨")));
	Pval(tree.find_freq(wstr("С��")));
	Pval(tree.find_freq(wstr("��")));

	vector<Rstree::Pair> results = tree.find_all_substrs();

	for (size_t i = 0; i < results.size(); i++)
	{
		Pval3(i, wstr2str(results[i].first), results[i].second);
	}

	for (size_t i = 0; i < tree.size(); i++)
	{
		vector<Rstree::Pair> results = tree.find_substrs(i);
		for (size_t j = 0; j < results.size(); j++)
		{
			Pval4(i, j, wstr2str(results[j].first), results[j].second);
		}
	}
}

void Run()
{
	std::locale::global(std::locale("zh_CN.GB18030"));
	Rstree tree(L"#", FLAGS_minlen, FLAGS_maxlen, FLAGS_minfreq);
	Pval3(tree._min_substr_len, tree._max_substr_len, tree._min_frequency);
	ifstream ifs(FLAGS_i);
	string line;
	int count = 0;
	set<string> uids;
	gezi::read_to_set(FLAGS_uids, uids);
	while (getline(ifs, line))
	{
		if (count % 10000 == 0)
		{
			Pval2(tree.size(), count);
		}
		string uid, content;
		gezi::split(line, '\t', uid, content);
		if (uids.count(uid))
		{
			tree.add(wstr(line));
		}
		count++;
	}
	vector<Rstree::Pair> results = tree.find_all_substrs();
	Pval(results.size());
	//gezi::sort(results, 10, [](const Rstree::Pair& a, const Rstree::Pair& b) { return a.second > b.second; });
	std::sort(results.begin(), results.end(), [](const Rstree::Pair& a, const Rstree::Pair& b) { return a.second > b.second; });

	/*for (size_t i = 0; i < std::min((int)results.size(), 10); i++)
	{
	Pval3(i, wstr2str(results[i].first), results[i].second);
	}*/

	ofstream ofs(FLAGS_o);

	for (auto& item : results)
	{
		ofs << wstr2str(item.first) << "\t" << item.second << "\n";
	}
}


int main(int argc, char *argv[])
{
	google::InitGoogleLogging(argv[0]);
	google::InstallFailureSignalHandler();
	int s = google::ParseCommandLineFlags(&argc, &argv, false);
	if (FLAGS_log_dir.empty())
		FLAGS_logtostderr = true;

	run();

	run2();

	Run();

	return 0;
}

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
DEFINE_string(conf,"rstree_strategy.conf","conf file");

void run()
{
  
  {
    string src("����ôô�գ�Ҳ��žžž��");
    boost::regex reg("ôô��.*?žžž", boost::regex::perl);
    bool ret = boost::regex_search(src, reg);
    Pval(ret);
    src = boost::regex_replace(src, reg, "***");
    Pval(src);
  }
  {
    string src("--����iphone5s������ͻ���");
    boost::regex reg("--����.*", boost::regex::perl);
    bool ret = boost::regex_search(src, reg);
    Pval(ret);
    src = boost::regex_replace(src, reg, "***");
    Pval(src);
  }
  
  {
    string src("ôô��,Ҳžžž");
    Pval(src);
    Pval(src.length());
    setlocale(LC_ALL,"zh_CN.GB18030");
    wstring wsr = str_to_wstr(src);
    Pval(wsr.length());
    string src2 = wstr_to_str(wsr);
    Pval(src2);
    Pval(src2.length());
  }

  SharedConf::init(FLAGS_conf);
  PostProcessor processor;
  processor.init_static();
  processor.init();

  {
      /*
      string src("΢�ź�:imanu20����");
      string ret = processor.get_spam_weixin_qq(src);
      std::cout << ret << std::endl;
      */
      string src("����\"�й��������\"���߰˴��桿��{�й�����}Ҳ���Թ�");
      //src = "�ɶ����Ƽ���˺���õ� ���ȴ�����ƹ���˺���á��Ժ�̶̵ļ���ʱ���ڣ���˺���õ��������Ѿ����ɶ���ÿ����ͥ����֪������Ұζ��ʤ��Ұζ�ɶ����Ƽ���˺������Ȼ��Ϊ�������ӼҴ��͡��������м����С�̽�׵���ѡ��ɫʳ ѧ���ڳɶ���˺���ü���,�������ɶ����Ƽ���ʳ�� �й������רҵ���Ĵ���ɫ��ʳ��ѵ����,�ְ���һ��һ���⣬���̰��ᣬѧ��Ϊֹ����֤����ȫѧ�ᡣ �Ŷӳ�Ա��������ְ����ְ�ĸ���˾��ʦ�����ܳ�������ʵ���ۺ� �Ҽ�����ѵ��˾��֮���Խ�רҵ���ҹ�˾���г�Ա������������������Ȼ��ְ�ĸ����Ƶ꣬С��������˾���ܳ�����ʦ���ȣ����ǲ����������Ա��������ѵ�ʹ��ڵĶ���������ʮ�����ʵ�ʲ������ܽ�����ļ������鼰�����ɹ������ǳ�ʦѧУ����ˮ��ϰ�����Ǹ����ģ����Ǻ��ĺ���ʮ��������������רҵ���飬רҵ���������Ƕ����ı��ϣ������������ڳ�ʦѧУѧ�����ģ� ���Ⱥ�Ϊȫ����Ҳ������ṩ����ָ������Ը��˴���ȫ���漼����ѵ�Ϳ���ı��";
      //src = "<em>�й�����n<\\em>";
      src = "�Ҿ�ֻ��˵hello ���ң�������";
      string ret = processor.get_seem_black(src);
      std::cout << "HHH::"<< ret << "HHH::"<< std::endl;
  }

  vector<INode > vec;
  vec.push_back(INode("ʮ��������wang������qq��ͷ����", 1));
  vec.push_back(INode("QQ����", 1));
  vec.push_back(INode("Q,Q����", 3));
  vec.push_back(INode("Q Q����", 1));
  vec.push_back(INode("�� ���� �Ƹ� Ϊ �� 2 ���� ���� �� ��", 2));
  vec.push_back(INode("������������𣿣��������� QQ 742208700 ���ն��� ��������", 3));
  vec.push_back(INode("Ҫ�����ҿ�ʮ�����Ҷ����û������������������ʮ���֡�", 20));
  vec.push_back(INode("����ôô�գ�Ҳ��žžž��", 5));
  vec.push_back(INode("����ôô�գ�Ҳ��žžž", 5));
  vec.push_back(INode("198��������", 2));
  vec.push_back(INode("��ӭ����ɶ�ɷ���", 2));
  vec.push_back(INode(FLAGS_i, 3));
  //  vec.push_back(INode("���������������塪����������������������������", 5));
  {
    //auto vec2 = processor.process(vec, 100);
    vector<ONode> vec2;
    processor.process(vec, vec2, 100);
    PrintVec2(vec2, str, black_count);
  }
  //  {
  //    //auto vec2 = processor.process(vec, 100);
  //    vector<Node> vec2;
  //    processor.process(vec,vec2, 5);
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

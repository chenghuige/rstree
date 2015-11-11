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

  SharedConf::init(FLAGS_conf);
  PostProcessor processor;
  processor.init_static();
  processor.init();

  {
      /*
      string src("微信号:imanu20哈哈");
      string ret = processor.get_spam_weixin_qq(src);
      std::cout << ret << std::endl;
      */
      string src("快来\"中国人民哈哈\"【七八传奇】吧{中国人民}也可以哈");
      //src = "成都上善佳手撕烤兔店 首先创造和推广手撕烤兔。以后短短的几年时间内，手撕烤兔的名声就已经被成都的每个家庭所熟知。不是野味，胜似野味成都上善佳手撕烤兔已然成为广大市民居家待客、娱乐休闲及旅行、探亲的首选特色食 学正宗成都手撕烤兔技术,搜索【成都上善佳美食】 中国最大最专业的四川特色美食培训机构,手把萄一对一讲解，包教包会，学会为止，保证您完全学会。 团队成员均是曾任职或在职的各大公司厨师长或总厨，技术实力雄厚。 我技术培训公司，之所以叫专业因我公司所有成员，均是曾经或现在依然在职的各个酒店，小吃连锁公司的总厨、厨师长等，都是餐饮店核心人员，我们培训和传授的都是有着数十几年的实际操作所总结出来的技术经验及技术成果，并非厨师学校的流水型习，我们给您的，就是核心和数十几年摸索出来的专业经验，专业，就是我们对您的保障；在这里是你在厨师学校学不到的！ 已先后为全国多家餐饮店提供技术指导。针对个人创行全方面技术培训和开店谋划";
      //src = "<em>中国人米n<\\em>";
      src = "我就只想说hello 看我！！！！";
      string ret = processor.get_seem_black(src);
      std::cout << "HHH::"<< ret << "HHH::"<< std::endl;
  }

  vector<INode > vec;
  vec.push_back(INode("十五字来我wang店悄悄qq在头像上", 1));
  vec.push_back(INode("QQ来了", 1));
  vec.push_back(INode("Q,Q来了", 3));
  vec.push_back(INode("Q Q来了", 1));
  vec.push_back(INode("歪 歪昵 称改 为 【 2 团情 儿邀 请 】", 2));
  vec.push_back(INode("有玩弩的朋友吗？？狩猎利器 QQ 742208700 免收定金 货到付款", 3));
  vec.push_back(INode("要不是我快十级了我都懒得回你的贴，数数够不够十五字。", 20));
  vec.push_back(INode("既能么么哒，也能啪啪啪】", 5));
  vec.push_back(INode("既能么么哒，也能啪啪啪", 5));
  vec.push_back(INode("198三四五六", 2));
  vec.push_back(INode("欢迎到那啥吧访问", 2));
  vec.push_back(INode(FLAGS_i, 3));
  //  vec.push_back(INode("插插插插插插插插插插插插插插插———————————————", 5));
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

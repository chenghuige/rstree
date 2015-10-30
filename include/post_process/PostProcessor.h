/**
 *  ==============================================================================
 *
 *          \file   PostProcessor.h
 *
 *        \author   chenghuige
 *
 *          \date   2013-10-31 21:28:41.545779
 *
 *  \Description:  后处理包括
 *  TODO  在DSuffixTree模块增加分词(utf8分词)判断，即对于文本同时存储一个是否分词边界位判断 这样抽出的串符合分词 避免截断 配置开关
 *  另外的策略是不受分词影响 那取决与检索系统 可以对我的前后两个词做过滤 
 *  另外很长的未被截断的话 一般是水串
 * 
 *  或者最后只对串分词
 * 
 *  1. 文本修整
 *     前后空格处理 trim  如果之后长度不符合过滤  //输入前已经处理
 *  2. 过滤 
 *     2.1 特定格式的串豁免 比如重复度高的  啊阿阿阿啊  在一起在一起在一起
 *         要先过滤掉其它所有字符 只要中文？
 *     2.2 白词豁免 匹配到白库里面的词 那么过滤掉 不处理  比如 十五字 混经验 乔粱宇 大水怪
 *  3. 排序
 *     TODO 按照串文本打分 排序 -> 按照长度排序 -> 按照freq排序
 *     现在
 *     判断串是否包含嫌疑词 比如 淘宝 兼职 头像 美容 双眼皮 交流群
 *     按照嫌疑词命中数目多少排序  -> 按照长度排序(或者分词后的term数目) -> 按照freq排序
 * 
 *     TODO 嫌疑模式 比如 到××吧 
 *     另外考虑串的相似度  比如   ABCDEFG    BCDEFGH 选取一个就可以了 选黑词多的  TODO
 *     分别去头尾部  判断是否一样 反过来 再判断一次
 * 
 * 问题 多个相同文本 每个都要再走这个流程 速度
 * 另外rsc模块 打分可能还要再单独使用更多文本特征 而不是文本打分作为一个单独特征 重复计算
 * 是否都在rsc模块进行后处理？ TODO
 * 合并并且采用openmpi ?
 *  ==============================================================================
 */

#ifndef POST_PROCESSOR_H_
#define POST_PROCESSOR_H_

#include "RstreeFilter.h"
#include "common_util.h"
#include "conf_util.h"
#include "debug_util.h"
#include "string_util.h"
#include <glog/logging.h>
#include "MatchDict.h"
#include "RegexSearcher.h"
#include <functional>
//#include <boost/bind.hpp>
#include "rstree_def.h"
#include "pcre.h"

const int OVECCOUNT = 100;
const std::string weixin_qq = "(((微|薇|徵|威|溦|徽|v|wei)(亻言|信|xin))|"
                           "(((\\+|加)(微|薇|徵|威|溦|徽|v(?!ip|pn)))(亻言|信|xin|x)?)|"
                           "(((\\+|加)(我|他|她)?(v(?!ip|pn)|微|薇|徵|威|溦|徽))(亻言|信|xin|x)?)|"
                           "((vx|wx)(\\:|：|号))|((薇|溦|徽|嶶|徵|v|wei)(我|他|她|号))|"
                           "([扣扣|球球|秋秋|囚囚|蔻蔻|抠抠|叩叩|扣|σσ|óó|θθ|φφ|qq|QQ|企鹅|Q|q]))"
                           "[号码|号|:|：|\\s]*"
                           "([\\w]{5,15})";
const std::string seem_black = "(【(.{6,})】)|(<B>(.{6,})<B>)|(<em>(.{6,})<\\\\em>)|"
                                 "(\\{(.{6,})\\})|(\\((.{6,})\\))";
//std::string ReProcesser::m_weixin = "((徽|薇|溦|嶶|v|wei)(亻言|信|xin))|((\\+|加)(徽|薇|溦|嶶|徵|v(?!ip|pn)))|((\\+|加)(我|他|她)?(v(?!ip|pn)|微|薇|徵|威|溦|徽))|(wx(\\:|：|号))|((薇|溦|徽|嶶|徵|v|wei)(我|他|她|号))";
class PostProcessor
{
public:

    PostProcessor()
    : _max_match_count(1000)
    {

    }

    virtual ~PostProcessor()
    {
        if (_result)
        {
            dm_pack_del(_result);
        }
    }

    bool init()
    {
        return init(SharedConf::conf());
    }

    //如果是同一个类 多个线程只读 如果内存占用不大 就是成员变量 如果内存占用大 就用static init_static,如果是多个类都需要访问的
    //用指针 或者 全局 或者 单例子

    static bool init_static()
    {
        return init_static(SharedConf::conf());
    }
    //进程加载公共资源  TODO多线程环境需要2个init分开 init 线程级别 init_dict(or init_static()) 进程级别

    static bool init_static(comcfg::Configure& conf, string section = "PostProcessor")
    {
        string black_file = "./data/black.dm";
        CONF(black_file);
        _black_dict.init(black_file);
    }

    bool init(comcfg::Configure& conf, string section = "PostProcessor")
    {
        {
            CONF(_max_match_count);
            _result = dm_pack_create(_max_match_count);
            CHECK_NOTNULL(_result);
        }
        {
            string black_pattern_file = "./data/black.pattern.txt";
            CONF(black_pattern_file);
            _black_reg_searcher.init2(black_pattern_file);
        }
        {
            _white_filter.init();
        }

        {
            const char* p_err_msg = NULL;
            int offset = -1;
            _weixin_qq = pcre_compile(weixin_qq.c_str(), 0, &p_err_msg, &offset, NULL);
            CHECK_NOTNULL(_weixin_qq);
            offset = -1;
            _seem_black = pcre_compile(seem_black.c_str(), 0, &p_err_msg, &offset, NULL);
            CHECK_NOTNULL(_seem_black);
        }

        return true;
    }

    string filter(string input)
    {
        string s = input;
        boost::to_lower(s);
        return gezi::filter_str(s);
    }

    int black_process(const vector<INode>& ivec, vector<ONode>& ovec)
    {
        int black_count = 0;
        //for (auto &item : ivec)
        for (int i = 0; i < (int) ivec.size(); i++)
        {
            const INode& item = ivec[i];
            ONode node(item.str, item.wstr, item.count);
            node.filtered_str = filter(node.str);
            //--------------------黑词匹配
            //      Pval(node.str);
            //      Pval(node.filtered_str);
            node.black_count = _black_dict.search_count(node.filtered_str, _result);
            //      Pval(node.black_count);
            //      if (node.black_count > 0)
            //      {
            //        Pval(node.str);
            //        Pval(_result->ppseg_cnt);
            //        Pval(_result->poff[0]);
            //        Pval(_result->ppseg[0]->len);
            //        Pval(_result->ppseg[0]->pstr);
            //      }

            //--------------------黑模版匹配
            if (_black_reg_searcher.has_match(node.wstr))
            {
                node.black_count++;
            }

            if (node.black_count > 0)
            {
                black_count++;
            }

            //      Pval(node.black_count);
            ovec.push_back(node);
        }
        return black_count;
    }

    struct Cmp
    {

        bool operator() (const ONode& left, const ONode& right)
        {
            if (left.black_count == right.black_count)
            {
                if (left.str.length() >= MIN_GOOD_LENGTH && right.str.length() >= MIN_GOOD_LENGTH)
                {
                    if (left.count == right.count)
                    {
                        return left.str.length() > right.str.length();
                    }
                    else
                    {
                        return left.count > right.count;
                    }
                }
                else
                {
                    if (left.str.length() == right.str.length())
                    {
                        return left.count > right.count;
                    }
                    else
                    {
                        return left.str.length() > right.str.length();
                    }
                }
            }
            else
            {
                return left.black_count > right.black_count;
            }
        }
    };

    void process(const vector<INode>& ivec, vector<ONode>& vec, int max_count)
    {
        int black_count = black_process(ivec, vec);

        if (black_count < max_count)
        {
            vec = _white_filter.process(vec);
        }

        PrintVec(vec, str);
        if (vec.size() > max_count)
        {
            std::partial_sort(vec.begin(), vec.begin() + max_count, vec.end(), Cmp());
            vec.resize(max_count);
        }
        else
        {
            std::sort(vec.begin(), vec.end(), Cmp());
        }
    }

    //for test

    void process(const vector<Pair>& ivec, int max_count, vector<ONode>& vec)
    {
        vector<INode> rvec;
        //    for (auto &item : ivec)
        for (int i = 0; i < (int) ivec.size(); i++)
        {
            const Pair& item = ivec[i];
            rvec.push_back(INode(item.first, str_to_wstr(item.first), item.second));
        }
        process(rvec, vec, max_count);
    }

    //获取第一个spam

    string get_spam(string input)
    {
        int count = _black_dict.search_count(input, _result);
        if (count > 0)
        {
            return _result->ppseg[0]->pstr;
        }

        return "";
    }
    string get_spam_weixin_qq(const string& input)
    {
        std::string ret = "";
        int ovector[OVECCOUNT] = {0};
        int rc = pcre_exec(_weixin_qq, NULL, input.c_str(), input.size(), 0, 0, ovector, OVECCOUNT);
        cout << rc << endl;
        if (rc > 0)
        {
            cout << input << " " << input.size() << endl;
            for (int i = 0; i < rc; i++)
            {
                cout << "pair:" << ovector[i*2] << "->" << ovector[i*2+1] << endl;
            }
            int begin_pos = ovector[(rc-1) * 2];
            int end_pos   = ovector[(rc-1) * 2 + 1];
            cout << "begin_pos" << begin_pos << endl;
            cout << "end_pos" << end_pos << endl;
            ret = input.substr(begin_pos, end_pos - begin_pos);
        }
        return ret;
    }

    string get_seem_black(const string& input)
    {
        std::string ret = "";
        int ovector[OVECCOUNT] = {0};
        int rc = pcre_exec(_seem_black, NULL, input.c_str(), input.size(), 0, 0, ovector, OVECCOUNT);
        cout << rc << endl;
        if (rc > 0)
        {
            cout << input << " " << input.size() << endl;
            bool sub_match = false;
            for (int i = 1; i < rc; i++)
            {
                int begin_pos = ovector[2*i];
                int end_pos   = ovector[2*i + 1];
                if (begin_pos == -1)
                {
                    continue;
                }
                else if (sub_match)
                {
                    ret = input.substr(begin_pos, end_pos - begin_pos);
                    std::cout << "ret:" << ret << std::endl;
                }
                else
                {
                    sub_match = true;
                }
            }
        }
        return ret;
    }

    ////  //获取覆盖最多spam的区间对应的string
    // string get_spampart(string input, int max_length)
    // {
    //   
    // }

    static gezi::MatchDict& black_dict()
    {
        return _black_dict;
    }
private:
    //--------------------------黑名单匹配 
    static gezi::MatchDict _black_dict;
    dm_pack_t* _result;
    int _max_match_count; //最多可能匹配的个数
    //--------------------------黑模式匹配
    RegexSearcher _black_reg_searcher;
    //--------------------------黑未匹配的情况下 匹配白名单或者白模式豁免过滤
    RstreeFilter _white_filter;
    static const int MIN_GOOD_LENGTH = 20;

    /*
     * 提取输入文档中的疑似黑载体，包括微信 qq 和在【】中的纯中文字符
     *add by jiaxiaojian
    */
    //微信 qq等
    pcre * _weixin_qq;
    //疑似黑载体 目前主要是【】中的
    pcre * _seem_black;
};


#endif  //----end of POST_PROCESSOR_H_

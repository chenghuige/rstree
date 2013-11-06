/***************************************************************************
 * 
 * Copyright (c) 2009 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/



/**
 * @file nshead_block_talk 接口测试 
 * @author wuliping(sat@baidu.com)
 * @date 2013/02/25 18:30:06
 * @brief 
 *  
 **/
#include <stdio.h>
#include <time.h>
#include <ub_log.h>
#include <ubclient_include.h>
#include <mc_pack.h>
#include <fstream>
#include <iostream>

//#include <gflags/gflags.h>

using namespace std;

const int MC_PACK_BUF_LEN = 200000;

int min_len = 8;
int max_len = 20;
int min_freq = 15;
char input_path[2000];

void *thread_nshead_simple_singletalk(void *param)
{
  ub_log_initthread("nshead_simple_singletalk");

  ub::UbClientManager *ubmgr = (ub::UbClientManager *)(param);

  int ret = 0;
  const char * ip;
  int port;
  struct timeval s1, e1;

  ifstream fin(input_path);
  if (!fin)
  {
    cerr << "can not find input.txt" << endl;
    exit(-1);
  }

  string line;

  while (getline(fin, line))
  {

    printf("input line is [%s]\n", line.c_str());

    //申请内存，填写发送的nshead请求信息
    nshead_t reqhead;
    reqhead.log_id = rand();
    char reqbuf[] = "test simple talk";
    reqhead.body_len = sizeof (reqbuf);

    //申请内存用于存放response
    nshead_t reshead;
    reshead.log_id = reqhead.log_id;
    sprintf(reqhead.provider, "test_client");
    int recvLen = MC_PACK_BUF_LEN;
    char resbuf[recvLen];
    memset(resbuf, 0, sizeof (resbuf));

    char temp_buf[MC_PACK_BUF_LEN];
    char pack_buf[MC_PACK_BUF_LEN];

    mc_pack_t * req_pack = mc_pack_open_w(2, pack_buf, sizeof (pack_buf), temp_buf, sizeof (pack_buf));
    mc_pack_put_str(req_pack, "content", line.c_str());
   
    mc_pack_put_int32(req_pack, "min_freq", min_freq);
    mc_pack_put_int32(req_pack, "min_len", min_len);
    mc_pack_put_int32(req_pack, "max_len", max_len);

    reqhead.body_len = mc_pack_get_size(req_pack);

    mc_pack_close(req_pack);

    gettimeofday(&s1, NULL);

    ////请求nshead头信息和body信息，填写
    ret = ubmgr->nshead_simple_singletalk("rstree", -1, &reqhead, (void *) pack_buf,
            &reshead, (void*) resbuf, (u_int) recvLen);

    //  如果用户想获得本次通信是与哪个server完成的，请将ip和port传入
    /*ret = ubmgr->nshead_simple_singletalk("service1", -1, &reqhead, (void *)reqbuf, 
            &reshead, (void*)resbuf, (u_int)recvLen, &ip, &port);
     */
    if (0 != ret)
    {
      UB_LOG_WARNING("nshead_simple_singletalk failed; err:%s", ub::get_talk_errorinfo(ret));
      continue;
    }
    gettimeofday(&e1, NULL);

    mc_pack_t * res_pack = mc_pack_open_r(resbuf, reshead.body_len, temp_buf, sizeof (temp_buf));

    int error_no;
    mc_pack_get_int32(res_pack, "error_no", &error_no);
    printf("ret error_no is [%d]\n", error_no);
    mc_pack_t * array_pack = mc_pack_get_object(res_pack, "substr_array");
    if (MC_PACK_PTR_ERR(array_pack) < 0)
    {
      printf("error in get array pack [%d]\n", MC_PACK_PTR_ERR(array_pack));
    }
    int str_cnt = mc_pack_get_item_count(array_pack);

    if (str_cnt == 0)
    {
      mc_pack_close(res_pack);
      continue;
    }

    mc_pack_item_t p_item;
    int ret_no = mc_pack_first_item(array_pack, &p_item);
    if (ret_no != 0)
    {
      mc_pack_close(res_pack);
      continue;
    }

    printf("substr=[%s], count=[%d]\n", p_item.key, *(int*) (p_item.value));

    mc_pack_item_t n_item;
    mc_pack_item_t * item_now = &p_item;
    mc_pack_item_t * item_next = &n_item;

    while (mc_pack_next_item(item_now, item_next) == 0)
    {
      printf("substr=[%s], count=[%d]\n", item_next->key, *(int*) (item_next->value));
      item_now = item_next;
    }

    UB_LOG_NOTICE("nshead_simple_singletalk with server(%s:%d) Success, response:%s, consume time:%ldus, err:%s",
            ip, port, resbuf,
            (long) ((e1.tv_sec - s1.tv_sec)*1000000 + (e1.tv_usec - s1.tv_usec)),
            ub::get_talk_errorinfo(ret));

    mc_pack_close(res_pack);

    usleep(20 * 1000);

  }

  ub_log_closethread();
  return NULL;
}

int main(int argc, char ** argv)
{

  //  int s = google::ParseCommandLineFlags(&argc, &argv, false);

  if (argc != 1 && argc != 5)
  {
    cerr << "usage: " << argv[0] << " input_path min_freq min_len max_len" << endl;
    exit(-1);
  }

  sprintf(input_path, "input.txt");

  if (argc == 5)
  {
    sprintf(input_path, argv[1]);
    min_freq = atoi(argv[2]);
    min_len = atoi(argv[3]);
    max_len = atoi(argv[4]);
  }

  // 打开日志
  int ret = ub_load_comlog("./conf/", "comlog.conf");
  if (ret != 0)
  {
    UB_LOG_WARNING("ub_load_comlog() failed, %d", ret);
    return -1;
  }

  ub::UbClientManager *ubmgr = new ub::UbClientManager;
  // ubclientmanger初始化
  ret = ubmgr->init("./conf/", "client.conf");
  if (ret == 0)
  {
    UB_LOG_NOTICE("ubclientmanger init Success");
  } else
  {
    UB_LOG_WARNING("ubclientmanger init Failed");
    return -1;
  }

  const int threadNum = 1;
  pthread_t thd[threadNum];
  for (int i = 0; i < threadNum; i++)
  {
    pthread_create(&thd[i], NULL, thread_nshead_simple_singletalk, (void*) ubmgr);
  }
  for (int i = 0; i < threadNum; i++)
  {
    pthread_join(thd[i], NULL);
  }

  //销毁ubclientmanger
  delete ubmgr;

  // 关闭日志
  ub_log_close();
  return 0;

}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */

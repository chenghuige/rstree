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

void *thread_nshead_simple_singletalk(void *param)
{
    ub_log_initthread("nshead_simple_singletalk");

    ub::UbClientManager *ubmgr = (ub::UbClientManager *)(param);

    int repNum = 0;
    int ret = 0;
    const char * ip;
    int port;
    struct timeval s1, e1;

    while(1) {
        //申请内存，填写发送的nshead请求信息
        nshead_t reqhead;
        reqhead.log_id = rand();
        char reqbuf[] = "test simple talk";
        reqhead.body_len = sizeof(reqbuf);

        //申请内存用于存放response
        nshead_t reshead;
        reshead.log_id = reqhead.log_id;
        int recvLen = 20000;
        char resbuf[recvLen];
        memset(resbuf, 0, sizeof(resbuf));

        char temp_buf[20000];
        char pack_buf[20000];

        mc_pack_t * req_pack = mc_pack_open_w(2, pack_buf, sizeof(pack_buf), temp_buf, sizeof(pack_buf));
        mc_pack_put_str(req_pack, "content", "test content test title");
        mc_pack_put_int32(req_pack, "freq", 2);
        mc_pack_put_int32(req_pack, "min_len", 2);
        mc_pack_put_int32(req_pack, "max_len", 10);

        reqhead.body_len = mc_pack_get_size(req_pack);

        mc_pack_close(req_pack);

        gettimeofday(&s1, NULL);

        ////请求nshead头信息和body信息，填写
        ret = ubmgr->nshead_simple_singletalk("rstree", -1, &reqhead, (void *)pack_buf, 
                &reshead, (void*)resbuf, (u_int)recvLen);

        //  如果用户想获得本次通信是与哪个server完成的，请将ip和port传入
        /*ret = ubmgr->nshead_simple_singletalk("service1", -1, &reqhead, (void *)reqbuf, 
                &reshead, (void*)resbuf, (u_int)recvLen, &ip, &port);
        */
        if (0 != ret) {
            UB_LOG_WARNING("nshead_simple_singletalk failed; err:%s", ub::get_talk_errorinfo(ret));
            break;
        }
        gettimeofday(&e1, NULL);

        printf("ret head len is [%d]\n", reshead.body_len);
        
        mc_pack_t * res_pack = mc_pack_open_r(resbuf, reshead.body_len, temp_buf, sizeof(temp_buf));

        int pack_size = mc_pack_get_size(res_pack);
        printf("ret pack size is [%d]\n", pack_size);
        int error_no;
        mc_pack_get_int32(res_pack, "error_no", &error_no);
        printf("ret error_no is [%d]\n", error_no);
        mc_pack_t * array_pack = mc_pack_get_array(res_pack, "substr_array");
        if(MC_PACK_PTR_ERR(array_pack) < 0)
        {
            printf("error in get array pack [%d]\n", MC_PACK_PTR_ERR(array_pack));
        }
        int str_cnt = mc_pack_get_item_count(array_pack);
        printf("ret str cnt is [%d]\n", str_cnt);


        UB_LOG_NOTICE("nshead_simple_singletalk with server(%s:%d) Success, response:%s, consume time:%ldus, err:%s", 
            ip, port, resbuf, 
            (long)((e1.tv_sec-s1.tv_sec)*1000000+(e1.tv_usec-s1.tv_usec)),
            ub::get_talk_errorinfo(ret));

        repNum ++;
        if (repNum > 1000) {
            break;
        }

    }
    
    ub_log_closethread();
    return NULL;
}

void *thread_nshead_simple_singletalk_ex(void *param)
{
    ub_log_initthread("nshead_simple_singletalk_ex");

    ub::UbClientManager *ubmgr = (ub::UbClientManager *)(param);

    int repNum = 0;
    int ret = 0;
    const char * ip;
    int port;
    struct timeval s1, e1;

    while(1) {
        //申请内存，填写发送的nshead请求信息
    	char req_buff[1000];
    	memset(req_buff,0,sizeof(req_buff));

    	nshead_t *reqhead;
	    reqhead = (nshead_t*)req_buff;
	    reqhead->log_id = rand();
        reqhead->magic_num = 0xfb709394;
	
        char Msg[] = "test simple ex talk";
    	char *req_body_buf	= (char*)req_buff + sizeof(nshead_t); //跳过nshead_t部分
        reqhead->body_len = snprintf(req_body_buf, 100, "%s", Msg);

        //申请内存用于存放response
	    char res_buff[20000];
	    memset(res_buff,0,sizeof(res_buff));

        gettimeofday(&s1, NULL);

        //请求nshead头信息和body信息，填写
    	ret = ubmgr->nshead_simple_singletalk_ex("service1", -1, (void *)req_buff, (u_int)(sizeof(nshead_t) + reqhead->body_len),
		                                               (void*)res_buff, (u_int)sizeof(res_buff));

        //  如果用户想获得本次通信是与哪个server完成的，请将ip和port传入
    	/*ret = ubmgr->nshead_simple_singletalk_ex("service1", 10, (void *)req_buff, (u_int)(sizeof(nshead_t) + reqhead->body_len),
		                                       (void*)res_buff, (u_int)sizeof(res_buff), &ip, &port);
        */
        if (0 != ret) {
            UB_LOG_WARNING("nshead_simple_singletalk failed; err:%s", ub::get_talk_errorinfo(ret));
            break;
        }

        gettimeofday(&e1, NULL);
    	char *res_body_buf	= (char*)res_buff + sizeof(nshead_t); //跳过nshead_t部分

        UB_LOG_NOTICE("nshead_simple_singletalk_ex with server(%s:%d) Success, response:%s, consume time:%ldus",
            ip, port, res_body_buf, 
            (long)(e1.tv_sec-s1.tv_sec)*1000000+(e1.tv_usec-s1.tv_usec));

        repNum ++;
        if (repNum > 100000) {
            break;
        }

    }
    
    ub_log_closethread();
    return NULL;
}

void *thread_nshead_singletalk(void *param)
{
    ub_log_initthread("nshead_singletalk");

    ub::UbClientManager *ubmgr = (ub::UbClientManager *)(param);

    int repNum = 0;
    int ret = 0;
    const char * ip;
    int port;
    struct timeval s1, e1;

    while(1) {
        ub::nshead_talkwith_t *currTalk = new ub::nshead_talkwith_t;
        if (NULL == currTalk) {
            UB_LOG_WARNING("new ub::nshead_talkwith_t Failed");
            break;
        }

        char resMsg[] = "test single talk";
        currTalk->reqhead.log_id = rand();
        currTalk->reqbuf = new char[100];
        if (NULL == currTalk->reqbuf) {
            UB_LOG_WARNING("new buffer Failed");
            delete currTalk;
            break;
        }
        memset(currTalk->reqbuf, 0, 100);
        currTalk->reqhead.body_len = snprintf(currTalk->reqbuf, 100, "%s", resMsg);
    
        int resLen = 200;
        currTalk->resbuf = new char[resLen];
        if (NULL == currTalk->resbuf) {
            UB_LOG_WARNING("new buffer Failed");
            delete [] currTalk->reqbuf;
            delete currTalk;
            break;
        }
        memset(currTalk->resbuf, 0, resLen);
        currTalk->maxreslen = resLen;
		currTalk->defaultserverarg.key = -1;	

        gettimeofday(&s1, NULL);
    
        ret = ubmgr->nshead_singletalk("service1", currTalk);
        if (0 != ret || currTalk->success != 0) {
            UB_LOG_WARNING("nshead_singletalk failed; err:%s", ub::get_talk_errorinfo(ret));
            delete [] currTalk->resbuf;
            delete [] currTalk->reqbuf;
            delete currTalk;
            break;
        }

        ip = currTalk->innerTalk.server->getIP(currTalk->innerTalk.ipnum);
        port = currTalk->innerTalk.server->getPort();

        gettimeofday(&e1, NULL);
        UB_LOG_NOTICE("nshead_singletalk with server(%s:%d) Success, response:%s, consume time:%ldus",
            ip, port, currTalk->resbuf, 
            (long)(e1.tv_sec-s1.tv_sec)*1000000+(e1.tv_usec-s1.tv_usec));

        delete [] currTalk->resbuf;
        delete [] currTalk->reqbuf;
        delete currTalk;

        repNum ++;
        if (repNum > 100000) {
            break;
        }
    }
    ub_log_closethread();
    return NULL;
}

void *thread_nshead_talk(void *param)
{
    ub_log_initthread("nshead_talk");

    ub::UbClientManager *ubmgr = (ub::UbClientManager *)(param);

    int repNum = 0;
    const char * ip;
    int port;

    int talkNum = 5;
    ub::nshead_talkwith_t **currTalk = NULL;

    while(1) {
        currTalk = new ub::nshead_talkwith_t*[talkNum];
        if (NULL == currTalk) {
            break;
        }
        memset(currTalk, 0, sizeof(currTalk));

        char resMsg[50];
        snprintf(resMsg, 50, "%s", "`~!@#$%^&*()_-+={}[];:’");

        bool alloc_fail = false;
        for (int i=0; i<talkNum; i++) {
            currTalk[i] = new ub::nshead_talkwith_t;
            if (NULL == currTalk[i]) {
                alloc_fail = true;
                break;
            }   

            currTalk[i]->reqhead.log_id = rand();
            currTalk[i]->reqbuf = new char[100];
            if (NULL == currTalk[i]->reqbuf) {
                alloc_fail = true;
                break;
            }
            memset(currTalk[i]->reqbuf, 0, 100);
            currTalk[i]->reqhead.body_len = snprintf(currTalk[i]->reqbuf, 100, "%s", resMsg);

            int maxreslen = 100;
            currTalk[i]->resbuf = new char[maxreslen];
            if (NULL == currTalk[i]->resbuf) {
                alloc_fail = true;
                break;
            }
            memset(currTalk[i]->resbuf, 0, maxreslen);
            currTalk[i]->maxreslen = maxreslen;

            currTalk[i]->defaultserverarg.key = -1;	
        }
        if (alloc_fail)
            goto Failed;

        int ret = ubmgr->nshead_talk("service1", currTalk, talkNum);
        if (ret != 0)
            goto Failed;

        for (int i=0; i<talkNum; i++) {
            if (currTalk[i]->success != 0) {
                UB_LOG_WARNING("nshead_simple_singletalk failed; err:%s",
                    ub::get_talk_errorinfo(currTalk[i]->success));
                goto Failed;
            } else {
                ip = currTalk[i]->innerTalk.server->getIP(currTalk[i]->innerTalk.ipnum);
                port = currTalk[i]->innerTalk.server->getPort();

                UB_LOG_NOTICE("nshead_talk with server(%s:%d) Success, response:%s", 
                    ip, port, currTalk[i]->resbuf); 
            }
            delete []currTalk[i]->resbuf;
            delete []currTalk[i]->reqbuf;
            delete currTalk[i];
        }
        delete [] currTalk;
        currTalk = NULL;

        repNum ++;
        if (repNum > 100000) {
            break;
        }
    }
    ub_log_closethread();
    return NULL;

Failed:
    if(currTalk != NULL) {
        for (int i = 0;i < talkNum; i++) {
            if (currTalk[i] != NULL)  {
                if(currTalk[i]->resbuf != NULL)
                    delete []currTalk[i]->resbuf;
                if(currTalk[i]->reqbuf != NULL)
                    delete []currTalk[i]->reqbuf;
                delete currTalk[i];
            }
        }
        delete [] currTalk;
    }

    ub_log_closethread();
    return NULL;
}

int main(int /*argc*/, char ** /*argv*/)
{
    // 打开日志
    int ret = ub_load_comlog("./conf/", "comlog.conf");
    if(ret != 0) {
        UB_LOG_WARNING("ub_load_comlog() failed, %d", ret);
        return -1;
    }

    ub::UbClientManager *ubmgr = new ub::UbClientManager;    
    // ubclientmanger初始化
    ret = ubmgr->init("./conf/", "client.conf");
    if (ret == 0) {
        UB_LOG_NOTICE("ubclientmanger init Success");
    } else {
        UB_LOG_WARNING("ubclientmanger init Failed");
        return -1;
    }

    const int threadNum = 15;
    pthread_t thd[threadNum];
    for(int i = 0; i < threadNum; i++) {
        pthread_create(&thd[i], NULL, thread_nshead_simple_singletalk, (void*)ubmgr);
    }
    for (int i = 0; i < threadNum; i++) {
        pthread_join(thd[i], NULL);
    }

    for(int i = 0; i < threadNum; i++) {
        pthread_create(&thd[i], NULL, thread_nshead_simple_singletalk_ex, (void*)ubmgr);
    }
    for (int i = 0; i < threadNum; i++) {
        pthread_join(thd[i], NULL);
    }

    for(int i = 0; i < threadNum; i++) {
        pthread_create(&thd[i], NULL, thread_nshead_singletalk, (void*)ubmgr);
    }
    for (int i = 0; i < threadNum; i++) {
        pthread_join(thd[i], NULL);
    }

    for(int i = 0; i < threadNum; i++) {
        pthread_create(&thd[i], NULL, thread_nshead_talk, (void*)ubmgr);
    }
    for (int i = 0; i < threadNum; i++) {
        pthread_join(thd[i], NULL);
    }
    
    //销毁ubclientmanger
    delete ubmgr;

    // 关闭日志
    ub_log_close();
    return 0;

}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */

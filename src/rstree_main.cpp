#include "ub.h"
#include "cfgflag.h"
#include "UbMonitor.h"
#include "ub_server_monitor.h"
#include <bsl/var/String.h>

#include "mc_pack.h"
#include "rstree.h"
#include "conf.h"
#include "rstree_def.h"
#include "rstree_util.h"

bsl::var::Ref g_monitor_info;
char g_proc_name[1024];
rstree_t *g_rstree;


bsl::var::IVar &get_service_monitor_list(bsl::ResourcePool &rp)
{
    return g_monitor_info;
}
void get_proc_name() 
{
    pid_t pid = getpid();
    char exec_file[1024];
    char proc_name[1024]; 
    snprintf(exec_file, sizeof(exec_file), "/proc/%d/exe", pid); 
    int ret = readlink(exec_file, proc_name, sizeof(proc_name)); 
    proc_name[ret] = 0; 
    //��ȡ�ļ��� 
    char *p = strrchr(proc_name, '/'); 
    snprintf(g_proc_name, sizeof(g_proc_name), "%s", p + 1); 
}

/**
 * @brief����׺�������ı���Ԥ����
 *
 */
static string filter(const char * input)
{
	string s(input);
	s = str_replace_all(s, " ", "");	

	return s;
}

/**
 *@update server
**/
static int update_server_callback()
{
	nshead_t *req_head = (nshead_t *) ub_server_get_read_buf();
	nshead_t *res_head = (nshead_t *) ub_server_get_write_buf();

	int req_body_len = req_head->body_len;
	char *req_buf = (char*) (req_head + 1);
	req_buf[req_body_len] = 0;

	ub_log_setbasic(UB_LOG_REQSVR, "%s", req_head->provider);
	ub_log_setbasic(UB_LOG_SVRNAME, "%s", "rstree");

	res_head->log_id = req_head->log_id;

	char temp_buf[MC_PACK_BUF_SIZE];
	mc_pack_t *req_pack = mc_pack_open_r(req_buf, req_head->body_len, temp_buf, sizeof(temp_buf));

	int error_no = 0;
	const char* content = mc_pack_get_str(req_pack, "content");

	if(MC_PACK_PTR_ERR(content) < 0)
	{
		error_no = -1;
	}

	int freq, min_len, max_len;
	int ret_no = 0;
	ret_no |= mc_pack_get_int32(req_pack, "freq", &freq);
	ret_no |= mc_pack_get_int32(req_pack, "min_len", &min_len);
	ret_no |= mc_pack_get_int32(req_pack, "max_len", &max_len);

	if(ret_no != 0)
	{
		error_no = -1;
	}

	mc_pack_close(req_pack);

	map<wstring, int> ret_map;

	if(error_no == 0)
	{
		g_rstree->set_min_frequency(freq);
		g_rstree->set_min_substr_len(min_len);
		g_rstree->set_max_substr_len(max_len);

		string fcontent = filter(content);

		wstring wc = c2w(fcontent.c_str());

		if((int)wc.size() < g_rstree->get_min_str_len() || (int)wc.size() > g_rstree->get_max_str_len())
		{
			error_no = -3;	
		}
		else
		{
			ret_map = g_rstree->add_text(wc);

			if(g_rstree->get_tree_size() >= g_rstree->get_max_tree_size())
			{
				int ret_no = g_rstree->remove_text();
				if(ret_no != 0)
				{
					ul_writelog(UL_LOG_WARNING, "failed to remove text, ret_no=[%d]", ret_no);
					error_no = -2;
				}	
				else
				{
					ul_writelog(UL_LOG_TRACE, "remove text done!");
				}
			}

//			g_rstree->print_tree();
		}
	}

	char mcpack_buf[MC_PACK_BUF_SIZE];
	mc_pack_t *ret_pack = mc_pack_open_w(2, mcpack_buf, sizeof(mcpack_buf), temp_buf, sizeof(temp_buf));
	mc_pack_t *array_pack = mc_pack_put_array(ret_pack, "substr_array");
	for(map<wstring, int>::iterator iter = ret_map.begin(); iter != ret_map.end(); iter++)
	{
		string substr = w2c(iter->first.c_str());
		mc_pack_put_int32(array_pack, substr.c_str(), iter->second);
		ul_writelog(UL_LOG_TRACE, "result substring[%s] freq[%d]", substr.c_str(), iter->second);
		ul_writelog(UL_LOG_DEBUG, "array pack size is [%d]", mc_pack_get_item_count(array_pack));

		if(mc_pack_get_size(ret_pack) + 50000 > (int)ub_server_get_write_size())
		{
			break;
		}
	}

	mc_pack_put_int32(ret_pack, "error_no", error_no);
	mc_pack_close(ret_pack);

	char *res_buf = (char*)(res_head + 1);

	res_head->body_len = mc_pack_get_size(ret_pack);
	memcpy(res_buf, mcpack_buf, res_head->body_len);

	printf("ret pack size is [%d]\n", res_head->body_len);

	return 0;
}

/**
 *@brief timer
**/
static int monitor_timer(void *)
{
	ul_writelog(UL_LOG_NOTICE, "call timer");
	return 0;
}

static void app_init()
{
	setlocale(LC_ALL, "zh_CN.UTF-8");
	int tree_size = -1;
	Conf::getSharedInstance()->getConfValueByKey("tree_size", CONF_VALUE_TYPE_INT32, (void*)(&tree_size), sizeof(int));
	if(tree_size == -1)
	{
		tree_size = DEFAULT_TREE_SIZE;
		ul_writelog(UL_LOG_WARNING, "can not find conf tree_size, use default value [%d]", DEFAULT_TREE_SIZE);
	}

	int min_str_len = -1;
	Conf::getSharedInstance()->getConfValueByKey("min_str_len", CONF_VALUE_TYPE_INT32, (void*)(&min_str_len), sizeof(int));
	if(min_str_len == -1)
	{
		min_str_len = DEFAULT_MIN_STR_LEN;
		ul_writelog(UL_LOG_WARNING, "can not find conf min_str_len, use default value [%d]", DEFAULT_MIN_STR_LEN);
	}

	int max_str_len = -1;
	Conf::getSharedInstance()->getConfValueByKey("max_str_len", CONF_VALUE_TYPE_INT32, (void*)(&max_str_len), sizeof(int));
	if(max_str_len == -1)
	{
		max_str_len = DEFAULT_MAX_STR_LEN;
		ul_writelog(UL_LOG_WARNING, "can not find conf max_str_len, use default value [%d]", DEFAULT_MAX_STR_LEN);
	}

	g_rstree = new rstree_t();
	g_rstree->set_tree_size(tree_size);
	g_rstree->set_min_str_len(min_str_len);
	g_rstree->set_max_str_len(max_str_len);

	ul_writelog(UL_LOG_NOTICE, "init suffix tree done, tree_size=[%d] min_str_len=[%d] max_str_len=[%d]", tree_size, min_str_len, max_str_len);

}

static void app_close()
{

}

/**
 * @brief ע��main������֧�������в�������
 * ������ ./xxxx -h ��ʾ����
**/
int main(int argc, char **argv)
{
	ub_t * fw = ub_init("rstree", argc, argv);	//��ʼ�����
	if (fw == NULL) { 	//����0��ʾҪ�˳�
		return 0; 
	}
	//comcfg::CmdOption opt;//����һ�����������е���
	//opt.init(argc, argv, "");//��֪��opt��ôʹ�õĲο�public/configure��ʹ��
	//opt["�����е�key��"].to_cstr();//�����ַ����������֪���Ƿ���ڣ��ȵ���opt.hasOption(char *)�ж�һ��

	ub_conf_data_t *conf = ub_open_conf(fw);	//�������ļ�
	UBFW_ASSERT(conf != NULL, "open configure %s/%s error", fw->path, fw->file);

	ubfw_open_galileo_log(fw);  //��Galileo��־
	//����comlog
	UBFW_ASSERT(ub_load_log(fw) == 0, "open log error");
	ub_svr_vec_t *svrvec = ub_svr_vec_create();	//���᷵��NULL�����û�ڴ��ˣ������Զ��˳�
	UBFW_ASSERT(svrvec != NULL, "create ub_server_vec_error");
	//��������
	
	Conf::getSharedInstance()->openConfFile(fw->path, TREE_CONF_FILE);

	app_init();
	
	ub::UbMonitor *monitor = ub_load_monitor(fw);
	UBFW_ASSERT(fw->conf_build || monitor != NULL, "create UbMonitor error ");

	bsl::ResourcePool rp;
	g_monitor_info = rp.create<bsl::var::Dict>();
	get_proc_name();
	g_monitor_info[g_proc_name] = rp.create<bsl::var::Dict>();
	g_monitor_info[g_proc_name]["__global"] = rp.create<bsl::var::Dict>();
	g_monitor_info[g_proc_name]["__global"]["RUNNING_TIME"] = rp.create<bsl::var::String>("ģ������ʱ��"); 
	g_monitor_info[g_proc_name]["__global"]["PROGRAM_VERSION"] = rp.create<bsl::var::String>("����汾��");
	g_monitor_info[g_proc_name]["__global"]["LIB_VERSION"] = rp.create<bsl::var::String>("���ӿ�汾"); 
	g_monitor_info[g_proc_name]["__global"]["DATA_VERSION"] = rp.create<bsl::var::String>("���ݰ汾"); 

	int max_server_num = 0;    	//����ubserver	update
	ub_server_t *updatesvr = ub_load_svr(fw, "update");	//���� update server
	UBFW_ASSERT(fw->conf_build || updatesvr != NULL, "create update server error");
	ub_server_setoptsock(updatesvr, UBSVR_NODELAY);
	ub_server_set_callback(updatesvr, update_server_callback);	//���÷������ص�
	ub_svr_vec_add(svrvec, updatesvr, fw);	//��server��ӵ�������ͳһ����
	ub_server_set_monitor(updatesvr, monitor);
	g_monitor_info[g_proc_name][ub_server_get_server_name(updatesvr)] = get_monitor_list(rp);
	if (max_server_num < ub_server_get_threadnum(updatesvr)) {
		max_server_num = ub_server_get_threadnum(updatesvr);
	}

	monitor->set_query_item_callback(get_service_monitor_list);	//����ubclient
	//��������д��Ҫ���ص�����

	//���ü������, ������Զ���������ģʽ���������Ӧ���˳�������
	ub_builddone_exit(fw);

	UBFW_ASSERT(ub_svr_vec_run(svrvec) == 0, "run server vectors error");	//����server
	ub_timer_task_t *timer = ub_create_timer_task();
	UBFW_ASSERT(timer!=0, "create timer task error");
	UBFW_ASSERT(ub_add_timer_task(timer, monitor_timer, NULL, 10000/*ms*/) == 0, 
			"start monitor callback");
	UBFW_ASSERT(ub_run_timer_task(timer) >0, "run timer error");



	monitor->run();
	ub_svr_vec_join(svrvec);	//�ȴ�server
	monitor->join(); 
	ub_svr_vec_destroy(svrvec);	//����server
	ub_join_timer_task(timer);	//�ȴ���ʱ��
	ub_destroy_timer_task(timer);	//���ٶ�ʱ��

	ub_log_close();
	ubfw_close_galileo_log();

	app_close();

	ub_close(fw);
	return 0;
}

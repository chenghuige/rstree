#include "ub.h"
#include "cfgflag.h"
#include "UbMonitor.h"
#include "ub_server_monitor.h"
#include <bsl/var/String.h>

#include "mc_pack.h"
#include "rstree.h"
#include "conf.h"
#include "rstree_def.h"

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

static int query_process(mc_pack_t * in_pack, mc_pack_t * out_pack)
{

	return 0;	
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

	char *res_buf = (char*)(res_head + 1);

	size_t res_len = ub_server_get_write_size() - sizeof(nshead_t);
	res_head->body_len = snprintf(res_buf, res_len, "%s", req_buf);


	ub_log_setbasic(UB_LOG_REQSVR, "%s", req_head->provider);
	ub_log_setbasic(UB_LOG_SVRNAME, "%s", "rstree");

	char temp_buf[20000];
	mc_pack_t *req_pack = mc_pack_open_r(req_buf, req_head->body_len, temp_buf, sizeof(temp_buf));

	const char* content = mc_pack_get_str(req_pack, "content");
	int freq, len;
	mc_pack_get_int32(req_pack, "freq", &freq);
	mc_pack_get_int32(req_pack, "len", &len);

	printf("req content:[%s]\n", content);
	printf("req freq:[%d]\n", freq);
	printf("req len:[%d]\n", len);

	mc_pack_close(req_pack);

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
	int tree_size;
	Conf::getSharedInstance()->getConfValueByKey("tree_size", CONF_VALUE_TYPE_INT32, (void*)(&tree_size), sizeof(int));

	int min_str_len;
	Conf::getSharedInstance()->getConfValueByKey("min_str_len", CONF_VALUE_TYPE_INT32, (void*)(&min_str_len), sizeof(int));

	g_rstree = new rstree_t();
	g_rstree->set_tree_size(tree_size);
	g_rstree->set_min_str_len(min_str_len);
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

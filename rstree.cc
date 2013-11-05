#include "ub.h"
#include "cfgflag.h"
#include "UbMonitor.h"
#include "ub_server_monitor.h"
#include <bsl/var/String.h>
#include <glog/logging.h>
#include <gflags/gflags.h>

#include <set>
#include <cmath>
#include <iostream>

#include "mc_pack.h"

#include "conf_util.h"
#include "string_util.h"

#include "rstree_def.h"

#include "Rstree.h"
#include "RstreeFilter.h"
#include "PostProcessor.h"
#include "PostAdjustor.h"
//#include "Seg.h"
#include "word_seg.h"
#include "RstreeWorker.h"


using namespace std;
using namespace gezi;
bsl::var::Ref g_monitor_info;
char g_proc_name[1024];

RstreeWorker g_worker;

DEFINE_int32(level, 0, "min log level");

extern void merge_map(map<wstring, int> &, const map<wstring, int> &);

bsl::var::IVar &get_service_monitor_list(bsl::ResourcePool &rp)
{
  return g_monitor_info;
}

void get_proc_name()
{
  pid_t pid = getpid();
  char exec_file[1024];
  char proc_name[1024];
  snprintf(exec_file, sizeof (exec_file), "/proc/%d/exe", pid);
  int ret = readlink(exec_file, proc_name, sizeof (proc_name));
  proc_name[ret] = 0;
  //��ȡ�ļ��� 
  char *p = strrchr(proc_name, '/');
  snprintf(g_proc_name, sizeof (g_proc_name), "%s", p + 1);
}
/**
 *@rstree server �ص�����
 **/
static int rstree_server_callback()
{
  nshead_t *req_head = (nshead_t *) ub_server_get_read_buf();
  nshead_t *res_head = (nshead_t *) ub_server_get_write_buf();

  int req_body_len = req_head->body_len;


  ub_log_setbasic(UB_LOG_REQSVR, "%s", req_head->provider);
  ub_log_setbasic(UB_LOG_SVRNAME, "%s", "rstree");

  res_head->log_id = req_head->log_id;
  char *req_buf = (char*) (req_head + 1);
  req_buf[req_body_len] = 0;

  char temp_buf[MC_PACK_BUF_SIZE];
  mc_pack_t *req_pack = mc_pack_open_r(req_buf, req_body_len, temp_buf, sizeof (temp_buf));

  int error_no = 0;
  const char* content = mc_pack_get_str(req_pack, "content");

  if (MC_PACK_PTR_ERR(content) < 0)
  {
    error_no = -1;
  }

  int min_freq, min_len, max_len;
  int ret_no = 0;
  ret_no |= mc_pack_get_int32(req_pack, "min_freq", &min_freq);
  ret_no |= mc_pack_get_int32(req_pack, "min_len", &min_len);
  ret_no |= mc_pack_get_int32(req_pack, "max_len", &max_len);

  if (ret_no != 0)
  {
    UB_LOG_WARNING("request format is wrong log id [%d]", res_head->log_id);
    error_no = -1;
  }

  mc_pack_close(req_pack);

  
  //-----------------------------real work here
  vector<Pair> result_vec;
  if (error_no == 0)
  {
    UB_LOG_DEBUG("logid[%d] get content[%s], min_freq %d, min_len %d, max_len %d", res_head->log_id, 
            content, min_freq, min_len, max_len);
    result_vec = g_worker.get_substrs(content, min_freq, min_len, max_len);
  }

  char mcpack_buf[MC_PACK_BUF_SIZE];
  mc_pack_t *ret_pack = mc_pack_open_w(2, mcpack_buf, sizeof (mcpack_buf), temp_buf, sizeof (temp_buf));
  mc_pack_t *array_pack = mc_pack_put_object(ret_pack, "substr_array");

  for (int i = 0; i < (int) result_vec.size(); i++)
  {
    int ret_no = mc_pack_put_int32(array_pack, result_vec[i].first.c_str(), result_vec[i].second);
    UB_LOG_TRACE("result substring[%s] freq[%d] ret_no[%d]", result_vec[i].first.c_str(), result_vec[i].second, ret_no);

    if (mc_pack_get_size(ret_pack) + 50000 > (int) ub_server_get_write_size())
    {
      break;
    }
  }

  mc_pack_put_int32(ret_pack, "error_no", error_no);
  mc_pack_close(ret_pack);

  ub_log_setbasic(UB_LOG_ERRNO, "%d", error_no);
  ub_log_pushnotice("ret_cnt", "%d", result_vec.size());
  ub_log_pushnotice("tree_size", "%d", g_worker.tree_size());

  char *res_buf = (char*) (res_head + 1);

  res_head->body_len = mc_pack_get_size(ret_pack);
  memcpy(res_buf, mcpack_buf, res_head->body_len);

  return 0;
}

/**
 *@brief timer
 **/
static int monitor_timer(void *)
{
  UB_LOG_NOTICE("call timer");
  return 0;
}

/**
 * @brief: ���������ļ��Լ�ȫ�ֱ�����ʼ��
 *
 *
 */
static void app_init()
{
  RstreeWorker::init_static();
  
  g_worker.init(); //rstree�����ǵ��߳� Ҳ��ȫ��init
}

/**
 * @brief: �ͷ�ȫ�ֱ���
 *
 */
static void app_close()
{
}

/**
 * @brief ע��main������֧�������в�������
 * ������ ./xxxx -h ��ʾ����
 **/
int main(int argc, char **argv)
{

  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  int s = google::ParseCommandLineFlags(&argc, &argv, false);
  if (FLAGS_log_dir.empty())
    FLAGS_logtostderr = true;
  FLAGS_minloglevel = FLAGS_level;

  ub_t * fw = ub_init("rstree", argc, argv); //��ʼ�����
  if (fw == NULL)
  { //����0��ʾҪ�˳�
    return 0;
  }
  //comcfg::CmdOption opt;//����һ�����������е���
  //opt.init(argc, argv, "");//��֪��opt��ôʹ�õĲο�public/configure��ʹ��
  //opt["�����е�key��"].to_cstr();//�����ַ����������֪���Ƿ���ڣ��ȵ���opt.hasOption(char *)�ж�һ��

  ub_conf_data_t *conf = ub_open_conf(fw); //�������ļ�
  UBFW_ASSERT(conf != NULL, "open configure %s/%s error", fw->path, fw->file);

  //  ubfw_open_galileo_log(fw); //��Galileo��־
  //����comlog
  UBFW_ASSERT(ub_load_log(fw) == 0, "open log error");
  ub_svr_vec_t *svrvec = ub_svr_vec_create(); //���᷵��NULL�����û�ڴ��ˣ������Զ��˳�
  UBFW_ASSERT(svrvec != NULL, "create ub_server_vec_error");
  //��������

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

  int max_server_num = 0; //����ubserver	update
  ub_server_t *updatesvr = ub_load_svr(fw, "update"); //���� update server
  UBFW_ASSERT(fw->conf_build || updatesvr != NULL, "create update server error");
  ub_server_setoptsock(updatesvr, UBSVR_NODELAY);
  ub_server_set_callback(updatesvr, rstree_server_callback); //���÷������ص�
  ub_svr_vec_add(svrvec, updatesvr, fw); //��server��ӵ�������ͳһ����
  ub_server_set_monitor(updatesvr, monitor);
  g_monitor_info[g_proc_name][ub_server_get_server_name(updatesvr)] = get_monitor_list(rp);
  if (max_server_num < ub_server_get_threadnum(updatesvr))
  {
    max_server_num = ub_server_get_threadnum(updatesvr);
  }

  monitor->set_query_item_callback(get_service_monitor_list); //����ubclient
  //��������д��Ҫ���ص�����

  //���ü������, ������Զ���������ģʽ���������Ӧ���˳�������
  ub_builddone_exit(fw);

  UBFW_ASSERT(ub_svr_vec_run(svrvec) == 0, "run server vectors error"); //����server
  ub_timer_task_t *timer = ub_create_timer_task();
  UBFW_ASSERT(timer != 0, "create timer task error");
  UBFW_ASSERT(ub_add_timer_task(timer, monitor_timer, NULL, 10000/*ms*/) == 0,
          "start monitor callback");
  UBFW_ASSERT(ub_run_timer_task(timer) > 0, "run timer error");



  monitor->run();
  ub_svr_vec_join(svrvec); //�ȴ�server
  monitor->join();
  ub_svr_vec_destroy(svrvec); //����server
  ub_join_timer_task(timer); //�ȴ���ʱ��
  ub_destroy_timer_task(timer); //���ٶ�ʱ��

  ub_log_close();
  //  ubfw_close_galileo_log();

  app_close();

  ub_close(fw);
  return 0;
}

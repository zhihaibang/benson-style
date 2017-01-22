#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <string>
#include <fstream>

using namespace std;

pid_t ForkWorker();

struct WorkerHandler
{
    int (*init_handler)(int);//后面括号内是传入参数类型
    int (*interval_handler)(void);
    int (*fini_handler)(void);
    int interval_time;
};

//主进程
int main(int argc, char const *argv[])
{
	if (!access("./pid", F_OK))
	{
		printf("error: master_process server is already running!\n");
		return -1;
	}

	ofstream ofs;
	ofs.open("./pid");
	ofs << getpid();
	ofs.close();

	prctl(PR_SET_NAME, "master_process", NULL, NULL, NULL);
	printf("master_process: server started!\n");

	pid_t pid = ForkWorker();

	if (pid < 0)
	{
		printf("master_process: failed to fork worker_process!\n");
		goto __ex;
	}
	
	ofs.open("./pid",ios::app);//追加形式打开
	ofs << "\n" << pid;
	ofs.close();
	
	printf("master_process: success to fork worker_process!\n");

	while (1)
	{
		if (waitpid(pid, NULL, WNOHANG))
		{
			pid = ForkWorker();
			if (pid < 0) 
			{
				printf("master_process: failed to refork worker_process!\n");
			}
			else if (pid > 0)
			{
				printf("master_process: success to refork worker_process!\n");
			}
		}
		sleep(5);
	}

__ex:
	printf("master_process: server stopped!\n");
	return 0;
}

pid_t ForkWorker()
{
	pid_t pid = fork();
	if (pid)
	{
		return pid;
	}
	
	prctl(PR_SET_NAME, "worker_counter", NULL, NULL, NULL);
	
    void *dl_fd = NULL;
    void *handler = NULL;

	string base_path = "./module/";
	string module_name = "libcounter.so";
	string module_path = base_path + module_name;
	
	WorkerHandler worker_handler;

    dl_fd = dlopen(module_path.c_str(), RTLD_LAZY);
    if (!dl_fd)
    {
        printf("[module_name: %s] failed to load library[%s] errMsg:[%s]\n", module_name.c_str(), module_name.c_str(), dlerror());
        exit(-1);
    }
    else
    {
        printf("[module_name: %s] success to load library[%s]\n", module_name.c_str(), module_name.c_str());
    }

    handler = dlsym(dl_fd, "handle_init");
    if (!handler)
    {
        printf("[module_name: %s] notice no sym[handle_init]\n", module_name.c_str());
        worker_handler.init_handler = NULL;
    }
    else
    {
        printf("[module_name: %s] success to load sym[handle_init]\n", module_name.c_str());
        worker_handler.init_handler = (int (*)(int))handler;
    }

    handler = dlsym(dl_fd, "handle_interval");
    if (!handler)
    {
        printf("[module_name: %s] notice no sym[handle_interval] errMsg:[%s]\n", module_name.c_str(), dlerror());
        exit(-1);
    }
    else
    {
        printf("[module_name: %s] success to load sym[handle_interval]\n", module_name.c_str());
        worker_handler.interval_handler = (int (*)(void))handler;
        worker_handler.interval_time = 3;//每隔3s执行一次
    }

    handler = dlsym(dl_fd, "handle_fini");
    if (!handler)
    {
        printf("[module_name: %s] notice no sym[handle_fini]\n", module_name.c_str());
        worker_handler.fini_handler = NULL;
    }
    else
    {
        printf("[module_name: %s] success to load sym[handle_fini]\n", module_name.c_str());
        worker_handler.fini_handler = (int (*)(void))handler;
    }
 
    printf("[module_name: %s] <<<<<<<<<  begin loop  >>>>>>>>>>>>\n", module_name.c_str());

    int start = 3,ret = 0;
    if (NULL != worker_handler.init_handler)
    {
        ret = worker_handler.init_handler(start);
        if (ret)
        {
            printf("[module_name: %s] init failed\n", module_name.c_str());
            exit(ret);
        }
    }

    while (1)
    {      
        ret = worker_handler.interval_handler();
        if (0 != ret)
        {
            printf("[module_name: %s] interval process failed\n", module_name.c_str());
            break;
        }
        sleep(worker_handler.interval_time);
    }

    if (NULL != worker_handler.fini_handler)
    {
        ret = worker_handler.fini_handler();
        if (ret)
        {
            printf("[module_name: %s] fini failed\n", module_name.c_str());
        }
    }

    printf("[module_name: %s] <<<<<<<<<  end loop  >>>>>>>>>>>>\n", module_name.c_str());

    dlclose(dl_fd);
    exit(ret);
}
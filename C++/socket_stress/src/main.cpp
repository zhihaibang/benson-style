#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/prctl.h>
#include <signal.h>
#include <sys/time.h>

#include "Client.h"
#include "config_interface.h"
#include "ILog.h"
#include "log_interface.h"

using namespace std;

int StressWorker();
int TcpWorker();
void StatResult();
int LogLevel(const string& name);

string g_ip;
int g_port;
SOCKET_TYPE g_type;
int g_socket_num;
int g_succ_count = 0;
int g_fail_count = 0;
int g_stress_time = 0;


int main(int argc, char const *argv[])
{
	if (CONFIG_LOAD("../etc/config.ini"))
	{		
		printf("error: failed to load ini file[../etc/config.ini]");
		return -1;
	}

	string socket_type = CONFIG_GETSTR("config", "socket_type");
	g_stress_time = CONFIG_GETINT("config", "stress_time");

	if(socket_type == "TCP_SOCK")
	{
		g_type = TCP_SOCK;
	}
	else if(socket_type == "UDP_SOCK")
	{
		g_type = UDP_SOCK;
	}
	else if(socket_type == "UNIX_STREAM_SOCK")
	{
		g_type = UNIX_STREAM_SOCK;
	}
	else if(socket_type == "UNIX_DGRAM_SOCK")
	{
		g_type = UNIX_DGRAM_SOCK;
	}
	else
	{
		ERROR_LOG("error: [../etc/config.ini] socket_type error");
		return -1;
	}

	g_ip = CONFIG_GETSTR(socket_type, "ip");
	g_port = CONFIG_GETINT(socket_type, "port");
	g_socket_num = CONFIG_GETINT(socket_type, "socket_num");

	TRACE_LOG("success to load config file [../etc/config.ini]");

	int log_level = LogLevel(CONFIG_GETSTR("log_config", "log_level"));
	LOG_INIT("../log/socketstress.log", log_level);
	INFO_LOG("server started!");

	if(g_socket_num > MAXSOCKETNUM)
	{
		WARNING_LOG("socket_num greater than %d", MAXSOCKETNUM);
		return -1;
	}

	int ret = StressWorker();
	if(ret == 0)
	{
		StatResult();
	}
	return 0;
}


int StressWorker()
{
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigaction(SIGPIPE, &sa, 0 );
	
	int ret = -1;
	switch(g_type)
	{
		case TCP_SOCK:
			ret = TcpWorker();
			if(ret != 0)
			{
				return -1;
			}
			break;
		case UDP_SOCK:
			break;
		case UNIX_STREAM_SOCK:
			break;
		case UNIX_DGRAM_SOCK:
			break;
		default:
			break;
	}
	return 0;
}

int TcpWorker()
{
	Client* client = new Client(g_type,g_ip,g_port,g_socket_num);
	int ret = client->createClient();

	if(ret != 0)
	{		
		ERROR_LOG("fail to create a client");
		return -1;
	}

	INFO_LOG("success to create a client");

	ret = client->connectServer();
	if(ret != 0)
	{
		ERROR_LOG("fail to connect server");
		return -1;
	}

	INFO_LOG("success to connect server");

	ret = client->createEpoll();

	if(ret != 0)
	{
		ERROR_LOG("fail to create epoll");
		return -1;
	}
	INFO_LOG("success to create epoll");
	INFO_LOG("start to send message!......");

	client->writeToServer(g_stress_time, &g_succ_count, &g_fail_count);

	return 0;
}


void StatResult()
{
	int total_count = g_succ_count + g_fail_count;
	int num_per_second = g_succ_count/g_stress_time;
 	INFO_LOG("测试时间: %ds", g_stress_time);
 	INFO_LOG("total_count = %d, succ_count = %d, fail_count = %d", total_count, g_succ_count, g_fail_count);
 	INFO_LOG("每秒吞吐量 = %d", num_per_second);
}

int LogLevel(const string& name)
{
	int ret = 0;

	if (name == "error") 
	{
		ret = LOG_LEV_ERROR;
	} 
	else if (name == "info") 
	{
		ret = LOG_LEV_INFO;
	} else if (name == "warning") 
	{
		ret = LOG_LEV_WARNING;
	} else if (name == "debug") 
	{
		ret = LOG_LEV_DEBUG;
	} else 
	{
		ret = LOG_LEV_TRACE;
	}

	return ret;
}
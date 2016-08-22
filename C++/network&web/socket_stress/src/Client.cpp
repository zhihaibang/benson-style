#include <sys/epoll.h>
#include <string>
#include <string.h>
#include <sys/time.h>
#include "Socket.h"
#include "Client.h"
#include "ILog.h"
#include "log_interface.h"

using namespace std;

Client::Client(SOCKET_TYPE type, string ip, int port, int socket_num)
{
	type_ = type;
	ip_ = ip;
	port_ = port;
	socket_num_ = socket_num;
}

Client::~Client()
{
	socket_num_ = 0;
	sockets_.clear();
}

int Client::createClient()
{
	int fail_count = 0;
	for(int i=0; i<socket_num_; ++i)
	{
		Socket* socket = new Socket();
		int ret = socket->create(type_);
		if(ret == 0)
		{
			socket->setNonBlock();
			int client_fd = socket->getHandle();
			sockets_[client_fd] = socket;
		}
		else
		{
			delete socket;
			fail_count++;
		}
	}

	if(fail_count > 0)
	{
		WARNING_LOG("create socket: fail_count = %d", fail_count);
	}

	if(fail_count > socket_num_*0.3)
	{
		return -1;
	}

	return 0;
}

int Client::connectServer()
{
	map<int , Socket*>::iterator it;
	int fail_count = 0;
	for(it = sockets_.begin(); it != sockets_.end(); ++it)
	{
		int ret = it->second->connect(ip_.c_str(), port_);
		if(ret != 0)
		{
			fail_count++;
		}
	}

	if(fail_count > 0)
	{
		WARNING_LOG("connect server: fail_count = %d", fail_count);
	}

	if(fail_count > socket_num_*0.3)
	{
		return -1;
	}

	return 0;
}


int Client::createEpoll()
{
	epfd_ = epoll_create(MAXSOCKETNUM);
	if(epfd_ == -1)
	{
		ERROR_LOG("fail to create epoll fd, <errno: %d>", errno);
		return -1;
	}
	
	map<int , Socket*>::iterator it;
	for(it = sockets_.begin(); it != sockets_.end(); ++it)
	{
		struct epoll_event ev;
		int client_fd = it->first;
		ev.data.fd = client_fd;
		ev.events = EPOLLOUT | EPOLLRDHUP;
		int ret = epoll_ctl(epfd_, EPOLL_CTL_ADD, client_fd, &ev);
		if(ret != 0)
		{
			return -1;
		}
	}

	return 0;
}

int Client::ReConnect(int fd, Socket* socket)
{
	map<int , Socket*>::iterator it = sockets_.find(fd);
	if(it != sockets_.end())
	{
		sockets_.erase(it);
	}

	socket = new Socket();
	int ret = socket->create(type_);

	if(ret != 0)
	{
		delete socket;
		return -1;
	}

	ret = socket->connect(ip_.c_str(), port_);
	if(ret != 0)
	{
		delete socket;
		return -1;
	}

	socket->setNonBlock();
	sockets_[socket->getHandle()] = socket;
	return 0;
}

int Client::writeToServer(const int stress_time, int* succ_count, int* fail_count)
{
	int ret = -1;
	struct epoll_event events[MAXSOCKETNUM];

	const char* send_msg = "hello,world";
	size_t send_len = strlen(send_msg);
	
	long begin_time = time(NULL);
	for( ; ; )
	{
		long end_time = time(NULL);
		if((end_time-begin_time) > stress_time)
		{
			return 0;
		}
		int nfds = epoll_wait(epfd_, events, MAXSOCKETNUM , -1);
		for(int i=0; i<nfds; ++i)
		{
			if(events[i].events & EPOLLOUT)
			{
				int client_fd = events[i].data.fd;
				Socket* socket = sockets_[client_fd];
				ret = socket->write(send_msg, send_len);
				if(ret == 0)
				{
					(*succ_count)++;
				}				
				else if(ret < 0)
				{				
					(*fail_count)++;
				}
			}
			else if(events[i].events & (EPOLLRDHUP | EPOLLERR | EPOLLHUP))
			{
				int client_fd = events[i].data.fd;
				Socket* socket = new Socket();
				ret = ReConnect(client_fd, socket);
				if(ret == 0)
				{
					struct epoll_event ev;
					client_fd = socket->getHandle();
					ev.data.fd = client_fd;
					ev.events = EPOLLOUT | EPOLLRDHUP;
					epoll_ctl(epfd_, EPOLL_CTL_ADD, client_fd, &ev);
				}
			}
		}		
	}
	return 0;
}

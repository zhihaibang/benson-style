#include <stdio.h>
#include <sys/file.h>
#include <unistd.h>
#include <errno.h>
#include <sstream>
#include "SocketPool.h"

SocketPool::SocketPool()
{
	is_init = false;
}


SocketPool::~SocketPool()
{
	for(map<Socket*, int>::iterator it = socket_pool_.begin(); it != socket_pool_.end(); ++it)
	{
		Socket* sock = it->first;
		delete sock;//析构函数中已close
	}
	socket_pool_.clear();
	is_init = false;
}

int SocketPool::Initialize(int pool_size, const string &server_path)
{
	if(is_init)
	{
		return 0;
	}
	
	if(pool_size > MAX_POOL_SIZE)
	{
		stringstream ss;
		ss << "pool_size must less than "<<MAX_POOL_SIZE;
		err_msg_ = ss.str();
		return -1;
	}

	server_path_ = server_path;

	for(int i=0; i<pool_size; ++i)
	{
		CreateSocket();
	}
	
	is_init = true;
	return 0;
}

int SocketPool::CreateSocket()
{
	Socket* my_sock = new Socket();
	if(-1 == my_sock->create(UNIX_SOCK))
	{
		delete my_sock;
		stringstream ss;
		ss << "fail to create socket, errno="<<errno;
		err_msg_ = ss.str();
		return -1;
	}

	int nSendBuf = 16 * 1024 * 1024; //16M
	setsockopt(my_sock->getHandle(), SOL_SOCKET, SO_SNDBUF, (const char*)&nSendBuf, sizeof(int));
	my_sock->setTimeOut(0, kReadTimeOut);//设置读取超时时间

	if(-1 == Connect(my_sock))
	{
		socket_pool_[my_sock] = kSocketNotConnected;
	}
	else
	{
		socket_pool_[my_sock] = kSocketFree;
	}
	
#ifdef I_AM_DEBUG
	printf("success to create socket, fd=%d, socket_ptr=%p\n", my_sock->getHandle(), my_sock);
#endif

	return 0;
}

int SocketPool::Connect(Socket* socket)
{
	struct sockaddr_un address;
	memset(&address, 0, sizeof(struct sockaddr_un));
	address.sun_family = PF_UNIX;
	strncpy(address.sun_path, server_path_.c_str(), server_path_.size() + 1);

	int ret = connect(socket->getHandle(), (struct sockaddr *)&address, sizeof(address));

	if(-1 == ret)
	{
		stringstream ss;
		ss << "fail to connect cc_agent, errno="<<errno
			<< ", server_path="<<server_path_
			<<", fd="<<socket->getHandle();
		err_msg_ = ss.str();
    	return -1;
	}

	return 0;
}


int SocketPool::Reconnect(Socket* socket)
{
	if(-1 == Connect(socket))
	{
		socket_pool_[socket] = kSocketNotConnected;
		return -1;
	}

	socket_pool_[socket] = kSocketBusy;
	return 0;
}

void SocketPool::FreeSocket(Socket* socket)
{
	socket_pool_[socket] = kSocketFree;
}


Socket* SocketPool::GetInstance()
{
	for(map<Socket*, int>::iterator it = socket_pool_.begin(); it != socket_pool_.end(); ++it)
	{
		if(it->second == kSocketFree)
		{
			socket_pool_[it->first] = kSocketBusy;
			return it->first;
		}
	}

	//如果连接池中没有可用的socket，查找是否有未连接的socket,重新连接返回
	map<Socket*, int>::iterator it = socket_pool_.begin();
	while(it != socket_pool_.end())
	{
		if(it->second == kSocketNotConnected)
		{
			if(0 == Reconnect(it->first))
			{
				return it->first;
			}
			else//重连失败,删除并新建一个socket
			{
				socket_pool_.erase(it++);
				CreateSocket();
				break;
			}
		}
		else
		{
			it++;
		}
	}

	stringstream ss;
	ss << "fail to get available socket from socket pool, pool_size="<<socket_pool_.size();
	err_msg_ = ss.str();
	return NULL;
}


bool operator <(const Socket &s1, const Socket &s2)
{
	return s1.getHandle()<s2.getHandle()?true:false;
}


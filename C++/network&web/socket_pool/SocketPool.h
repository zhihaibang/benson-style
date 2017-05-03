#ifndef __SOCKET_POOL_H__
#define __SOCKET_POOL_H__

#include <stdio.h>
#include <string>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <errno.h>
#include <map>
#include "Socket.h"

#define MAX_POOL_SIZE 100

using namespace std;

enum SocketStatus
{
	kSocketNotConnected = 0x0,		//没有连接成功
	kSocketFree = 0x1, 				//空闲状态
	kSocketBusy = 0x2, 				//忙状态，已被使用
};

class SocketPool
{
public:
    SocketPool();
    ~SocketPool();

    int Initialize(int pool_size, const string &server_path);
    Socket* GetInstance();
    int Reconnect(Socket* socket);
    void FreeSocket(Socket* socket);
	
public:
	const string& err_msg() const {return err_msg_;}

private:
	int CreateSocket();
	int Connect(Socket* socket);

private:
	#define kReadTimeOut 65000        //读操作超时时间 65ms

	map<Socket*, int> socket_pool_;//key根据socket的fd排序，value的值有三种状态
	string server_path_;
	string err_msg_;
	bool is_init;
};

#endif

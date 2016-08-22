#ifndef __CLIENT_H
#define __CLIENT_H

#include <map>
#include <string>
#include "Socket.h"
using namespace std;

#define MAXSOCKETNUM 1024

class Client
{
public:
	Client(SOCKET_TYPE type, string ip, int port, int socket_num);
	int createClient();
	int connectServer();
	int createEpoll();
	int writeToServer(const int stress_time, int* succ_count, int* fail_count);
	virtual ~Client();

private:
	int ReConnect(int fd, Socket* socket);
	
private:
	SOCKET_TYPE type_;
	int socket_num_;
	map<int , Socket*> sockets_;
	int epfd_;

	string ip_;
	int port_;
};

#endif
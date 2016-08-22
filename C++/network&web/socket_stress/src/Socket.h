#ifndef __SOCKET_H
#define __SOCKET_H

#define INVALID_SOCK_FD -1

typedef enum
{
	TCP_SOCK = 1,
	UDP_SOCK = 2,
	UNIX_STREAM_SOCK = 3,
	UNIX_DGRAM_SOCK = 4
}SOCKET_TYPE;


class Socket
{
public:
	Socket();
	int create(SOCKET_TYPE type);

	int getHandle()const;

	int connect(const char *pszIP, unsigned uiPort, int timeout_sec = 0, int timeout_usec = 30*1000)const;
	int connect(const char *pszUinxPath)const;//函数名后加const代表该函数不能修改成员变量

	int write(const char * send_buf, size_t send_len, int flag = 0);

	int setNonBlock()const;
	int setBlock()const;

	int close();
	virtual ~Socket();
protected:
	int setHandle(int fd);
private:
	int _handle;

	//缓冲区
	#define MAXSENDBUFSIZE 32*1024*1024
	char buffer[MAXSENDBUFSIZE];
	int bufpos;
};

//inline函数必须要定义在头文件中
inline int Socket::getHandle()const
{
	return _handle;
}

#endif
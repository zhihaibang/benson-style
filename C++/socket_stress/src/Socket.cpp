#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
#include <fcntl.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Socket.h"
#include "ILog.h"
#include "log_interface.h"

using namespace std;

Socket::Socket() : _handle(INVALID_SOCK_FD)
{	
}

Socket::~Socket()
{
	if(_handle != INVALID_SOCK_FD)
	{
		close();
	}
} 	

int Socket::create(SOCKET_TYPE type)
{
	switch(type)
	{
		case TCP_SOCK :
			_handle = ::socket(AF_INET, SOCK_STREAM , 0);
			break;
		case UDP_SOCK : 
			_handle = ::socket(AF_INET, SOCK_DGRAM , 0);
			break;
		case UNIX_STREAM_SOCK:
			_handle = ::socket(PF_UNIX, SOCK_STREAM, 0);
			break;
		case UNIX_DGRAM_SOCK:
			_handle = ::socket(PF_UNIX, SOCK_DGRAM, 0);
			break;
		default : 
			break;
	}
	if(_handle != INVALID_SOCK_FD)
	{
		return 0;
	}
	return -1;
}


int Socket::connect(const char *pszIP, unsigned uiPort, int timeout_sec, int timeout_usec)const
{
    if(NULL == pszIP)
    {
		return -1;
    }
    struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(uiPort);
	addr.sin_addr.s_addr = inet_addr(pszIP);

    int ret = ::connect(_handle, (struct sockaddr *)&addr, sizeof(sockaddr_in));

    if(ret == 0)
    {
        return 0;
    }

    if(errno != EINPROGRESS)
    {
        goto __fail;
    }

    fd_set rset, wset;
    struct timeval tval;
    FD_ZERO(&rset);
    FD_SET(_handle, &rset);
    wset = rset;
    tval.tv_sec = timeout_sec;
    tval.tv_usec = timeout_usec;
    int ready_n;

    if ((ready_n = select(_handle + 1, &rset, &wset, NULL, &tval)) == 0)
    {
        goto __fail;
    }

    if (FD_ISSET(_handle, &rset) || FD_ISSET(_handle, &wset))
    {
        int error;
        socklen_t len = sizeof (error);
        if (getsockopt(_handle, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
        {
            goto __fail;
        }

        if(error != 0)
        {
            goto __fail;
        }

        return 0;
    }
    goto __fail;

__fail:
    ::close(_handle);
    return -1;
}

int Socket::connect(const char *pszUinxPath)const
{
	return -1;
}


int Socket::setNonBlock()const
{
    int iFlags;
    iFlags = fcntl(_handle, F_GETFL, 0);
    iFlags |= O_NONBLOCK;
    iFlags |= O_NDELAY;
    int ret = fcntl(_handle, F_SETFL, iFlags);
    return ret;
}

int Socket::setBlock()const 
{
	if (fcntl(_handle, F_SETFL, ~O_NONBLOCK & fcntl(_handle, F_GETFL)) != 0) 
	{
		return -1;
	}
	return 0;
}


int Socket::write(const char * send_buf, size_t send_len, int flag)
{
	if(flag)//阻塞方式发送
	{
		size_t ret = ::write(_handle, send_buf, send_len);
		if(ret == send_len)
		{
			return 0;
		}
		WARNING_LOG("send message fail, errno=%d", errno);
		return -1;
	}

	if(send_len > MAXSENDBUFSIZE)
	{
		WARNING_LOG("send buffer length great than total buffer size, drop it");
		return -2;
	}

	size_t left_size = MAXSENDBUFSIZE - bufpos;

	if(left_size >= send_len)
	{
		memcpy(buffer+bufpos, send_buf, send_len);
		bufpos += send_len;
	}
	else
	{
		memcpy(buffer, send_buf, send_len);
		bufpos = send_len;
		WARNING_LOG("send buffer length great than left buffer size");
	}

	int nwritten = 0, sentlen = 0;

	while(bufpos > 0)
	{
		nwritten = ::write(_handle, buffer + sentlen, bufpos - sentlen);
		if (nwritten <= 0) break;
		sentlen += nwritten;

		if(sentlen == bufpos)
		{
			bufpos = 0;
		}
	}

	if (nwritten == -1)
	{
        if (errno == EAGAIN) {
            nwritten = 0;
            return 1;
        } else {
        	ERROR_LOG("send message fail, errno=%d ", errno);
            return -1;
        }
    }

    return 0;
}

int Socket::close()
{
	if(_handle != INVALID_SOCK_FD)
	{
		::close(_handle);
		_handle = INVALID_SOCK_FD;
	}
	return 0;
}


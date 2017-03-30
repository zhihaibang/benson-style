/*
 * =====================================================================================
 *
 * @Created time: 2017-03-30
 * @Author: zhihaibang
 * @Description: 一个简单的tcp-server，epoll范例,未编译
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>


using namespace std;

#define MAXLINE 1024
#define LISTENQ 20


int HandleProcess(const char* recv_buf, string &result);



int main(int argc, const char *argv[])
{
	if(argc != 3)
	{
		printf("illegal input params\n");
		return -1;
	}
	
	int port = atoi(argv[2]);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(server_fd <= 0)
    {
        printf("fail to create server fd\n");
        return -1;
    }

    struct epoll_event ev,events[20];
    int epfd = epoll_create(256);
	if (-1 == epfd)
	{
		printf("fail to create epoll fd\n");
		return -1;
	}

    ev.data.fd = server_fd;
    ev.events = EPOLLIN;

    //注册epoll事件
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &ev);
    if(0 != ret)
    {
    	printf("fail to EPOLL_CTL_ADD fd, errno = %d\n", errno);
		return -1;
    }

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;

    //绑定ip和port
    char local_addr[20];
	strcpy(local_addr,argv[1]);

    inet_aton(local_addr,&(server_addr.sin_addr));
    server_addr.sin_port=htons(port);
    
    ret = bind(server_fd,(sockaddr *)&server_addr, sizeof(server_addr));
    if(0 != ret)
    {
    	printf("fail to bind server_fd, errno = %d", errno);
		return -1;
    }

    ret = listen(server_fd, LISTENQ);
    if(0 != ret)
    {
    	printf("fail to listen server_fd, errno = %d", errno);
		return -1;
    }

    printf("server started, begin listening(ip=%s,port=%d)...",argv[1],port);
	
	int connect_fd;
    char recv_buf[MAXLINE];
    socklen_t clilen; 
    struct sockaddr_in client_addr;

    for ( ; ; ) 
    {
        int nfds = epoll_wait(epfd, events, 20, 5000);
        for(int i=0; i<nfds; ++i)
        {
            if(events[i].data.fd == server_fd)
            {
                connect_fd = accept(server_fd,(sockaddr *)&client_addr, &clilen);//读写都用这个connect_fd
                if(connect_fd <= 0)
                {
                    printf("fail to accept connect_fd, errno = %d", errno);
                    continue;
                }
              
                ev.data.fd = connect_fd;
                ev.events = EPOLLIN|EPOLLET;
                ret = epoll_ctl(epfd, EPOLL_CTL_ADD, connect_fd, &ev);

                if(0 != ret)
			    {
			    	printf("fail to EPOLL_CTL_ADD connect_fd, errno = %d", errno);
			    }
            }
            else if(events[i].events & EPOLLIN)//如果是已经连接的用户，并且收到数据，那么进行读入
            {
                connect_fd = events[i].data.fd;
                ret = read(connect_fd, recv_buf, MAXLINE);

                if(ret <= 0)
                {
                    printf("fail to read from client, errno = %d", errno);
					close(connect_fd);
                    continue;
                }

                ev.data.fd = connect_fd;
                ev.events = EPOLLOUT;
                ret = epoll_ctl(epfd, EPOLL_CTL_MOD, connect_fd, &ev);
                if(0 != ret)
			    {
			    	printf("fail to EPOLL_CTL_MOD connect_fd to EPOLLOUT, errno = %d", errno);
			    }
            }
            else if(events[i].events & EPOLLOUT) // 如果有数据发送
            {
                connect_fd = events[i].data.fd;

                string result;
                ret = HandleProcess(recv_buf, result);

                if(ret != 0)
                {
                    printf("fail to HandleProcess, result: %s", result.c_str());
                }

                ret = write(connect_fd, result.c_str(), result.size());

                if((size_t)ret != result.size())
                {
                	printf("fail to send to client, errno = %d", errno);
                }
				else
				{
					//printf("success to send to client, result: %s", result.c_str());
				}
				
				close(connect_fd);
            }
        }
    }

    close(server_fd);
    close(epfd);
	
	INFO_LOG("server stopped.");
	return 0;
}

int HandleProcess(const char* recv_buf, string &result)
{
    return 0;
}



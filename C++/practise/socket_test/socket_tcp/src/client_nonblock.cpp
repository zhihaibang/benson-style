#include <iostream>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/epoll.h>

using namespace std;

#define MAXLINE 80
#define SERV_PORT 8000


int SetNonBlock(int iSock)
{
    int iFlags;

    iFlags = fcntl(iSock, F_GETFL, 0);
    iFlags |= O_NONBLOCK;
    iFlags |= O_NDELAY;
    int ret = fcntl(iSock, F_SETFL, iFlags);
    return ret;
}

int main(int argc, char *argv[])
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    SetNonBlock(sockfd);

    sockaddr_in servaddr = {0};
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    servaddr.sin_port = htons(SERV_PORT);

    int ret = connect(sockfd, (sockaddr *)&servaddr, sizeof(servaddr));//非阻塞connect会立即返回，根据errno判断是否连接

    if(ret == 0){
        cout<<"connect success quickly"<<endl;
    }else if(errno != EINPROGRESS){
        close(sockfd);
	    cout<<"connect fail"<<endl;
	    return -1;
    }
    //用select判断是否连接成功
    fd_set rset, wset;//读写集合
    struct timeval tval;
    FD_ZERO(&rset);
    FD_SET(sockfd, &rset);
    wset = rset;
    tval.tv_sec = 0;
    tval.tv_usec = 30 * 1000; //30毫秒
    int ready_n;
    if ((ready_n = select(sockfd + 1, &rset, &wset, NULL, &tval)) == 0)
    {
        close(sockfd);
	    cout<<"connect fail"<<endl;
	    return -1; /* 超时，连接失败 */
    }

    if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset))
    {
        int error;
        socklen_t len = sizeof (error);
        if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) 
        {
            close(sockfd);
		    cout<<"connect fail"<<endl;
		    return -1;
        }
        if(error != 0)
        {
            close(sockfd);
		    cout<<"connect fail"<<endl;
		    return -1;
        }
    }
    cout<<"connect success"<<endl;  

    char message[20] = "ASDFERFER";

    struct epoll_event ev,events[20];
    int epfd = epoll_create(256);
    ev.data.fd = sockfd;
    ev.events = EPOLLOUT | EPOLLRDHUP;

    ret = epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&ev);
    if(ret != 0)
    {
    	cout<<"epoll out add fail, errno="<<errno<<endl;
    }

    for(; ;)
    {
    	int nfds = epoll_wait(epfd,events,20,-1);//没有事件会一直阻塞
    	if (nfds == -1) 
    	{
    		cout<<"epoll_pwait"<<endl;
            break;
        }

    	for(int i=0; i<nfds; ++i)
    	{
    		cout<<"event:"<<events[i].events<<endl;
    		if(events[i].events & EPOLLOUT)
    		{
    			int write_pos = 0;
    			int nLeft = sizeof(message);
    			while(nLeft > 0)
			    {
			        int nWrite = write(sockfd, message + write_pos, nLeft);
			        if(nWrite == 0)
			        {
			            cout<<"send fail,nWrite=0"<<endl;
			            break;
			        }
			        if( nWrite < 0)
			        {
			            if(errno == EWOULDBLOCK || errno == EAGAIN)
			            {
			                cout<<"EWOULDBLOCK EAGAIN"<<endl;
			            	break;
			            }
			            else if(errno == EINTR)
			            {
			            	cout<<"EINTR"<<endl;
			                continue;
			            }
			            else
			            {
			            	cout<<"send fail,nWrite<0"<<endl;
			                break;
			            }
			        }
			        else
			        {
			            nLeft -= nWrite;
			            write_pos += nWrite;
			        }
			    }
			    cout<<"send success,fd="<<sockfd<<endl;
    		}
    		//else if(events[i].events & (EPOLLRDHUP | EPOLLERR | EPOLLHUP))
            else if((events[i].events & EPOLLRDHUP) | (events[i].events & EPOLLERR) | (events[i].events & EPOLLHUP))
    		{
    			cout<<"EPOLLRDHUP,EPOLLERR,EPOLLHUP, fd = "<< events[i].data.fd<<endl;
                close(events[i].data.fd);
    		}		
    	}
    }

    close(sockfd);
    return 0;
}
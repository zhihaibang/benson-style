#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

using namespace std;

#define MAXLINE 80
#define LISTENQ 20
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

int WriteNonBlock(int fd_, const char* send_buf, size_t send_len)
{
    int write_pos = 0;//发送位置
    int nLeft = send_len;//表示未发完的数据
    while(nLeft > 0)
    {
        int nWrite = write(fd_, send_buf + write_pos, nLeft);//已发完的数据长度
        if(nWrite == 0)//socket已关闭，发送失败
        {
            return -1;
        }
        if( nWrite < 0)
        {
            if(errno == EWOULDBLOCK || errno == EAGAIN)//没有空间可写数据
            {
                nWrite = 0;
            }else if(errno == EINTR){//连接正常，操作被中断，可继续发送
                continue;
            }
            else
            {
                return -1;
            }
        }
        else//发送成功
        {
            nLeft -= nWrite;
            write_pos += nWrite;
        }
    }
    return 0;
}

int ReadNonBlock(int fd_, char* recv_buf, size_t recv_len)
{
    while(1)
    {
        int ret = read(fd_, recv_buf, recv_len);
        if(ret > 0){//接收成功
            return 0;
        }
        if(ret == 0){//socket已关闭，发送失败
            return -1;
        }
        if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR){
            continue;
        }
    }
    return -1;
}

int main(int argc, char* argv[])
{
    int i, listenfd, connfd, epfd, nfds, portnumber=SERV_PORT;
    ssize_t n=-1;
    char line[MAXLINE];
    socklen_t clilen;
   
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd <= 0){
        cout<<"create fd fail"<<endl;
        return -1;
    }
    SetNonBlock(listenfd);

    //声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件
    struct epoll_event ev,events[20];
    //生成用于处理accept的epoll专用的文件描述符
    epfd=epoll_create(256);
    //设置与要处理的事件相关的文件描述符
    ev.data.fd=listenfd;
    //设置要处理的事件类型
    ev.events=EPOLLIN|EPOLLET;
    //ev.events=EPOLLIN;

    //注册epoll事件
    epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);

    //listenfd绑定ip地址
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    char local_addr[20]="127.0.0.1";
    inet_aton(local_addr,&(serveraddr.sin_addr));
    serveraddr.sin_port=htons(portnumber);
    
    //bind和listen不是阻塞函数
    bind(listenfd,(sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(listenfd, LISTENQ);

    cout << "server listening ..."  << endl;

    int ret = -1;

    for ( ; ; ) 
    {
        //等待epoll事件的发生
        nfds=epoll_wait(epfd,events,20,-1);

        //处理所发生的所有事件
        for(i=0;i<nfds;++i)
        {
            if(events[i].data.fd == listenfd)//如果新监测到一个SOCKET用户连接到了绑定的SOCKET端口，建立新的连接
            {
                connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen);//以后读写都用这个返回的fd
                if(connfd < 0){
                    perror("connfd<0,accept fail");
                    exit(1);
                }
                SetNonBlock(connfd);//设置为非阻塞模式
                char *str = inet_ntoa(clientaddr.sin_addr);
                cout << "accapt a connection from " << str << endl;
                
                ev.data.fd=connfd;
                ev.events=EPOLLIN|EPOLLET;
                epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
            }
            else if(events[i].events & EPOLLIN)//如果是已经连接的用户，并且收到数据，那么进行读入
            {
                connfd = events[i].data.fd;

                ret = ReadNonBlock(connfd, line, MAXLINE);
                if(ret != 0){
                    cout<<"read line fail"<<endl;
                    close(connfd);
                    break;
                }
                cout << "receive:" << line << endl;


                 ev.data.fd = connfd;
                 ev.events = EPOLLOUT|EPOLLET;
                 epoll_ctl(epfd,EPOLL_CTL_MOD, connfd, &ev);//修改标识符，等待下一个循环时发送数据，异步处理的精髓
            }
            else if(events[i].events & EPOLLOUT) // 如果有数据发送
            {
                connfd = events[i].data.fd;

                //将从client接收到的字母转化为大写，回送给client
                for (int i = 0; i < n; i++)
               		line[i] = toupper(line[i]);

                ret = WriteNonBlock(connfd, line, MAXLINE);
                if(ret != 0){
                    cout<<"send line fail"<<endl;
                    close(connfd);
                    break;
                }
                cout << "send:" << line << endl;

                ev.data.fd = connfd;
                ev.events = EPOLLIN|EPOLLET;
                epoll_ctl(epfd,EPOLL_CTL_MOD, connfd, &ev);
            }
        }
    }
    close(listenfd);
    return 0;
}

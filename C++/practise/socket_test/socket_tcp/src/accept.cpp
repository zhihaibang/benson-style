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

int SetNonBlock(int iSock)
{
    int iFlags;

    iFlags = fcntl(iSock, F_GETFL, 0);
    iFlags |= O_NONBLOCK;
    iFlags |= O_NDELAY;
    int ret = fcntl(iSock, F_SETFL, iFlags);
    return ret;
}

int main(int argc, char* argv[])
{
    int listenfd, connfd;
   
    struct sockaddr_in serveraddr;
    struct sockaddr_in clientaddr;
    socklen_t clilen;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

   SetNonBlock(listenfd);

    //listenfd绑定ip地址
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    char local_addr[20]="127.0.0.1";
    inet_aton(local_addr,&(serveraddr.sin_addr));
    serveraddr.sin_port=htons(8000);
    
    //bind和listen不是阻塞函数
    bind(listenfd,(sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(listenfd, 20);

    cout << "server listening ..."  << endl;

    int ret = -1;

    while(1)
    {
        connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen);//以后读写都用这个返回的fd
        cout<<"connfd = "<<connfd<<", errno = "<<errno<<endl;
        sleep(1);
    }
    return 0;
}

#include <iostream>
#include <arpa/inet.h>//inet_ntop函数使用
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;
 
#define MAXLINE 80
#define SERV_PORT 8000
 
int main()
{
 	//设置一个socket地址结构server_addr,代表服务器internet地址, 端口
    struct sockaddr_in servaddr, cliaddr;
    socklen_t cliaddr_len;
    int listenfd, connfd;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];

    //创建用于internet的流协议(TCP)socket,用server_socket代表服务器socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));//把一段内存区的内容全部设置为0
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    listen(listenfd, 20);

    cout<<"Accepting connections ..."<<endl;
    cliaddr_len = sizeof(cliaddr);
    
    while (1) 
    {
       connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
       int n = read(connfd, buf, MAXLINE);
       if(n>0){
          cout<<"received from "<<inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str))
          <<" at PORT "<<cliaddr.sin_port<<":"<<buf<<endl;
       }
         
       for (int i = 0; i < n; i++)//将从client接收到的字母转化为大写，回送给client
               buf[i] = toupper(buf[i]);
       n = write(connfd, buf, sizeof(buf));
    }
    close(connfd);
}
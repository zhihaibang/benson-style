#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;
 
#define MAXLINE 80
#define SERV_PORT 8888
 
int main()
{
 	  //设置一个socket地址结构server_addr,代表服务器internet地址, 端口
    struct sockaddr_in servaddr;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];

    //创建用于internet的数据报协议(UDP)socket,用server_socket代表服务器socket
    int serverfd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));//把一段内存区的内容全部设置为0
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    
    int ret = bind(serverfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if(ret < 0){
        cout<<"fail bind"<<endl;
        return 0;
    }
    
    /* 定义一个地址，用于捕获客户端地址 */
    struct sockaddr_in client_addr;
	  socklen_t client_addr_length = sizeof(client_addr);

    while (1) 
    {   	
       int n = recvfrom(serverfd,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,&client_addr_length);
       if(n>0){
          cout<<"received from "<<inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str))
          <<" at PORT "<<client_addr.sin_port<<":"<<buf<<endl;
          for(int i = 0; i < n; i++)//将从client接收到的字母转化为大写，回送给client
               buf[i] = toupper(buf[i]);
          sendto(serverfd,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
       }
    }
    close(serverfd);
    return 0;
}
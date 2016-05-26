#include <iostream>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

#define MAXLINE 80
#define SERV_PORT 8888

int main(int argc, char *argv[])
{
    char buf[MAXLINE];

    /* 服务端地址 */
    sockaddr_in servaddr = {0};
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    servaddr.sin_port = htons(SERV_PORT);

    //定义一个client socket
    int client_fd = socket(AF_INET, SOCK_DGRAM, 0);

    cin>>buf;
    int count = sendto(client_fd,buf,sizeof(buf),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
    if(count>0){
    	cout<<"success send to server"<<endl;
        socklen_t server_add_len = sizeof(servaddr);
    	int n = recvfrom(client_fd,buf,sizeof(buf),0,(struct sockaddr *)&servaddr,&server_add_len);
       	if(n>0){
          cout<<"response from server:"<<buf<<endl;
       	}
    }else{
    	cout<<"fail send to server"<<endl;
    } 
    close(client_fd);
    return 0;
}
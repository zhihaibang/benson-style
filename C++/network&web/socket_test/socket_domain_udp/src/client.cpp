#include <iostream>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <sys/un.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
using namespace std;

#define MAXLINE 60 
  
int main()  
{  
    /* create a socket */  
    int client_fd = socket(AF_UNIX, SOCK_DGRAM, 0);  
    
    /*server address*/
    struct sockaddr_un servaddr;  
    servaddr.sun_family = AF_UNIX;  
    strcpy(servaddr.sun_path, "server_socket");
    
    char buf[MAXLINE];
    cin>>buf;

    int count = sendto(client_fd,buf,sizeof(buf),0,(struct sockaddr *)&servaddr,sizeof(servaddr));  
    if(count>0){
      cout<<"send success:"<<buf<<endl;
    }
       
    /* close the socket */  
    close(client_fd);     
    return 0;  
}  
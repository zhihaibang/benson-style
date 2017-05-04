#include <iostream>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <sys/un.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <errno.h>
using namespace std;

#define MAXLINE 60  

int main()  
{  
  /* delete the socket file */
  unlink("server_socket");
    
  /* create a UNIX socket */  
  int serverfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    
  struct sockaddr_un server_addr;  
  server_addr.sun_family = AF_UNIX;  
  strcpy(server_addr.sun_path, "server_socket");
    
  /* bind with the local file */  
  bind(serverfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    
  char buf[MAXLINE];
  struct sockaddr_un client_addr;
  socklen_t len = sizeof(client_addr);

  while(1)  
  {  
    int n = recvfrom(serverfd,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,&len);
    if(n > 0){
		cout<<"received:"<<buf<<endl;
		for(int i = 0; i < n; i++)//将从client接收到的字母转化为大写，回送给client
               buf[i] = toupper(buf[i]);
		n = sendto(serverfd,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
		if(n <= 0)
		{
			cout<<"fail to send,errno="<<errno<<endl;
		}
		else
		{
			cout<<"success to send,buf="<<buf<<endl;
		}
    }
	else
	{
		cout<<"fail to received,errno="<<errno<<endl;
	}
  }  
  close(serverfd);  
  return 0;  
}  
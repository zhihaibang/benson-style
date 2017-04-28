#include <sys/types.h>  
#include <sys/socket.h>  
#include <sys/un.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <errno.h> 
  
int main()  
{  
  /* delete the socket file */  
  unlink("server_socket");  
    
  /* create a socket */  
  int server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);  
    
  struct sockaddr_un server_addr;  
  server_addr.sun_family = AF_UNIX;  
  strcpy(server_addr.sun_path, "server_socket");  
    
  /* bind with the local file */  
  bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));  
    
  /* listen */  
  listen(server_sockfd, 5);  
    
  char ch;  
  int client_sockfd;  
  struct sockaddr_un client_addr;  
  socklen_t len = sizeof(client_addr);  
  while(1)  
  {  
    printf("server waiting:\n");  
      
    /* accept a connection */  
    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &len);  
      
    /* exchange data */  
    if(read(client_sockfd, &ch, 1) == 1)
    {
      printf("get char from client: %c\n", ch);
      ++ch;  
      if(write(client_sockfd, &ch, 1) == 1)
      {
        printf("send char to client: %c\n", ch);
      }
      else
      {
        printf("fail to send char to client,errno=%d\n", errno);
      }
    }
    else
    {
      printf("fail to get char from client,errno=%d\n", errno);
    }
         
    /* close the socket */  
    close(client_sockfd);  
  }  
    
  return 0;  
}  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <sys/un.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>
#include <errno.h>  
  
int main()  
{  
  /* create a socket */  
  int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);  
    
  struct sockaddr_un address;  
  address.sun_family = AF_UNIX;  
  strcpy(address.sun_path, "server_socket");  
    
  /* connect to the server */  
  int result = connect(sockfd, (struct sockaddr *)&address, sizeof(address));  
  if(result == -1)  
  {  
    perror("connect failed: ");  
    exit(1);  
  }  
    
  /* exchange data */  
  char ch = 'A';  
  if(write(sockfd, &ch, 1) == 1)
  {
    printf("send char to server: %c\n", ch);
    if(read(sockfd, &ch, 1) == 1)
    {
      printf("get char from server: %c\n", ch);  
    }
    else
    {
      printf("fail to get char from server,errno=%d\n", errno);
    }
  }
  else
  {
    printf("fail to send char to server,errno=%d\n", errno);
  }
  
  
  
  /* close the socket */  
  close(sockfd);  
    
  return 0;  
}  
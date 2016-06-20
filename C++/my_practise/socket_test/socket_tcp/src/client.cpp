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
#define SERV_PORT 8000

int main(int argc, char *argv[])
{
    char buf[MAXLINE];

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in servaddr = {0};
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    servaddr.sin_port = htons(SERV_PORT);

    if (0 != connect(sockfd, (sockaddr *)&servaddr, sizeof(servaddr)))
    {
        cout<<"connected failed"<<endl;
        return 1;
    }

    char message[20];
    cin>>message;
    
	int count = write(sockfd, message, sizeof(message));
	if(count > 0){
	  	cout<<"send to server:"<<message<<endl;
	}else{
	  	cout<<"fail send to server"<<endl;
	  	return 1;
	}
	  
	count = read(sockfd, buf, sizeof(message));
	if(count > 0){
	  	cout<<"response from server: "<<buf<<endl;
	}
    close(sockfd);
    return 0;
}

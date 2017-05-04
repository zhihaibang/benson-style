#include <iostream>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <sys/un.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <errno.h>
#include <sstream>
#include <sys/syscall.h>

using namespace std;

#define MAXLINE 60 
  
int main()  
{  
    /* create a socket */  
    int client_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	
	/*bind client socket*/
	struct sockaddr_un addr_bind;
	memset (&addr_bind, 0, sizeof(addr_bind));
	addr_bind.sun_family = AF_UNIX;
	
	pthread_t pid = syscall(SYS_gettid);//获取线程pid
	stringstream ss;
	ss << "tmp_"<<pid;
	string  temp_file = ss.str();
	unlink(temp_file.c_str());
	
	strcpy(addr_bind.sun_path, temp_file.c_str());
	int ret = bind(client_fd, (struct sockaddr *)&addr_bind, sizeof(addr_bind));
	if(ret != 0)
	{
		cout<<"bind fail,errno:"<<errno<<endl;
		return -1;
	}
    
	/*server address*/
    struct sockaddr_un servaddr;  
    servaddr.sun_family = AF_UNIX;  
    strcpy(servaddr.sun_path, "server_socket");
    
    char buf[MAXLINE];
    cin>>buf;

    int count = sendto(client_fd,buf,sizeof(buf),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
    if(count>0)
	{
		cout<<"send success:"<<buf<<endl;
		
		socklen_t server_add_len = sizeof(servaddr);
		int n = recvfrom(client_fd,buf,sizeof(buf),0,(struct sockaddr *)&servaddr,&server_add_len);
		if(n > 0){
			cout<<"response from server:"<<buf<<endl;
		}else
		{
			cout<<"fail to response from server,errno="<<errno<<endl;
		}
    }
	else
	{
		cout<<"fail to sendto server,errno="<<errno<<endl;
	}
	    
    /* close the socket */
    close(client_fd);
	unlink(temp_file.c_str());
    return 0;  
}  
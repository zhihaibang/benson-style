#include <stdio.h>
#include <sys/file.h>
#include <unistd.h>
#include <errno.h>

int main(void)  
{  
    FILE *fp = NULL;   
      
    if ((fp = fopen("./file_lock", "wb+")) == NULL) //打开文件  
    {
		printf("file open error,errno=%d!\n",errno);
		return -1;
	}    	
    if (flock(fp->_fileno, LOCK_EX) != 0) //给该文件加互斥锁
        printf("file lock by others\n");//加锁失败，阻塞
    while(1)
    {     
        printf("process1-ex\n");
        sleep(1);  
    }    
    flock(fp->_fileno, LOCK_UN); //文件解锁 
    fclose(fp); //关闭文件       
    return 0;  
}  
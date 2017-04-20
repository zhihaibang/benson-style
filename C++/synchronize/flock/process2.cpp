#include <stdio.h>
#include <sys/file.h>
#include <unistd.h>
#include <errno.h>

int main(void)  
{  
    FILE *fp = NULL;  
    int i = 0;  
      
    if ((fp = fopen("./file_lock", "wb+")) == NULL) //打开文件
    {
		printf("file open error,errno=%d!\n",errno);
		return -1;
	}   
    if(flock(fp->_fileno, LOCK_SH) != 0)//文件加共享锁
    {
    	printf("file lock by others\n");//加锁失败，阻塞
    } 
    while(1) //进入循环  
    {     
        printf("process2-sh\n");
        sleep(1);  
    }     
    flock(fp->_fileno, LOCK_UN); //释放文件锁  
	fclose(fp); //关闭文件
    return 0;  
}  
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
    flock(fp->_fileno, LOCK_EX); //文件加锁  
    while(1) //进入循环  
    {     
        printf("%d\n", i++);  
        sleep(1);  
    }     
    flock(fp->_fileno, LOCK_UN); //释放文件锁  
	fclose(fp); //关闭文件
    return 0;  
}  
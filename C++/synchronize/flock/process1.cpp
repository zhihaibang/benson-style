#include <stdio.h>
#include <sys/file.h>
#include <unistd.h>
#include <errno.h>

int main(void)  
{  
    FILE *fp = NULL;  
    int i = 10;   
      
    if ((fp = fopen("./file_lock", "wb+")) == NULL) //打开文件  
    {
		printf("file open error,errno=%d!\n",errno);
		return -1;
	}    	
    if (flock(fp->_fileno, LOCK_EX) != 0) //给该文件加锁  
        printf("file lock by others\n");  
    while(i) //进入循环,加锁时间为10秒,打印倒计时  
    {     
        printf("%d\n", i--);  
        sleep(1);  
    }    
    flock(fp->_fileno, LOCK_UN); //文件解锁 
    fclose(fp); //10秒后退出,关闭文件       
    return 0;  
}  
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

using namespace std;


int ForkWork(int count);

int main(int argc, char const *argv[])
{
	int count = 10;

	ForkWork(count);

	return 0;
}

/*
假定父进程malloc的指针指向0x12345678, 
fork 后，子进程中的指针也是指向0x12345678，
但是这两个地址都是虚拟内存地址，
经过内存地址转换后所对应的 物理地址是不一样的。
所以两个进程中的这两个地址相互之间没有任何关系。
父子进程的地址空间是相互独立的！

fork后，这两个虚拟地址实际上指向相同的物理地址（内存页);
只有任何一个进程试图修改这个虚拟地址里的内容前，
两个虚拟地址才会指向不同的物理地址，
新的物理地址的内容从原物理地址中复制得到
这就是Linux的copy-on-write技术。

fork的返回值比较特殊，在父进程和子进程中返回值不相同,如果创建子进程成功，那么
在父进程中，返回子进程pid,
在子进程中，返回0
*/
int ForkWork(int count)
{		
	pid_t pid = 0;
		
	for (int i = 0; i < count; ++i)
	{	
		/*此时仅有一个进程*/
		pid = fork();

		/*此时已经有两个进程在同时运行*/
		if (pid==0 || pid==-1)
		{
			break;
		}		
	}
	
	if (pid == -1)
	{
		perror("fail to fork!\n");
		exit(1);
	}
	else if (pid == 0)
	{	
		int a = 10;
		count = 5;
		printf("child-pid:%u, count=%d, count_ptr:%p, a_ptr:%p\n", getpid(), count, &count, &a);
		exit(0);
	}
	else if(pid > 0)
	{
		count = 3;
		printf("father-pid:%u, count=%d, count_ptr:%p\n", getpid(), count, &count);
		exit(0);
	}

	return 0;
}
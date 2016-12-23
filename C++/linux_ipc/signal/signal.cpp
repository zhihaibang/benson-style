#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void signal_handler(int signo)
{
    signal(signo, signal_handler);
    printf("recv signal[%d]\n", signo);
    switch(signo)
    {
        case SIGHUP:
             //终端退出
             printf("Process recieve SIGHUP\n");
             break;       
        case SIGTERM:
            /*程序结束(12)信号, 与SIGKILL不同的是该信号可以被阻塞和 
            处理. 通常用来要求程序自己正常退出. shell命令kill缺省产生这 
            个信号.*/
             printf("do sth....\n");
             printf("Process recieve SIGTERM\n");
             break;
        case SIGKILL:
             printf("Process recieve SIGKILL\n");
             break;
        case SIGINT:
             //按下ctrl+c产生，程序终止
             printf("Process recieve SIGINT\n");
             break;
        case SIGUSR1:
             printf("Process recieve SIGUSR1\n");
             break;
        default:
             printf("%d signal unregister\n", signo);
             break;
    }
    exit(0);
}

//主进程
int main(int argc, char const *argv[])
{
    signal(SIGTERM, signal_handler);
    signal(SIGKILL, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGUSR1, signal_handler);

    pid_t pid = getpid();
    printf("pid=%d\n",pid);
             
    while(1)sleep(1);
    return 0;
}
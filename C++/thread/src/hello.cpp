#include <iostream>
#include <pthread.h> //多线程相关操作头文件

using namespace std;

#define NUM_THREADS 5 //线程数

//函数返回的是函数指针，便于后面作为参数
void* say_hello(void* args)
{
    cout << "hello..." << endl;
    return 0;
}

int main()
{
    pthread_t tids[NUM_THREADS]; //线程id
    for( int i = 0; i < NUM_THREADS; ++i )
    {
        //参数：创建的线程id，线程参数，线程运行函数的起始地址，运行函数的参数
        int ret = pthread_create(&tids[i], NULL, say_hello, NULL);        
        if( ret != 0 ) 
        {
            cout << "pthread_create error:error_code=" << ret << endl;
        }
    }
    //等待各个线程退出后，进程才结束，否则进程强制结束，线程处于未终止的状态
    pthread_exit(NULL); 
}

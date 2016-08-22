#include <iostream>
#include <pthread.h>

using namespace std;

#define NUM_THREADS 5 //线程数


void* say_hello(void* args)
{
	int i = *((int*)args); //对传入的参数进行强制类型转换，由无类型指针转变为整形指针，再用*读取其指向到内容
    cout << "hello " <<i<< endl;
    return 0;
}

int main()
{
    pthread_t tids[NUM_THREADS]; //线程id
    for( int i = 0; i < NUM_THREADS; ++i )
    {
        //参数：创建的线程id，线程参数，线程运行函数的起始地址，运行函数的参数
        //传入到函数的参数必须强转为void*类型，即无类型指针，&i表示取i的地址，即指向i的指针
        int ret = pthread_create(&tids[i], NULL, say_hello, (void*)&i);
	pthread_join(tids[i], NULL); //pthread_join用来等待一个线程的结束，是一个线程阻塞的函数
        if( ret != 0 ) 
        {
            cout << "pthread_create error:error_code=" << ret << endl;
        }
    }
    //等待各个线程退出后，进程才结束，否则进程强制结束，线程处于未终止的状态
    pthread_exit(NULL); 
}

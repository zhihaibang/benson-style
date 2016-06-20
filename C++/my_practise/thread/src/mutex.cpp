#include <iostream>
#include <pthread.h>

using namespace std;

#define NUM_THREADS 3 //线程数

int sum = 0; //定义全局变量，让所有线程同时写，这样就需要锁机制
pthread_mutex_t sum_mutex; //互斥锁 


void* say_hello(void* args)
{
	int i = *((int*)args);   
    pthread_mutex_lock(&sum_mutex ); //先加锁，再修改sum的值，锁被占用就阻塞，直到拿到锁再修改sum;
    cout << "thread "<<i<<":before sum =" << sum <<endl;
    sum += i;
    cout << "thread "<<i<<":after sum = " << sum <<endl;
    pthread_mutex_unlock( &sum_mutex ); //释放锁，供其他线程使用
    return 0;
}

int main()
{
    pthread_t tids[NUM_THREADS]; //线程id
    int index[NUM_THREADS];//用来保存i的值避免被修改
    pthread_mutex_init(&sum_mutex,NULL);
    for( int i = 0; i < NUM_THREADS; i++)
    {
    	index[i] = i;
        int ret = pthread_create(&tids[i], NULL, say_hello, (void*)&(index[i]));
    }
    for( int i = 0; i < NUM_THREADS; i++ )
    {
        pthread_join(tids[i], NULL);     
    }
    cout<<"finally,sum="<<sum<<endl;
    //等待各个线程退出后，进程才结束，否则进程强制结束，线程处于未终止的状态
    pthread_exit(NULL);   
}

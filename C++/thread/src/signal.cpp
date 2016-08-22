#include <iostream>
#include <pthread.h>

using namespace std;  

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;/*初始化互斥锁*/
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;/*初始化条件变量*/ 
/*thread1打印10以内的偶数，thread2打印奇数*/

int i= 0;

void *thread1(void *args)  
{  
    for(i=0;i<=9;i++)//i是全局变量
    {
        pthread_mutex_lock(&mutex);
        if(i%2==0){
            cout << "thread 1,i=" << i<<endl;       
        }else{
            pthread_cond_signal(&cond);/*i变成奇数,条件改变，发送信号，通知其他线程*/
            cout<<"thread 1,i="<<i<<",signal other threads"<<endl;
        }
        pthread_mutex_unlock(&mutex);
    }  
    
    return 0;
}


void *thread2(void *args)
{   
    while(1)
    {
        pthread_mutex_lock(&mutex);
        if(i%2==0){//i是偶数线程就挂起
            /*操作有2步：
            第一解锁，先解除之前的pthread_mutex_lock锁定的mutex;
            第二挂起，阻塞并在等待队列里休眠，即所在线程挂起，直到再次被再次唤醒*/
            pthread_cond_wait(&cond,&mutex);/*wait必须和互斥锁同时用在一个线程里，它同时起到对资源的加锁和解锁*/
            cout<<"pthread_cond_wait"<<endl;       
        }     
        cout << "thread 2,i=" << i<<endl;
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}  


int main()  
{  
    pthread_t t_a;  
    pthread_t t_b;  
    pthread_create(&t_a,NULL,thread1,NULL);/*再创建进程t_a*/
    pthread_create(&t_b,NULL,thread2,NULL); /*先创建进程t_b*/   
    pthread_join(t_a, NULL);/*等待进程t_a结束*/  
    pthread_join(t_b, NULL);/*等待进程t_b结束*/  
    pthread_mutex_destroy(&mutex);  
    pthread_cond_destroy(&cond);  
    return 0;
}  

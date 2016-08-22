#include <iostream>
#include <unistd.h>
#include <pthread.h>
using namespace std;


class Thread{
	private:
		static void* thread(void*);//静态函数

	public:
		int simpleThread();
};

int Thread::simpleThread()
{
    pthread_t id;
    int ret = pthread_create(&id, NULL,thread, NULL);//只能接收静态函数作为函数
    if(ret) {
        cerr << "Create pthread error!" << endl;
        return 1;
    }
   /* for(int i = 0;i < 3;i++) {
        cout <<  "This is the main process." << endl;
        sleep(1);
    }*/
    pthread_join(id, NULL);
    return 0;
}

void* Thread::thread(void* ptr)
{
    for(int i = 0;i < 3;i++) 
    {
        sleep(1);
        cout << "This is a pthread." << endl;
    }
    return 0;
}

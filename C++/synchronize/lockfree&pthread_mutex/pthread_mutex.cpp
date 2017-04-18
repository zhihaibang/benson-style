#include <string>
#include <sstream>
#include <list>
#include <pthread.h>
#include <iostream>
#include <time.h>

using namespace std;

int producer_count = 0;
int consumer_count = 0;

list<string> product;
list<string> consumer_list;
pthread_mutex_t mutex;

const int iterations = 10000;

//是否生产完毕标志
bool done = false;

void* producer(void* args)
{
    for (int i = 0; i != iterations; ++i) {
    	pthread_mutex_lock(&mutex);
        int value = ++producer_count;
        stringstream ss;
        ss<<value;
        product.push_back(ss.str());
		//cout<<"list push:"<<ss.str()<<endl;
		pthread_mutex_unlock(&mutex);
    }
    return 0;
}



//消费函数
void* consumer(void* args)
{
    //当没有生产完毕，则边消费边生产
    while (!done) {
    	pthread_mutex_lock(&mutex);
    	if(!product.empty()){
    		consumer_list.splice(consumer_list.end(), product);
    		pthread_mutex_unlock(&mutex);
    		while(!consumer_list.empty()){
    			string value = consumer_list.front();
				consumer_list.pop_front();
		    	//cout<<"list pop:"<<value<<endl;
		        ++consumer_count;
    		}	    	
    	}else{
    		pthread_mutex_unlock(&mutex);
    	}
    }
	//如果生产完毕，则消费
	while(!consumer_list.empty()){
		string value = consumer_list.front();
		consumer_list.pop_front();
    	//cout<<"list pop:"<<value<<endl;
        ++consumer_count;
	}
	return 0;
}

int main(int argc, char* argv[])
{
    struct timespec time_start={0, 0},time_end={0, 0};
    clock_gettime(CLOCK_REALTIME, &time_start);

    pthread_t producer_tid;
    pthread_t consumer_tid;
    
    pthread_mutex_init (&mutex,NULL);
    pthread_create(&producer_tid, NULL, producer, NULL);
    pthread_create(&consumer_tid, NULL, consumer, NULL);

    //等待生产者生产完毕
    pthread_join(producer_tid, NULL);
    //可以消费标志
    done = true;     //主线程不等生产线程完毕就设置done标记
    cout << "producer done" << endl;    //输出以观察主线程和各子线程的执行顺序
           
    //等待消费者结束
    pthread_join(consumer_tid, NULL);
    clock_gettime(CLOCK_REALTIME, &time_end);

    long cost = (time_end.tv_sec-time_start.tv_sec)/1000000 + (time_end.tv_nsec-time_start.tv_nsec)/1000;
    
    cout<<"===========cost time:"<<cost<<"us==========="<<endl;

    cout << "produced " << producer_count << " objects." << endl;
    cout << "consumed " << consumer_count << " objects." << endl;
}

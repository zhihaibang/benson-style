#include <string>
#include <sstream>
#include <pthread.h>
#include <iostream>
#include <time.h>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/atomic.hpp>

using namespace std;

int producer_count = 0;
boost::atomic_int consumer_count (0);

boost::lockfree::spsc_queue<string, boost::lockfree::capacity<1024> > spsc_queue;

const int iterations = 10000;

void* producer(void* args)
{
    for (int i = 0; i != iterations; ++i) {
        int value = ++producer_count;
        stringstream ss;
        ss<<value;
        while(!spsc_queue.push(ss.str()));
		//cout<<"queue push:"<<ss.str()<<endl;
    }
    return 0;
}

//是否生产完毕标志
boost::atomic<bool> done (false);

//消费函数
void* consumer(void* args)
{
	string value;
    //当没有生产完毕，则边消费边生产
    while (!done) {
    	//只要能弹出元素，就消费
        while (spsc_queue.pop(value)) {
        	//cout<<"queue pop:"<<value<<endl;
            ++consumer_count;
        }
    }
	//如果生产完毕，则消费
    while (spsc_queue.pop(value)){
    	//cout<<"queue pop:"<<value<<endl;
        ++consumer_count;
    }
	return 0;
}

int main(int argc, char* argv[])
{
    using namespace std;
    cout << "boost::lockfree::queue is ";
    if (!spsc_queue.is_lock_free())
        cout << "not ";
    cout << "lockfree" << endl;

    struct timespec time_start={0, 0},time_end={0, 0};
    clock_gettime(CLOCK_REALTIME, &time_start);

    pthread_t producer_tid;
    pthread_t consumer_tid;
    
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
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <functional>
#include <cstdlib>
#include <utility>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <exception>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <sys/time.h>

using namespace boost::interprocess;

#define MAX_NAME_SIZE 32*1024
#define MAX_SHM_SIZE 32*1024*1024

class Item
{
    public:
        Item(){}
        ~Item(){}

        int id;
        int size;
        char name[MAX_NAME_SIZE];//用string类型时,另一个进程无法读取,它不知道string的地址
};

typedef int KeyType;
typedef Item MappedType;
typedef std::pair<const int, Item> ValueType;

typedef allocator<ValueType, managed_shared_memory::segment_manager> ShmemAllocator;

typedef map<KeyType, MappedType, std::less<KeyType>, ShmemAllocator> MyMap;

int main()
{
    try {
        // init
        managed_shared_memory segment(create_only, "SharedMemory", MAX_SHM_SIZE);

        const ShmemAllocator alloc_inst (segment.get_segment_manager());

        MyMap * mymap = segment.construct<MyMap>("MyMap") (std::less<KeyType>(), alloc_inst);
		
		int counter = 10;
		
		struct timeval tv_begin, tv_end;
		gettimeofday(&tv_begin, NULL);
		
        for(int i = 0; i < counter; ++i){	 
			Item v;
            v.id = i;
			strcpy(v.name,"zhb");
            mymap->insert(ValueType(i, (Item)v));
        }
		
		gettimeofday(&tv_end, NULL);
		long interval = (tv_end.tv_sec - tv_begin.tv_sec)*1000000 + (tv_end.tv_usec - tv_begin.tv_usec);//单位是us
		
		printf("insert %d map, cost time:%lldus\n", counter, interval);
		
		/*
        for (MyMap::iterator it = mymap->begin(); it != mymap->end(); it++) {
            printf("%d-%s ", it->second.id, it->second.name);
        }
        
		printf("\n");*/

        //shared_memory_object::remove("SharedMemory");
    }
    catch (const std::exception & e) {
        printf("Exception:%s\n", e.what());
        shared_memory_object::remove("SharedMemory");
    }

    return 0;

}
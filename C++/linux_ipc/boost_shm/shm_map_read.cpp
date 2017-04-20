#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
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
#include <string>

using namespace boost::interprocess;
using std::string;

#define MAX_NAME_SIZE 32*1024

class Item
{
    public:
        Item(){}
        ~Item(){}

        int id;
        int size;
        char name[MAX_NAME_SIZE];
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
        managed_shared_memory segment(open_only, "SharedMemory");
		
		struct timeval tv_begin, tv_end;
		gettimeofday(&tv_begin, NULL);

        MyMap * mymap = segment.find<MyMap>("MyMap").first;
		
		gettimeofday(&tv_end, NULL);
		long interval = (tv_end.tv_sec - tv_begin.tv_sec)*1000000 + (tv_end.tv_usec - tv_begin.tv_usec);//单位是us
		
		printf("get map cost time:%lldus\n", interval);
		
        for (MyMap::iterator it = mymap->begin(); it != mymap->end(); it++) {
            printf("%d-%s ", it->second.id, it->second.name);
        }
        printf("\n");

        //shared_memory_object::remove("SharedMemory");
    }
    catch (const std::exception & e) {
        printf("Exception:%s\n", e.what());
        //shared_memory_object::remove("SharedMemory");
    }

    return 0;

}
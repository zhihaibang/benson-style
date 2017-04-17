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

class Item
{
    public:
        Item(){}
        ~Item(){}

        int id;
        int size;
        string name;
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

        MyMap * mymap = segment.find<MyMap>("MyMap").first;

        for (MyMap::iterator it = mymap->begin(); it != mymap->end(); it++) {
            printf("%d ", it->second.id);
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
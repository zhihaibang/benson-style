#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <iostream>

using namespace boost::interprocess;

//类型和allocator的定义，使用共享内存时需要使用boost::interprocess中
//重新实现的容器而不能使用标准容器
typedef managed_shared_memory::segment_manager                       segment_manager_t;
typedef allocator<void, segment_manager_t>                           void_allocator;
typedef allocator<int, segment_manager_t>                            int_allocator;
typedef vector<int, int_allocator>                                   int_vector;
typedef allocator<int_vector, segment_manager_t>                     int_vector_allocator;
typedef vector<int_vector, int_vector_allocator>                     int_vector_vector;
typedef allocator<char, segment_manager_t>                           char_allocator;
typedef basic_string<char, std::char_traits<char>, char_allocator>   char_string;

class complex_data
{
  int               id_;
  char_string       char_string_;
  int_vector_vector int_vector_vector_;

public:
  //因为void_allocator能够转换为任何类型的allocator<T>, 所以我们能够简单的在构造函数中
  //使用void_allocator来初始化所有的内部容器
  complex_data(int id, const char *name, const void_allocator &void_alloc)
    : id_(id), char_string_(name, void_alloc), int_vector_vector_(void_alloc)
  {}
  ~complex_data(){}
};

//将map的key定义为一个string，而把map的value定义为一个complex_data对象
typedef std::pair<const char_string, complex_data>                      map_value_type;
typedef allocator<map_value_type, segment_manager_t>                    map_value_type_allocator;
typedef map< char_string, complex_data, std::less<char_string>, map_value_type_allocator>          complex_map_type;

int main ()
{
	try {
	  //创建共享内存，根据测试，在32位的linux上，单块命名的共享内存大小
	  managed_shared_memory segment(open_or_create, "MySharedMemory", 65536);

	  //一个能够转换为任何allocator<T, segment_manager_t>类型的allocator 
	  void_allocator alloc_inst (segment.get_segment_manager());

	  //在共享内存上创建map
	  //如果map已由其他程序创建，或者不确定map是否已创建，应如下：
	  //complex_map_type *mymap = segment.find_or_construct<complex_map_type>
	  complex_map_type *mymap = segment.find_or_construct<complex_map_type>
	    ("MyMap")(std::less<char_string>(), alloc_inst);

	  for(int i = 0; i < 10; ++i){
	    //key(string) 和value(complex_data) 都需要在他们的构造函数中
		//包含一个allocator
		char tmp[16] = "";
		sprintf(tmp, "test%d", i);
	    char_string  key_object(tmp, alloc_inst);
	    complex_data mapped_object(i, "default_name", alloc_inst);
	    map_value_type value(key_object, mapped_object);
	    //向map中插值
	    mymap->insert(value);
	  }
	  
	  char tmp[16] = "";
	  sprintf(tmp, "test20");
	  char_string  key_object(tmp, alloc_inst);
		
	  complex_map_type::iterator it = mymap->find(key_object);
	  if(it != mymap->end())
	  {
		 mymap->erase(it);
	  }
	}
	catch (const std::exception & e) {
        printf("Exception:%s\n", e.what());
        //shared_memory_object::remove("MySharedMemory");
    }

  return 0;
}

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
  
  int id(){return id_;}
  
  const char_string& ret_char_string() const {return char_string_;}//注意函数名不能与typedef相同
  
};

//将map的key定义为一个string，而把map的value定义为一个complex_data对象
typedef std::pair<const char_string, complex_data>                      map_value_type;
typedef allocator<map_value_type, segment_manager_t>                    map_value_type_allocator;
typedef map< char_string, complex_data, std::less<char_string>, map_value_type_allocator>          complex_map_type;

int main ()
{
	try {
	  managed_shared_memory segment(open_read_only, "MySharedMemory"); 

	  void_allocator alloc_inst (segment.get_segment_manager());

	  complex_map_type * mymap = segment.find<complex_map_type>("MyMap").first;

      for (complex_map_type::iterator it = mymap->begin(); it != mymap->end(); it++) 
	  {
           printf("%s:%d-%s\n", it->first.c_str(), it->second.id(), it->second.ret_char_string().c_str());
      }
	}
	catch (const std::exception & e) {
        printf("Exception:%s\n", e.what());
        //shared_memory_object::remove("MySharedMemory");
    }

  return 0;
}

#include "LRUCache.h"
#include "color.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>

struct Student
{
	int id;
	const char* name;
};

int main(int argc, char const *argv[])
{
	if(argc != 2)
	{
		printf(L_RED"usage:"YELLOW" input capacity\n"NONE);
        return 0;
	}
	unsigned capacity = atoi(argv[1]);
	
	if(capacity > MAXSIZE)
	{
		printf(L_RED"errMsg:"YELLOW"capacity should less than 1048576\n"NONE);
        return 0;
	}

	LRUCache<Student> cache(capacity);
	printf("create cache: capacity=%u\n", capacity);

	string key = "zhb1";
	
	struct Student stu1 = {1,key.c_str()};
	cache.set(key,stu1);
	printf("set cache: key=%s,stu.id=%d,stu.name=%s\n", key.c_str(), stu1.id, stu1.name);

	key = "zhb2";
	struct Student stu2 = {2,key.c_str()};
	cache.set(key,stu2);
	printf("set cache: key=%s,stu.id=%d,stu.name=%s\n", key.c_str(), stu2.id, stu2.name);

	struct Student stu;
	int ret = cache.get(key,stu);
	if(ret == 0)
	{		
		printf("hit cache: key=%s,stu.id=%d,stu.name=%s\n", key.c_str(), stu.id, stu.name);
	}
	else
	{
		printf("miss cache, key=%s\n", key.c_str());
	}
	
	key = "zhb2";
	struct Student stu3 = {3,key.c_str()};
	cache.set(key,stu3);
	printf("set cache: key=%s,stu.id=%d,stu.name=%s\n", key.c_str(),stu3.id, stu3.name);

	ret = cache.get(key,stu);
	if(ret == 0)
	{		
		printf("hit cache: key=%s,stu.id=%d,stu.name=%s\n", key.c_str(), stu.id, stu.name);
	}
	else
	{
		printf("miss cache, key=%s\n", key.c_str());
	}

	key = "zhb3";
	ret = cache.get(key,stu);
	if(ret == 0)
	{		
		printf("hit cache: key=%s,stu.id=%d,stu.name=%s\n", key.c_str(), stu.id, stu.name);
	}
	else
	{
		printf("miss cache, key=%s\n", key.c_str());
	}

	return 0;
}
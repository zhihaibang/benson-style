#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <time.h>
#include "consistent_hash.h"
#include "murmurhash3.h"
#include "color.h"
using namespace std;

int main(int argc, char const *argv[])
{
	if(argc != 3)
	{
		printf(L_RED"usage:"YELLOW" ./main sample_count data_count\n"NONE);
		return 0;
	}
	
	int sample_count = atoi(argv[1]);
	int data_count = atoi(argv[2]);
	
	int node_num = 5;
	int virtual_num = 100;

	ConsistentHash* consistent_hash = new ConsistentHash(node_num, virtual_num);
	consistent_hash->Initialize();
	printf("consistent hash initialize success, node_num=%d, virtual_num=%d\n", node_num, virtual_num);
	
	vector<int> result(node_num,0);//节点存放数据数目统计
	vector<int> data_index(data_count,-1);//数据存放节点位置，下标是数据值i，它存放在data_index[i]上
	
	srand(time(NULL));
	for(int i=0; i<sample_count; ++i)
	{		
		int value = (rand()+getpid()) % data_count;
		stringstream ss;
		ss<<value;
		const char* key = ss.str().c_str();
		size_t index = consistent_hash->GetServerIndex(key);
		result[index]++;
		if(data_index[value] < 0 || (int)index != data_index[value])
		{
			printf("key = %s, index = %lu\n", key, index);
		}
		data_index[value] = index;
	}
	
	int error_index = 3;
	consistent_hash->DeleteNode(error_index);
	printf("node error,index = %d\n", error_index);	
	for(int i=0; i<sample_count; ++i)
	{		
		int value = (rand()+getpid()) % data_count;
		stringstream ss;
		ss<<value;
		const char* key = ss.str().c_str();
		size_t index = consistent_hash->GetServerIndex(key);
		result[index]++;
		if(data_index[value] < 0 || (int)index != data_index[value])
		{
			printf("key = %s, index = %lu\n", key, index);
		}
		data_index[value] = index;
	}
	
	consistent_hash->AddNewNode(error_index);
	printf("node recover,index = %d\n", error_index);	
	for(int i=0; i<sample_count; ++i)
	{		
		int value = (rand()+getpid()) % data_count;
		stringstream ss;
		ss<<value;
		const char* key = ss.str().c_str();
		size_t index = consistent_hash->GetServerIndex(key);
		result[index]++;
		if(data_index[value] < 0 || (int)index != data_index[value])
		{
			printf("key = %s, index = %lu\n", key, index);
		}
		data_index[value] = index;
	}
	
	for(int i=0;i<node_num; ++i)
	{
		printf("index = %d, data_count = %d\n", i, result[i]);
	}
		
	return 0;
}

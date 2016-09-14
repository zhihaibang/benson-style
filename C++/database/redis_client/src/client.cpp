#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <sstream>

#include "redis_client.h"
#include "redis_string.h"
#include "redis_list.h"
#include "redis_sortedset.h"
#include "redis_hash.h"

using namespace std;

#define HOSTNAME "192.168.144.120"
#define PORT 6381

void redis_string();
void redis_list();
void redis_sortedset();
void redis_hash();

int main()
{
	redis_string();
	redis_list();
	redis_hash();
	redis_sortedset();
	return 0;
}

void redis_string()
{
	const char* hostname = HOSTNAME;
	uint16_t port = PORT;
	RedisString* redis = new RedisString();
	int ret = redis->Connect(hostname,port);

	string key = "szhb";
	string value = "123456";
	ret = redis->Set(key,value);
	if(ret == 0){
		printf("set string success,key=%s,value=%s\n", key.c_str(), value.c_str());
	}else{
		printf("set string fail,err_code:%d,err_msg:%s\n", redis->err(),redis->errstr());
	}
	
	redis->Get("szhb",value);
	if(ret == 0){
		printf("get string success,value=%s\n\n", value.c_str());
	}else{
		printf("get string fail,err_code:%d,err_msg:%s\n\n", redis->err(), redis->errstr());
	}
	redis->Close();
}

void redis_list()
{
	const char* hostname = HOSTNAME;
	uint16_t port = PORT;
	RedisList* redis = new RedisList();
	int ret = redis->Connect(hostname,port);

	string key = "lzhb";
	string value = "list1";
	ret = redis->Push(key,value);
	if(ret == 0){
		printf("push list success,key=%s,value=%s\n", key.c_str(), value.c_str());
	}else{
		printf("push list fail,err_code:%d,err_msg:%s\n",redis->err(),redis->errstr());
	}
	
	ret = redis->Pop("lzhb",value);
	if(ret == 0){
		printf("pop list success,value=%s\n\n", value.c_str());
	}else{
		printf("pop list fail,err_code:%d,err_msg:%s\n\n", redis->err(), redis->errstr());
	}
	redis->Close();
}

void redis_hash()
{
	const char* hostname = HOSTNAME;
	uint16_t port = PORT;
	RedisHash* redis = new RedisHash();
	int ret = redis->Connect(hostname,port);

	string key = "hzhb";
	ret = redis->HSet(key,"field1","value1");
	if(ret != 0){
		printf("hash set fail,err_code:%d,err_msg:%s\n",redis->err(),redis->errstr());
	}
	ret = redis->HSet(key,"field2","value2");
	if(ret != 0){
		printf("hash set fail,err_code:%d,err_msg:%s\n",redis->err(),redis->errstr());
	}
	ret = redis->HSet(key,"field3","value3");
	if(ret != 0){
		printf("hash set fail,err_code:%d,err_msg:%s\n",redis->err(),redis->errstr());
	}

	string value;
	ret = redis->HGet(key,"field2",value);
	if(ret == 0){
		printf("hash get success,field:field2,value:%s\n",value.c_str());
	}else{
		printf("hash get fail,err_code:%d,err_msg:%s\n",redis->err(),redis->errstr());
	}

	map<string,string> map_values;
	ret = redis->HGetAll(key,map_values);
	if(ret == 0){
		printf("hash get success\n");
	}else{
		printf("hash get fail,err_code:%d,err_msg:%s\n",redis->err(),redis->errstr());
	}
	
	printf("HGetAll:\n");
	for(map<string,string>::iterator it = map_values.begin();it != map_values.end(); ++it)
	{
		printf("field:%s,value:%s\n",it->first.c_str(),it->second.c_str());
	}

	ret = redis->HDel(key,"field2");
	if(ret == 0){
		printf("hash delete field2 success\n");
	}else{
		printf("hash delete fail,err_code:%d,err_msg:%s\n",redis->err(),redis->errstr());
	}

	vector<string> lists;
	ret = redis->HKeys(key,lists);
	if(ret == 0){
		printf("hash hkeys success\n");
	}else{
		printf("hash hkeys fail,err_code:%d,err_msg:%s\n",redis->err(),redis->errstr());
	}

	printf("HKeys:\n");
	for(vector<string>::iterator it = lists.begin();it != lists.end(); ++it)
	{
		printf("%s\n",it->c_str());
	}

	ret = redis->HVals(key,lists);
	if(ret == 0){
		printf("hash hvals success\n");
	}else{
		printf("hash hvals fail,err_code:%d,err_msg:%s\n",redis->err(),redis->errstr());
	}

	printf("HVals:\n");
	for(vector<string>::iterator it = lists.begin();it != lists.end(); ++it)
	{
		printf("%s\n",it->c_str());
	}
	printf("\n");
	redis->Close();
}

void redis_sortedset()
{
	const char* hostname = HOSTNAME;
	uint16_t port = PORT;
	RedisSortedSet* redis = new RedisSortedSet();
	int ret = redis->Connect(hostname,port);

	string key = "zzhb";
	
	int count = 10;
	string member = "list";
	long start = time(NULL);

	for(int i=0;i<count;++i){
		long score = time(NULL);
		stringstream ss;
		ss<<member<<i;		
		ret = redis->Zadd(key, score, ss.str());
		if(ret == 0){
			//printf("zadd zset success,key=%s,score=%ld,member=%s\n", key.c_str(), score, member.c_str());
		}else{
			printf("zadd zset fail,err_code:%d,err_msg:%s\n", redis->err(), redis->errstr());
		}
	}

	
	long min = start;
	long max = time(NULL);
	vector<string> members;
	
	ret = redis->ZrangeByScore(key,min,max,members,false);
	if(ret == 0){
		printf("zrangebyscore zset success\n");
		int len = members.size();
		for(int i=0;i<len;++i){
			printf("members[%d]:%s\n",i,members[i].c_str());
		}
	}else{
		printf("zrangebyscore zset fail, err_code:%d,err_msg:%s\n", redis->err(), redis->errstr());
	}
	
	long long remove_count = -1;
	ret = redis->ZremRangeByScore(key,min,max,remove_count);
	if(ret == 0){
		printf("zremRangeByScore zset success,remove count:%lld\n",remove_count);
	}else{
		printf("zremRangeByScore zset fail\n");
	}
	
	ret = redis->ZrangeByScore(key,min,max,members,false);
	if(ret == 0){
		printf("zrangebyscore zset success,vector size:%d\n",(int)members.size());
	}else{
		printf("zrangebyscore zset fail, err_code:%d,err_msg:%s\n", redis->err(), redis->errstr());
	}
	
	redis->Close();
}


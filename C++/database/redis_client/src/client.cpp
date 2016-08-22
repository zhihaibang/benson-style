#include <stdio.h>
#include "redis_client.h"
#include "redis_string.h"
#include "redis_list.h"
#include "redis_sortedset.h"
#include <string>
#include <vector>
#include <sstream>
using namespace std;

#define HOSTNAME "192.168.144.120"
#define PORT 6381

void redis_string();
void redis_list();
void redis_sortedset();

int main()
{
	redis_string();
	redis_list();
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
		printf("set string fail,err:%s\n", redis->errstr());
	}
	
	redis->Get("szhb",value);
	if(ret == 0){
		printf("get string success,value=%s\n\n", value.c_str());
	}else{
		printf("get string fail,err:%s\n\n", redis->errstr());
	}
	redis->Close();
}

void redis_list()
{
	const char* hostname = HOSTNAME;
	uint16_t port = PORT;
	RedisList* redis = new RedisList();
	int ret = redis->Connect(hostname,port);
	if(ret == 0){
		printf("connect redis success\n");
	}else{
		printf("connect redis fail,err:%s\n", redis->errstr());
	}

	string key = "lzhb";
	string value = "list1";
	ret = redis->Push(key,value);
	if(ret == 0){
		printf("push list success,key=%s,value=%s\n", key.c_str(), value.c_str());
	}else{
		printf("push list fail,err:%s\n",redis->errstr());
	}
	
	ret = redis->Pop("lzhb",value);
	if(ret == 0){
		printf("pop list success,value=%s\n\n", value.c_str());
	}else{
		printf("pop list fail,err:%s\n\n", redis->errstr());
	}
	redis->Close();
}

void redis_sortedset()
{
	const char* hostname = HOSTNAME;
	uint16_t port = PORT;
	RedisSortedSet* redis = new RedisSortedSet();
	int ret = redis->Connect(hostname,port);

	string key = "zzhb";
	
	int count = 10000;
	string member = "list";
	long start = time(NULL);
	
	struct timeval tv_begin, tv_end;
	gettimeofday(&tv_begin, NULL);
	for(int i=0;i<count;++i){
		long score = time(NULL);
		stringstream ss;
		ss<<member<<i;		
		ret = redis->Zadd(key, score, ss.str());
		if(ret == 0){
			//printf("zadd zset success,key=%s,score=%ld,member=%s\n", key.c_str(), score, member.c_str());
		}else{
			printf("zadd zset fail,err:%s\n", redis->errstr());
		}
	}
	gettimeofday(&tv_end, NULL);
	long interval = (tv_end.tv_sec - tv_begin.tv_sec)*1000000 + (tv_end.tv_usec - tv_begin.tv_usec);
	printf("zadd zset %d members,cost time:%ldms\n",count,interval/1000);
	
	long min = start;
	long max = time(NULL);
	vector<string> members;
	
	gettimeofday(&tv_begin, NULL);
	ret = redis->ZrangeByScore(key,min,max,members);
	gettimeofday(&tv_end, NULL);
	
	if(ret == 0){
		printf("zrangebyscore zset success\n");
		int len = members.size();
		for(int i=0;i<len;++i){
			//printf("members[%d]:%s\n",i,members[i].c_str());
		}
	}else{
		printf("zrangebyscore zset fail,err:%s\n",  redis->errstr());
	}
	
	interval = (tv_end.tv_sec - tv_begin.tv_sec)*1000000 + (tv_end.tv_usec - tv_begin.tv_usec);
	printf("zrangebyscore zset %d members,cost time:%ldms\n",count,interval/1000);
	
	redis->Close();
}


#include <map>
#include <string.h>
#include <sstream>
#include "consistent_hash.h"
#include "murmurhash3.h"
using namespace std;

ConsistentHash::ConsistentHash(int node_num, int virtual_node_num)
{
	node_num_ = node_num;
	virtual_node_num_ = virtual_node_num;
}

ConsistentHash::~ConsistentHash()
{
	if(!server_nodes_.empty())
	{
		server_nodes_.clear();
	}
}


void ConsistentHash::Initialize()
{
	for(int i=0; i<node_num_; ++i)
	{
		for(int j=0; j<virtual_node_num_; ++j)
		{
			stringstream node_key;
			node_key<<"SHARD-"<<i<<"-NODE-"<<j;
			uint32_t partition = murmur3_32(node_key.str().c_str(), strlen(node_key.str().c_str()));
			server_nodes_.insert(pair<uint32_t, size_t>(partition, i));
		}
	}
}


size_t ConsistentHash::GetServerIndex(const char* key)
{
	uint32_t partition = murmur3_32(key, strlen(key));
	map<uint32_t, size_t>::iterator it = server_nodes_.lower_bound(partition);//沿环的顺时针找到一个大于等于key的虚拟节点

	if(it == server_nodes_.end())//未找到
	{
		return server_nodes_.begin()->second;
	}
	return it->second;
}


void ConsistentHash::DeleteNode(const int index)
{
	for(int j=0; j<virtual_node_num_; ++j)
	{
		stringstream node_key;
		node_key<<"SHARD-"<<index<<"-NODE-"<<j;
		uint32_t partition = murmur3_32(node_key.str().c_str(), strlen(node_key.str().c_str()));
		map<uint32_t,size_t>::iterator it = server_nodes_.find(partition);
		if(it != server_nodes_.end())
		{
			server_nodes_.erase(it);
		}
	}
}

void ConsistentHash::AddNewNode(const int index)
{
	for(int j=0; j<virtual_node_num_; ++j)
	{
		stringstream node_key;
		node_key<<"SHARD-"<<index<<"-NODE-"<<j;
		uint32_t partition = murmur3_32(node_key.str().c_str(), strlen(node_key.str().c_str()));
		server_nodes_.insert(pair<uint32_t, size_t>(partition, index));
	}
}
/*
 * =====================================================================================
 *
 * @Filename: LRUCache.h
 * @Description: 
 * @Version: 1.0.0
 * @Created time: 2016-05-04
 * @Update time: 
 * @Author: zhihaibang
 *
 * =====================================================================================
 */
#ifndef __LRUCACHE_H__
#define __LRUCACHE_H__

#include <unordered_map>
#include <string>
#include <list>
using namespace std;

#define MAXSIZE 1<<20 //1048576

template <typename Type>
class LRUCache
{
public:
    LRUCache(const unsigned capacity);
    ~LRUCache();
    int get(const string key, Type &value);
    void set(const string key, const Type &value);
	
private:
    struct CacheNode
    {
        string key;
        Type value;
        CacheNode(const string k, const Type &v):key(k), value(v){}
    };

    list<CacheNode> cacheList;
    unordered_map<string, typename list<CacheNode>::iterator> cacheMap;//need add typename
    unsigned size;
};



template <typename Type>
LRUCache<Type>::LRUCache(const unsigned capacity)
{
	if(capacity < MAXSIZE)
	{
		size = capacity;
	}
}

template <typename Type>
LRUCache<Type>::~LRUCache()
{
	cacheList.clear();
	cacheMap.clear();
	size = 0;
}

template <typename Type>
int LRUCache<Type>::get(const string key, Type &value)
{
	if(cacheMap.find(key) == cacheMap.end())//没有命中
    {
        return -1;
    }
    //命中
    typename list<CacheNode>::iterator it = cacheMap[key];
    cacheList.erase(it);
    //将命中的节点放到链表头部
    cacheList.push_front(CacheNode(key,it->value));
    cacheMap[key] = cacheList.begin();

    value = cacheMap[key]->value;
    return 0;
}

template <typename Type>
void LRUCache<Type>::set(const string key, const Type &value)
{
	if(cacheMap.find(key) == cacheMap.end())//没有命中
    {
        if(cacheList.size() == size)
        {
            cacheMap.erase(cacheList.back().key);
            cacheList.pop_back();
        }
        cacheList.push_front(CacheNode(key,value));
        cacheMap[key] = cacheList.begin();
    }
    else //命中
    {
        typename list<CacheNode>::iterator it = cacheMap[key];
        cacheList.erase(it);
        cacheList.push_front(CacheNode(key,value));
        cacheMap[key] = cacheList.begin();
    }
}


#endif


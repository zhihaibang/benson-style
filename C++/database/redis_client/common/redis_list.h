#ifndef __REDISLIST_H__
#define __REDISLIST_H__

/*
 * =====================================================================================
 *
 *       Filename:  redis_list.h
 *
 *    Description:  wrapper for redis client(list)
 *
 *        Version:  1.0
 *        Created:  2016-08-08
 *
 *         Author:  zhihaibang
 *
 * =====================================================================================
 */

#include "hiredis.h"
#include "redis_client.h"
#include <string>
#include <stdint.h>
using namespace std;

class RedisList : public RedisClient {
  public:
    int Push(const string &key, const string &value);
    int Push(const string &key, const char *value, size_t len);
    int Pop(const string &key, string &value);
};

#endif //__REDISLIST_H__
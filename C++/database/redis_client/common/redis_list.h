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

class RedisList : public RedisClient {
  public:
    int Push(const std::string &key, const std::string &value);
    int Push(const std::string &key, const char *value, size_t len);
    int Pop(const std::string &key, std::string &value);
};

#endif //__REDISLIST_H__
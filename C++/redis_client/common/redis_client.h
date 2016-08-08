#ifndef __REDISCLIENT_H__
#define __REDISCLIENT_H__

/*
 * =====================================================================================
 *
 *       Filename:  redis_client.h
 *
 *    Description:  wrapper for redis client
 *
 *        Version:  1.0
 *        Created:  2016-08-08
 *
 *         Author:  zhihaibang
 *
 * =====================================================================================
 */

#include "hiredis.h"
#include <string>
#include <stdint.h>

class RedisClient {
  public:
    RedisClient();
    virtual ~RedisClient();
    int Connect(const char *hostname, uint16_t port = 6379);
    int Ping();
    int ReConnect();
    void Close(); 
    int DBSize(int *sz);//返回当前数据库中key的数目
    int err();
    const char *errstr();
  protected:
    std::string hostname_;
    uint16_t port_;
    redisContext *c_;
    int err_;
    std::string errstr_;
};

#endif //!__REDISCLIENT_H__

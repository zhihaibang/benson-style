#ifndef __REDISSORTEDSET_H__
#define __REDISSORTEDSET_H__

/*
 * =====================================================================================
 *
 *       Filename:  redis_sortedset.h
 *
 *    Description:  wrapper for redis client(sortedset)
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
#include <vector>
#include <stdint.h>

class RedisSortedSet : public RedisClient {
  public:
    int Zadd(const std::string &key, const long score, const char *member, size_t len);
    int Zadd(const std::string &key, const long score, const std::string &member);
    int ZrangeByScore(const std::string &key, const long min, const long max, std::vector<std::string> &members, const bool withscore=true);
    int ZremRangeByScore(const std::string &key, const long min, const long max);

};

#endif
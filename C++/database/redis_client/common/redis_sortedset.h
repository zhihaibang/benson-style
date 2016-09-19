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
using namespace std;

class RedisSortedSet : public RedisClient {
  public:
    int Zadd(const string &key, const long score, const char *member, size_t len);
    int Zadd(const string &key, const long score, const string &member);
    int ZrangeByScore(const string &key, const long min, const long max, vector<string> &members, const bool withscore = false);
    int ZremRangeByScore(const string &key, const long min, const long max, long long &remove_count);

};

#endif
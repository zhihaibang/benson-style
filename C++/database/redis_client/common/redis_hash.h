#ifndef __REDISHASH_H__
#define __REDISHASH_H__

/*
 * =====================================================================================
 *
 *       Filename:  redis_hash.h
 *
 *    Description:  wrapper for redis client(hash)
 *
 *        Version:  1.0
 *        Created:  2016-09-14
 *
 *         Author:  zhihaibang
 *
 * =====================================================================================
 */
#include "hiredis.h"
#include "redis_client.h"
#include <string>
#include <map>
#include <vector>
#include <stdint.h>

using namespace std;

class RedisHash : public RedisClient {
  public:
    int HSet(const string &key, const string &field, const string &value);
    int HGet(const string &key, const string &field, string &value);
    int HGetAll(const string &key, map<string,string> &values);
    int HDel(const string &key, const string &field);
    int HKeys(const string &key, vector<string> &values);
    int HVals(const string &key, vector<string> &values);
    
};

#endif //!__REDISHASH_H__


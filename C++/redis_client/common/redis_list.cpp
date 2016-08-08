/*
 * =====================================================================================
 *
 *       Filename:  redis_list.cpp
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
#include "redis_list.h"
#include <string>
using namespace std;

int RedisList::Push(const string &key, const char *value, size_t len) {
  redisReply *reply = (redisReply *)redisCommand(
    c_,
    "rpush %b %b",
    key.c_str(),
    key.size(),
    value,
    len
  );

  int ret = -1;

  if (!reply) {
    if (c_->err) {
      err_ = c_->err;
      errstr_.assign(c_->errstr);
    }
  } else {
    if (REDIS_REPLY_ERROR == reply->type) {
      err_ = -1;
      errstr_.assign(reply->str, reply->len);
    } else {
      ret = 0;
    }

    freeReplyObject(reply);
  }

  return ret;
}

int RedisList::Push(const string &key, const string &value) {
  return Push(key, value.data(), value.size());
}

int RedisList::Pop(const string &key, string &value) {
  int ret = -1;
  redisReply *reply = (redisReply *)redisCommand(
    c_,
    "lpop %b",
    key.c_str(),
    key.size()
  );

  if (!reply) {
    if (c_->err) {
      err_ = c_->err;
      errstr_.assign(c_->errstr);
    }
  } else {
    switch (reply->type) {
      case REDIS_REPLY_STRING:
        value.assign(reply->str, reply->len);
        ret = 0;
        break;
      case REDIS_REPLY_NIL:
        ret = 0;
        break;
      case REDIS_REPLY_ERROR:
        err_ = -1;
        errstr_.assign(reply->str, reply->len);
        break;
      default:
        errstr_.assign("invalid type of reply");
        ret = -1;
        break;
    }

    freeReplyObject(reply);
  }

  return ret;
}
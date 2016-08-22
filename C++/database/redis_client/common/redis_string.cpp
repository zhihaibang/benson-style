/*
 * =====================================================================================
 *
 *       Filename:  redis_string.cpp
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
#include "redis_string.h"
#include <string>

int RedisString::Set(const std::string &key, const char *value, size_t len) {
  redisReply *reply = (redisReply *)redisCommand(
    c_,
    "set %b %b",
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

int RedisString::Set(const std::string &key, const std::string &value) {
  return Set(key, value.data(), value.size());
}

int RedisString::SetEX(const std::string &key, const char *value, size_t len, int ms) {
  redisReply *reply = (redisReply *)redisCommand(
    c_,
    "set %b %b EX %d",
    key.c_str(),
    key.size(),
    value,
    len,
    ms
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

int RedisString::SetEX(const std::string &key, const std::string &value, int ms) {
  return SetEX(key, value.data(), value.size(), ms);
}

int RedisString::Get(const std::string &key, std::string &value) {
  redisReply *reply = (redisReply *)redisCommand(
    c_,
    "get %b",
    key.c_str(),
    key.size()
  );

  int ret = -1;

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
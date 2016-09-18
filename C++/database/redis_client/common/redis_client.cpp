/*
 * =====================================================================================
 *
 *       Filename:  redis_client.cpp
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

using namespace std;

RedisClient::RedisClient() :c_(NULL) {}

int RedisClient::Connect(const char *hostname, uint16_t port) {
  hostname_.assign(hostname);
  port_ = port;

  Close();
  struct timeval timeout = {1, 600000};
  redisContext *c = redisConnectWithTimeout(hostname, port, timeout);

  if (c == NULL || c->err) {
    if (c) {
      redisFree(c);
    }

    return -1;
  }

  c_ = c;
  return 0;
}

int RedisClient::Ping() {
  int ret = 0;

  if (!c_) {
    ret = ReConnect();
  }

  return ret;
}

int RedisClient::ReConnect() {
  Close();

  struct timeval timeout = {1, 600000};
  redisContext *c = redisConnectWithTimeout(hostname_.c_str(), port_, timeout);

  if (c == NULL || c->err) {
    if (c) {
      redisFree(c);
    }

    return -1;
  }

  c_ = c;
  return 0;
}

void RedisClient::Close() {
  if (c_) {
    redisFree(c_);
    c_ = NULL;
  }
}

int RedisClient::DBSize(int *sz) 
{
  redisReply *reply = (redisReply *)redisCommand(c_, "dbsize");

  if (!reply) {
    if (c_->err) {
      err_ = c_->err;
      errstr_.assign(c_->errstr);
    }
    return -1;
  }
  
  int ret = -1;
  switch (reply->type) {
    case REDIS_REPLY_INTEGER:
      *sz = reply->integer;
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
  return ret;
}

int RedisClient::DeleteKey(const std::string &key)
{
  redisReply *reply = (redisReply *)redisCommand(
    c_,
    "del %b",
    key.c_str(),
    key.size()
  );

  if (!reply) {
    if (c_->err) {
      err_ = kRedisContextError;
      errstr_.assign(c_->errstr);
    }
    return -1;
  }

  int ret = -1;
  if (REDIS_REPLY_ERROR == reply->type) {
    err_ = kReplyError;
    errstr_.assign(reply->str, reply->len);
  } else if(REDIS_REPLY_INTEGER == reply->type){
    if(reply->integer >= 0){
      ret = 0;
    }
  } else {
    err_ = kUnknownErrror;
  }
  
  freeReplyObject(reply);
  return ret;
}

int RedisClient::err() {
  return err_;
}

const char *RedisClient::errstr() {
  return errstr_.c_str();
}

RedisClient::~RedisClient() {
  Close();
}

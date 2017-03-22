/*
 * =====================================================================================
 *
 *       Filename:  redis_client.cpp
 *
 *        Version:  1.0
 *        Created:  2016-08-08
 *         Update:  2016-09-19
 *
 *         Author:  zhihaibang
 *
 * =====================================================================================
 */

#include "hiredis.h"
#include "redis_client.h"
#include <string>
#include <vector>
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
      err_ = kReplyError;
      errstr_.assign(reply->str, reply->len);
      break;
    default:
	  err_ = kInvalidType;
      errstr_.assign("invalid type of reply");
      ret = -1;
      break;
  }

  freeReplyObject(reply);
  return ret;
}

int RedisClient::Expire(const string &key, const long time)
{
  redisReply *reply = (redisReply *)redisCommand(
    c_,
    "expire %b %ld",
    key.c_str(),
    key.size(),
    time
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
  } else {
    ret = 0;
  }

  freeReplyObject(reply);
  return ret;
}

int RedisClient::DeleteKey(const string &key)
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
    err_ = kUnknownError;
  }
  
  freeReplyObject(reply);
  return ret;
}

int RedisClient::GetAllKeys(vector<string> &keys)
{ 
  redisReply *reply = (redisReply *)redisCommand(c_,"keys *");
  
  keys.clear();
  if (!reply) {
    if (c_->err) {
      err_ = kRedisContextError;
      errstr_.assign(c_->errstr);
    }
    return -1;
  }

  int ret = -1,len=0;
  switch (reply->type) {
    case REDIS_REPLY_ARRAY:
      len = reply->elements;
      for(int i=0; i<len; ++i){
        redisReply* childReply = reply->element[i];
        if(childReply->type == REDIS_REPLY_STRING)
        {
          string key;
          key.assign(childReply->str, childReply->len);
          keys.push_back(key);
        }
      }
      ret = 0;
      break;
    case REDIS_REPLY_NIL:
      ret = 0;
      break;
    case REDIS_REPLY_ERROR:
      err_ = kReplyError;
      errstr_.assign(reply->str, reply->len);
      break;
    default:
      err_ = kInvalidType;
      errstr_.assign("invalid type of reply");
      break;
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


int RedisClient::WatchKey(const string &key)
{
  redisReply *reply = (redisReply *)redisCommand(
    c_,
    "watch %b",
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
  } else {
    ret = 0;
  }

  freeReplyObject(reply);
  return ret;
}

int RedisClient::BeginTransaction()
{
  redisReply *reply = (redisReply *)redisCommand(c_,"multi");

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
  } else {
    ret = 0;
  }

  freeReplyObject(reply);
  return ret;
}

int RedisClient::ExecTransaction()
{
  redisReply *reply = (redisReply *)redisCommand(c_,"exec");

  if (!reply) {
    if (c_->err) {
      err_ = kRedisContextError;
      errstr_.assign(c_->errstr);
    }
    return -1;
  }

  int ret = -1;
  if(REDIS_REPLY_ERROR == reply->type)
  {
    err_ = kReplyError;
  }
  else if(REDIS_REPLY_NIL == reply->type)
  {
    err_ = kTransactionAbort;
    errstr_.assign("transaction abort");
  }
  else
  {
    ret = 0;
  }

  freeReplyObject(reply);
  return ret;
}


int RedisClient::Discard()
{
  redisReply *reply = (redisReply *)redisCommand(c_,"discard");

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
  } else {
    ret = 0;
  }

  freeReplyObject(reply);
  return ret;
}

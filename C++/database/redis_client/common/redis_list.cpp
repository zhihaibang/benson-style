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
#include <sstream>
using namespace std;

int RedisList::Push(const string &key, const char *value, size_t len) 
{
  redisReply *reply = (redisReply *)redisCommand(
    c_,
    "rpush %b %b",
    key.c_str(),
    key.size(),
    value,
    len
  );

  if (!reply) 
  {
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

int RedisList::Push(const string &key, const string &value) 
{
  return Push(key, value.data(), value.size());
}

int RedisList::Pop(const string &key, string &value) 
{
  redisReply *reply = (redisReply *)redisCommand(
    c_,
    "lpop %b",
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
  switch (reply->type) {
    case REDIS_REPLY_STRING:
      value.assign(reply->str, reply->len);
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
      errstr_.assign("invalid type of reply");
      err_ = kInvalidType;
      ret = -1;
      break;
  }

  freeReplyObject(reply);
  return ret;
}

int RedisList::BatchPush(const string &key, vector<string> &values)
{
	stringstream ss;
	ss << "rpush %b ";
	for(vector<string>::iterator it = values.begin(); it != values.end(); ++it){
		ss << *it << " ";
	}
	redisReply *reply = (redisReply *)redisCommand(
		c_,
		ss.str().c_str(),
		key.c_str(),
		key.size()
	);

  if (!reply) 
  {
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
    ret = 0;//返回当前list的长度，不是新加的元素个数
  } 

  freeReplyObject(reply);
  return ret;
}
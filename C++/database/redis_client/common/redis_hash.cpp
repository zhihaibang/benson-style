/*
 * =====================================================================================
 *
 *       Filename:  redis_hash.cpp
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
#include "redis_hash.h"
#include <string>
#include <map>
#include <vector>

using namespace std;

int RedisHash::HSet(const string &key, const string &field, const string &value)
{
  redisReply *reply = (redisReply *)redisCommand(
    c_,
    "hset %b %b %b",
    key.c_str(),
    key.size(),
    field.c_str(),
    field.size(),
    value.c_str(),
    value.size()
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


int RedisHash::HGet(const string &key, const string &field, string &value)
{
  redisReply *reply = (redisReply *)redisCommand(
    c_,
    "hget %b %b",
    key.c_str(),
    key.size(),
    field.c_str(),
    field.size()
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
      break;
  }
  
  freeReplyObject(reply);
  return ret;
}


int RedisHash::HGetAll(const string &key, map<string,string> &map_values)
{
  redisReply *reply = (redisReply *)redisCommand(
    c_,
    "hgetall %b",
    key.c_str(),
    key.size()
  );

  map_values.clear();
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
      for(int i=1; i<len; i=i+2){
        redisReply* fieldReply = reply->element[i-1];
        redisReply* valueReply = reply->element[i];
        string filed;
        string value;
        if(fieldReply->type == REDIS_REPLY_STRING)
        {         
          filed.assign(fieldReply->str, fieldReply->len);
        }
        if(valueReply->type == REDIS_REPLY_STRING)
        {         
          value.assign(valueReply->str, valueReply->len);
        }
        map_values.insert(pair<string,string>(filed,value));
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

int RedisHash::HDel(const string &key, const string &field)
{
  redisReply *reply = (redisReply *)redisCommand(
    c_,
    "hdel %b %b",
    key.c_str(),
    key.size(),
    field.c_str(),
    field.size()
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


int RedisHash::HKeys(const string &key, vector<string> &values)
{ 
  redisReply *reply = (redisReply *)redisCommand(
    c_,
    "hkeys %b",
    key.c_str(),
    key.size()
  );
  
  values.clear();
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
          string value;
          value.assign(childReply->str, childReply->len);
          values.push_back(value);
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

int RedisHash::HVals(const string &key, vector<string> &values)
{ 
  redisReply *reply = (redisReply *)redisCommand(
    c_,
    "hvals %b",
    key.c_str(),
    key.size()
  );
  
  values.clear();
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
          string value;
          value.assign(childReply->str, childReply->len);
          values.push_back(value);
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
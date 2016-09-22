/*
 * =====================================================================================
 *
 *       Filename:  redis_sortedset.cpp
 *
 *        Version:  1.0
 *        Created:  2016-08-08
 *
 *         Author:  zhihaibang
 *
 * =====================================================================================
 */
#include <stdio.h>
#include "hiredis.h"
#include "redis_client.h"
#include "redis_sortedset.h"
#include <string>
#include <vector>
using namespace std;

int RedisSortedSet::Zadd(const string &key, const long score, const char *member, size_t len)
{
  redisReply *reply = (redisReply *)redisCommand(
    c_,
    "zadd %b %ld %b",
    key.c_str(),
    key.size(),
    score,
    member,
    len
  );

  if (!reply) {
    if (c_->err) {
      err_ = c_->err;
      errstr_.assign(c_->errstr);
    }
    return -1;
  }

  int ret = -1;
  if (REDIS_REPLY_ERROR == reply->type) {
    err_ = kReplyError;
    errstr_.assign(reply->str, reply->len);
  } 
  else if(REDIS_REPLY_INTEGER == reply->type)
	{
	  	if(reply->integer == 1)
	  	{
	  		ret = 0;
	  	}
	  	else if(reply->integer == 0)
	  	{
	  		errstr_ = "sortedset member has existed!";
	  	}
	}

  freeReplyObject(reply);
  return ret;
}

int RedisSortedSet::Zadd(const string &key, const long score, const string &member)
{
    return Zadd(key, score, member.data(), member.size());
}

int RedisSortedSet::ZrangeByScore(const string &key, const long min, const long max, vector<string> &members, const bool withscore)
{
  string command = "ZRANGEBYSCORE %b %ld %ld";
  if(withscore){
    command = "ZRANGEBYSCORE %b %ld %ld WITHSCORES";
  }
  
  redisReply *reply = (redisReply *)redisCommand(
    c_,
    command.c_str(),
    key.c_str(),
    key.size(),
    min,
    max
  );
  
  members.clear();
  if (!reply) {
    if (c_->err) {
      err_ = kRedisContextError;
      errstr_.assign(c_->errstr);
    }
    return -1;
  }

  int ret = -1,len=0;
  switch (reply->type) {
    case REDIS_REPLY_ARRAY://返回数组
    	len = reply->elements;
    	for(int i=0; i<len; ++i){
    		redisReply* childReply = reply->element[i];
    		if(childReply->type == REDIS_REPLY_STRING)
    		{
    			string value;
    			value.assign(childReply->str, childReply->len);
    			members.push_back(value);
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

int RedisSortedSet::ZremRangeByScore(const string &key, const long min, const long max, long long &remove_count)
{
  redisReply *reply = (redisReply *)redisCommand(
    c_,
    "ZREMRANGEBYSCORE %b %ld %ld",
    key.c_str(),
    key.size(),
    min,
    max
  );

  if (!reply){
    if (c_->err) {
      err_ = c_->err;
      errstr_.assign(c_->errstr);
    }
    return -1;
  }

  int ret = -1;
  if (REDIS_REPLY_ERROR == reply->type) {
    err_ = kReplyError;
    errstr_.assign(reply->str, reply->len);
  } else if(REDIS_REPLY_INTEGER == reply->type){
    remove_count = reply->integer;//返回被删除的数目
    ret = 0;
  } else {
    err_ = kUnknownError;
  }

  freeReplyObject(reply);
  return ret;
}
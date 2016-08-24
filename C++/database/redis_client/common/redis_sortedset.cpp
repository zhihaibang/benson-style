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

#include "hiredis.h"
#include "redis_client.h"
#include "redis_sortedset.h"
#include <string>
#include <vector>

int RedisSortedSet::Zadd(const std::string &key, const long score, const char *member, size_t len)
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

int RedisSortedSet::Zadd(const std::string &key, const long score, const std::string &member)
{
    return Zadd(key, score, member.data(), member.size());
}

int RedisSortedSet::ZrangeByScore(const std::string &key, const long min, const long max, std::vector<std::string> &members, const bool withscore)
{
  const char* withscore_str = "";
  if(withscore){
    withscore_str = "WITHSCORES";
  }

  redisReply *reply = (redisReply *)redisCommand(
    c_,
    "ZRANGEBYSCORE %b %ld %ld %s",
    key.c_str(),
    key.size(),
    min,
    max,
    withscore_str
  );

  int ret = -1;

  int len = 0;
  if (!reply) {
    if (c_->err) {
      err_ = c_->err;
      errstr_.assign(c_->errstr);
    }
  } else {
    switch (reply->type) {
      case REDIS_REPLY_ARRAY://返回数组
		len = reply->elements;
		for(int i=0;i<len;++i){
			std::string value = reply->element[i]->str;
			members.push_back(value);
		}
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

int RedisSortedSet::ZremRangeByScore(const std::string &key, const long min, const long max)
{
  redisReply *reply = (redisReply *)redisCommand(
    c_,
    "ZREMRANGEBYSCORE %b %ld %ld",
    key.c_str(),
    key.size(),
    min,
    max
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
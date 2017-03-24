#ifndef __REDISCLIENT_H__
#define __REDISCLIENT_H__

/*
 * =====================================================================================
 *
 *       Filename:  redis_client.h
 *
 *    Description:  wrapper for redis client
 *
 *        Version:  1.0
 *        Created:  2016-08-08
 *
 *         Author:  zhihaibang
 *
 * =====================================================================================
 */

#include "hiredis.h"
#include <string>
#include <stdint.h>
#include <vector>
using namespace std;

#define kRedisContextError 0x1
#define kReplyError 0x2
#define kInvalidType 0x3
#define kTransactionAbort 0x4
#define kUnknownError 0x5


class RedisClient {
  public:
    RedisClient();
    virtual ~RedisClient();
    int Connect(const char *hostname, uint16_t port = 6379);
    int Ping();
    int ReConnect();
    void Close(); 
    int err();
    const char *errstr();
    
    //关于key的函数
    int DBSize(int *sz);//返回当前数据库中key的数目
	int DeleteKey(const string &key);
	int Expire(const string &key, const long time);
	int GetAllKeys(vector<string> &keys);

    //事务
    int WatchKey(const string &key);
    int BeginTransaction();
    int ExecTransaction();
    int Discard();
     
/*如果某些函数（如redisConnect， redisCommand（调用不成功，函数返回值为NULL或者REDIS_ERR，此时context结构体中的err成员为非零值，可能为以下几种常量
REDIS_ERR_IO:当创建连接时（试着写socket或者读socket）发生的I/O错误。如果你在代码中包含了errno.h头文件，你便能得到准确的错误码。
REDIS_ERR_EOF:redis服务端已经关闭了此连接。
REDIS_ERR_PROTOCOL:服务端解析协议时发生了错误。
REDIS_ERR_OTHER:其他错误。目前仅仅表示无法解析目标主机名的错误。*/
  protected:
    string hostname_;
    uint16_t port_;
    redisContext *c_;
    int err_;
    string errstr_;
};

#endif //!__REDISCLIENT_H__

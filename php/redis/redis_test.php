<?php
/**
 * Created by PhpStorm.
 * User: zhihaibang1
 * Date: 2016/8/4
 * Time: 16:28
 */
require_once('RedisClient.php');
require_once('RedisString.php');
require_once('RedisHash.php');
require_once('RedisList.php');
require_once('RedisSet.php');
require_once('RedisSortedSet.php');

function microtime_float()
{
    list($usec, $sec) = explode(" ", microtime());
    return ((float)$usec + (float)$sec);
}

$host = "192.168.144.121";
$port = 6380;

//string操作
$redis_client = new RedisString($host, $port);
$time_start = microtime_float();
$redis_client->set("zhb","zhb1");
$value = $redis_client->get("zhb");
$time_end = microtime_float();
var_dump($value.$redis_client->errMsg);
//echo ((int)(($time_end-$time_start)*1000))."ms";//差值是秒数
$redis_client->closeRedis();

//hash操作
$redis_client = new RedisHash($host, $port);
$redis_client->hset('hzhb','filed','val');
$value = $redis_client->hget('hzhb','filed');
var_dump($value);
$redis_client->closeRedis();

//list操作
$redis_client = new RedisList($host, $port);
$redis_client->lpush('lzhb','list');
$type = $redis_client->getType('lzhb');
var_dump($type);
$value = $redis_client->lpop('lzhb');
var_dump($value);
$redis_client->closeRedis();

//set操作
$redis_client = new RedisSet($host, $port);
$redis_client->sadd('szhb','set1');
$redis_client->sadd('szhb','set2');
$value = $redis_client->smembers('szhb');
var_dump($value);
$redis_client->closeRedis();

//SortedSet操作
$redis_client = new RedisSortedSet($host, $port);
for($i=0;$i<20;$i++){
    $redis_client->zadd('zzhb',$i,'value'.$i);
}
$option = array('withscores'=>true,'limit'=>array(1,5));
$value = $redis_client->zrangebyscore('zzhb',2,10,$option);
var_dump($value);
$value = $redis_client->zrank('zzhb','value3');
var_dump($value);

//delete操作
$result = $redis_client->deleteKey("zhb",'hzhb');
var_dump('deleteKey result= '.$result);

$redis_client->closeRedis();





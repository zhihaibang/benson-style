<?php
/**
 * Created by PhpStorm.
 * User: zhihaibang1
 * Date: 2016/8/5
 * Time: 10:27
 */
require_once('RedisClient.php');

class RedisHash extends RedisClient{
    /**
     * 将哈希表key中的域field的值设为value。如果key不存在，一个新的哈希表被创建并进行HSET操作。如果域field已经存在于哈希表中，旧值将被覆盖
     *
     * @param  string key
     * @param  string field
     * @param  string value
     * @return int 成功返回1,旧值将被覆盖返回0,失败返回false
     */
    public function hset($key, $field, $value)
    {
        $this->clearERR();

        $key = trim($key);
        $field = trim($field);
        $value = trim($value);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->hset($key, $field, $value);
        }catch(RedisException $e){
            $this->setErrMsg($e, WriteError, __FUNCTION__);
        }

        return $result;
    }


    /**
     * 返回哈希表key中给定域field的值
     *
     * @param  string key
     * @param  string field
     * @return string或int 给定域的值，当给定域不存在或是给定key不存在时，返回nil
     */
    public function hget($key, $field)
    {
        $this->clearERR();

        $key = trim($key);
        $field = trim($field);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->hget($key, $field);
        }catch(RedisException $e){
            $this->setErrMsg($e, ReadError, __FUNCTION__);
        }

        return $result;
    }


    /**
     * 将多个field - value(域-值)对设置到哈希表key中。此命令会覆盖哈希表中已存在的域。如果key不存在，一个空哈希表被创建并执行HMSET操作
     *
     * @param  string key
     * @param  array field=>value
     * @return bool 如果命令执行成功，返回true。当key不是哈希表(hash)类型时，返回false
     */
    public function hmset($key, $values)
    {
        $this->clearERR();

        $key = trim($key);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->hmset($key, $values);
        }catch(RedisException $e){
            $this->setErrMsg($e, WriteError, __FUNCTION__);
        }

        return $result;
    }


    /**
     * 返回哈希表key中，一个或多个给定域的值。如果给定的域不存在于哈希表，那么返回一个nil值。
     *　因为不存在的key被当作一个空哈希表来处理，所以对一个不存在的key进行HMGET操作将返回一个只带有nil值的表
     *
     * @param  string key
     * @param  array fields
     * @return array 一个包含多个给定域的关联值的表，表值的排列顺序和给定域参数的请求顺序一样
     */
    public function hmget($key, $fields)
    {
        $this->clearERR();

        $key = trim($key);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->hmget($key, $fields);
        }catch(RedisException $e){
            $this->setErrMsg($e, ReadError, __FUNCTION__);
        }

        return $result;
    }


    /**
     * 返回哈希表key中，所有的域和值。在返回值里，紧跟每个域名(field name)之后是域的值(value)，所以返回值的长度是哈希表大小的两倍
     *
     * @param  string key
     * @param  array fields
     * @return array 以列表形式返回哈希表的域和域的值。 若key不存在，返回空列表
     */
    public function hgetall($key)
    {
        $this->clearERR();

        $key = trim($key);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->hgetall($key);
        }catch(RedisException $e){
            $this->setErrMsg($e, ReadError, __FUNCTION__);
        }

        return $result;
    }


    /**
     * 返回哈希表key中的所有域
     *
     * @param  string key
     * @return array 一个包含哈希表中所有域的表。当key不存在时，返回一个空表
     */
    public function hkeys($key)
    {
        $this->clearERR();

        $key = trim($key);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->hkeys($key);
        }catch(RedisException $e){
            $this->setErrMsg($e, ReadError, __FUNCTION__);
        }

        return $result;
    }


    /**
     * 返回哈希表key中的所有域
     *
     * @param  string key
     * @return array 一个包含哈希表中所有值的表。当key不存在时，返回一个空表
     */
    public function hvals($key)
    {
        $this->clearERR();

        $key = trim($key);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->hvals($key);
        }catch(RedisException $e){
            $this->setErrMsg($e, ReadError, __FUNCTION__);
        }

        return $result;
    }


    /**
     * 删除哈希表key中的一个或多个指定域，不存在的域将被忽略
     *
     * @param  string key
     * @param  array fields
     * @return int 被成功移除的域的数量，不包括被忽略的域
     */
    public function hdel($key, $fields)
    {
        $this->clearERR();

        $keys = trim($key);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->hdel($keys, $fields);
        }catch(RedisException $e){
            $this->setErrMsg($e, WriteError, __FUNCTION__);
        }

        return $result;
    }
}
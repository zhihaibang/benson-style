<?php
/**
 * Created by PhpStorm.
 * User: zhihaibang1
 * Date: 2016/8/5
 * Time: 10:24
 */
require_once('RedisClient.php');
require_once('define.php');

class RedisList extends RedisClient{
    /**
     * 由列表头部添加字符串值。如果不存在该键则创建该列表。如果该键存在，而且不是一个列表，返回FALSE
     *
     * @param  string key
     * @param  string value
     * @return int或bool 成功返回数组长度，失败false
     */
    public function lpush($key, $value)
    {
        $this->clearERR();

        $key = trim($key);
        $value = trim($value);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->lpush($key, $value);
        }catch(RedisException $e){
            $this->setErrMsg($e, WriteError, __FUNCTION__);
        }

        return $result;
    }


    /**
     * 由列表尾部添加字符串值。如果不存在该键则创建该列表。如果该键存在，而且不是一个列表，返回FALSE
     *
     * @param  string key
     * @param  string value
     * @return int或bool 成功返回数组长度，失败false
     */
    public function rpush($key, $value)
    {
        $this->clearERR();

        $key = trim($key);
        $value = trim($value);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->rpush($key, $value);
        }catch(RedisException $e){
            $this->setErrMsg($e, WriteError, __FUNCTION__);
        }

        return $result;
    }


    /**
     * 返回和移除列表的第一个元素
     *
     * @param  string key
     * @return string或bool 成功返回第一个元素的值 ，失败返回false
     */
    public function lpop($key)
    {
        $this->clearERR();

        $key = trim($key);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->lpop($key);
        }catch(RedisException $e){
            $this->setErrMsg($e, ReadError, __FUNCTION__);
        }

        return $result;
    }


    /**
     * 返回的列表的长度。如果列表不存在或为空，该命令返回0。如果该键不是列表，该命令返回FALSE
     *
     * @param  string key
     * @return int或bool 成功返回数组长度，失败false
     */
    public function llen($key)
    {
        $this->clearERR();

        $key = trim($key);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->lsize($key);
        }catch(RedisException $e){
            $this->setErrMsg($e, ReadError, __FUNCTION__);
        }

        return $result;
    }


    /**
     * 为列表指定的索引赋新的值,若不存在该索引返回false.
     *
     * @param  string key
     * @param  int index
     * @param  string value
     * @return bool 成功返回true,失败false
     */
    public function lset($key, $index, $value)
    {
        $this->clearERR();

        $key = trim($key);
        $value = trim($value);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->lset($key, $index, $value);
        }catch(RedisException $e){
            $this->setErrMsg($e, WriteError, __FUNCTION__);
        }

        return $result;
    }


    /**
     * 返回指定键存储在列表中指定的元素。
     * 0第一个元素，1第二个… -1最后一个元素，-2的倒数第二…
     * 错误的索引或键不指向列表则返回FALSE
     *
     * @param  string key
     * @param  int index
     * @return string或bool 成功返回指定元素的值，失败false
     */
    public function lget($key, $index)
    {
        $this->clearERR();

        $key = trim($key);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->lget($key, $index);
        }catch(RedisException $e){
            $this->setErrMsg($e, ReadError, __FUNCTION__);
        }

        return $result;
    }
}
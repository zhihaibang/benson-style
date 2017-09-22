<?php
/**
 * Created by PhpStorm.
 * User: zhihaibang1
 * Date: 2016/8/5
 * Time: 9:48
 */
require_once('RedisClient.php');

class RedisSet extends RedisClient{

    /**
     * 添加元素到set中
     *
     * @param $key
     * @param $value
     * @return bool
     */
    public function sadd($key, $value)
    {
        $this->clearERR();

        $key = trim($key);
        $value = trim($value);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->SADD($key, $value);
        }catch(RedisException $e){
            $this->setErrMsg($e, WriteError, __FUNCTION__);
        }

        return $result;
    }

    /**
    * 批量添加元素到set中
    * @param  键名，元素数组   
    * @return 添加成功的元素数目
    */
    public function sadds($key, $members)
    {
        $this->clearERR();

        $key = trim($key);
        
        if(!$this->checkConnection()){
            return false;
        }

        $result = false;
        
        $args = array();
        array_push($args, $key);
        foreach($members as $key => $value)
        {
            array_push($args, $value);
        }

        try{
            $result = call_user_func_array(array($this->redisclient, 'sAdd'), $args);
        }catch(RedisException $e){
            $this->setErrMsg($e, WriteError, __FUNCTION__);
        }

        return $result;
    }


    /**
     * 返回集合 key 中的所有成员,不存在的 key 被视为空集合。
     *
     * @param $key
     * @return bool,array
     */
    public function smembers($key)
    {
        $this->clearERR();

        $key = trim($key);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->SMEMBERS($key);
        }catch(RedisException $e){
            $this->setErrMsg($e, ReadError, __FUNCTION__);
        }

        return $result;
    }

    /**
     * 判断 member 元素是否集合 key 的成员
     * 如果 member 元素是集合的成员，返回 1 。如果 member 元素不是集合的成员，或 key 不存在，返回 0 。
     *
     * @param $key
     * @param $value
     * @return bool,int
     */
    public function sismember($key,$value)
    {
        $this->clearERR();

        $key = trim($key);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->SISMEMBER($key,$value);
        }catch(RedisException $e){
            $this->setErrMsg($e, ReadError, __FUNCTION__);
        }

        return $result;
    }

    /**
     * 移除集合 key 中的一个或多个 member 元素，不存在的member元素会被忽略
     *
     * @param $key
     * @param $value
     * @return bool,int,被成功移除的元素的数量，不包括被忽略的元素
     */
    public function srem($key,$value)
    {
        $this->clearERR();

        $key = trim($key);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->SREM($key,$value);
        }catch(RedisException $e){
            $this->setErrMsg($e, WriteError, __FUNCTION__);
        }

        return $result;
    }



}
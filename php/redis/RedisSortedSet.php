<?php
/**
 * Created by PhpStorm.
 * User: zhihaibang1
 * Date: 2016/8/5
 * Time: 9:48
 */
require_once('RedisClient.php');

class RedisSortedSet extends RedisClient{

    /**
     * 添加元素到SortedSet中
     *
     * @param $key
     * @param $score
     * @param $member
     * @return bool
     */
    public function zadd($key, $score, $member)
    {
        $this->clearERR();

        $key = trim($key);
        $member = trim($member);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->ZADD($key, $score, $member);
        }catch(RedisException $e){
            $this->setErrMsg($e, WriteError, __FUNCTION__);
        }

        return $result;
    }


    /**
     * 返回有序集 key 中，所有 score 值介于 min 和 max 之间(包括等于 min 或 max )的成员
     *
     * @param $key
     * @param $min
     * @param $max
     * @param $option,array('withscores'=>true,'limit'=>array(1,5));
     * @return bool
     */
    public function zrangebyscore($key, $min, $max, $option=null)
    {
        $this->clearERR();

        $key = trim($key);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            if(isset($option)){
                $result = $this->redis->ZRANGEBYSCORE($key,$min,$max,$option);
            }else{
                $result = $this->redis->ZRANGEBYSCORE($key,$min,$max);
            }
        }catch(RedisException $e){
            $this->setErrMsg($e, ReadError, __FUNCTION__);
        }

        return $result;
    }


    /**
     * 返回有序集 key中成员 member的排名。其中有序集成员按 score 值递增(从小到大)顺序排列
     * 排名以 0 为底，也就是说， score 值最小的成员排名为 0
     *
     * @param $key
     * @param $member
     * @return bool,int
     */
    public function zrank($key,$member)
    {
        $this->clearERR();

        $key = trim($key);

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->ZRANK($key,$member);
        }catch(RedisException $e){
            $this->setErrMsg($e, ReadError, __FUNCTION__);
        }

        return $result;
    }







}
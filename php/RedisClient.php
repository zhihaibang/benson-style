<?php

class RedisClient
{
	/**
	 * Redis机器ip
	 *
	 * @var string
	 */
	private $host;

	/**
	 * Redis端口
	 *
	 * @var string
	 */
	private $port;

	/**
	 * 数据库号
	 */
	private $db_number;

	/**
	 * Redis连接
	 *
	 * @var object
	 */
	private $redis;

	/**
	 * 是否连接
	 * @var bool
	 */
	private $con;

	/**
	 * 错误编码
	 *
	 * @var int
	 * 500:connect error
     * 501:write redis error
     * 502:read redis error
	 */
	public $errCode = 0;

	/**
	 * 错误信息
	 *
	 * @var string
	 */
	public $errMsg = '';

	/**
	 * 清除错误标识，在每个函数调用前调用
	 */
	private function clearERR()
	{
		$this->errCode = 0;
		$this->errMsg  = '';
	}

	/**
     *
     * @param string host     机器ip
     * @param int    port     端口
     * @param string db_number  数据库号，默认为0
     */
    public function __construct($host, $port, $db_number = 0)
    {
        $this->host = $host;
        $this->port = $port;
        $this->db_number = $db_number;
        $this->init();
    }


    /**
     * 初始化对象
     */
    private function init()
    {
    	try{
    		$this->$redis = new redis(); 
			$this->$con = $this->$redis->connect($this->host, $this->port);
    	} catch (RedisException $e) {
    		$this->errCode = 500;
			$this->errMsg = "redis:{$this->host}:{$this->port} connection failed:" . $e->getMessage();
			return false;
    	}
    	return true;
    }


    public function __destruct()
    {
        $this->closeRedis();
    }


    /**
     * 关闭数据库连接
     */
    function closeRedis()
    {
        if ($this->con) {
            $this->$redis->close();
        }
        return true;
    }


    /**
     * 检查Redis连接,是否有效，无效则重新建立
     */
    private function checkConnection()
    {
        if (!$this->con && !$this->redis)
        {
            $this->closeRedis();
            return $this->init();
        }
        return true;
    }


    /**
     * 设置key和value的值
     *
     * @param  string key
     * @param  string value
     * @return bool 成功返回true 否则返回false
     */
    public function setString($key, $value)
    {
    	$this->clearERR();

    	$key = trim($key);
    	$value = trim($value);

    	if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
        	$result = $this->$redis->set($key, $value);
        }catch(RedisException $e){
        	$this->errCode = 501;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',err_function:setString, errMsg:' . $e->getMessage();
        }

        return $result;
    }


    /**
     * 获取有关指定键的值
     *
     * @param  string key
     * @return string或bool 如果键不存在，则返回 FALSE。否则，返回指定键对应的value值
     */
    public function getString($key)
    {
    	$this->clearERR();

    	$key = trim($key);

    	if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
        	$result = $this->$redis->get($key, $value);
        }catch(RedisException $e){
        	$this->errCode = 501;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',err_function:getString, errMsg:' . $e->getMessage();
        }

        return $result;
    }


    /**
     * 由列表头部添加字符串值。如果不存在该键则创建该列表。如果该键存在，而且不是一个列表，返回FALSE
     *
     * @param  string key
     * @param  string value
     * @return int或bool 成功返回数组长度，失败false
     */
    public function lpushList($key, $value)
    {
    	$this->clearERR();

    	$key = trim($key);
    	$value = trim($value);

    	if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
        	$result = $this->$redis->lpush($key, $value);
        }catch(RedisException $e){
        	$this->errCode = 501;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',err_function:lpushList, errMsg:' . $e->getMessage();
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
    public function rpushList($key, $value)
    {
    	$this->clearERR();

    	$key = trim($key);
    	$value = trim($value);

    	if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
        	$result = $this->$redis->rpush($key, $value);
        }catch(RedisException $e){
        	$this->errCode = 501;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',err_function:rpushList, errMsg:' . $e->getMessage();
        }

        return $result;
    }


    /**
     * 返回和移除列表的第一个元素
     *
     * @param  string key
     * @return string或bool 成功返回第一个元素的值 ，失败返回false
     */
    public function lpopList($key)
    {
    	$this->clearERR();

    	$key = trim($key);

    	if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
        	$result = $this->$redis->lpop($key);
        }catch(RedisException $e){
        	$this->errCode = 502;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',err_function:lpopList, errMsg:' . $e->getMessage();
        }

        return $result;
    }


    /**
     * 返回的列表的长度。如果列表不存在或为空，该命令返回0。如果该键不是列表，该命令返回FALSE
     *
     * @param  string key
     * @return int或bool 成功返回数组长度，失败false
     */
    public function llenList($key)
    {
    	$this->clearERR();

    	$key = trim($key);

    	if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
        	$result = $this->$redis->lsize($key);
        }catch(RedisException $e){
        	$this->errCode = 501;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',err_function:llenList, errMsg:' . $e->getMessage();
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
    public function lsetList($key, $index, $value)
    {
    	$this->clearERR();

    	$key = trim($key);
    	$value = trim($value);

    	if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
        	$result = $this->$redis->lset($key, $index, $value);
        }catch(RedisException $e){
        	$this->errCode = 501;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',err_function:lsetList, errMsg:' . $e->getMessage();
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
    public function lgetList($key, $index)
    {
    	$this->clearERR();

    	$key = trim($key);

    	if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
        	$result = $this->$redis->lget($key, $index);
        }catch(RedisException $e){
        	$this->errCode = 502;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',err_function:lgetList, errMsg:' . $e->getMessage();
        }

        return $result;
    }

    /**
     * 将哈希表key中的域field的值设为value。如果key不存在，一个新的哈希表被创建并进行HSET操作。如果域field已经存在于哈希表中，旧值将被覆盖
     *
     * @param  string key
     * @param  string field
     * @param  string value
     * @return int 成功返回1,旧值将被覆盖返回0,失败返回false
     */
    public function hsetHash($key, $field, $value)
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
        	$result = $this->$redis->hset($key, $field, $value);
        }catch(RedisException $e){
        	$this->errCode = 501;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',err_function:hsetHash, errMsg:' . $e->getMessage();
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
    public function hgetHash($key, $field)
    {
    	$this->clearERR();

    	$key = trim($key);
    	$field = trim($field);

    	if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
        	$result = $this->$redis->hget($key, $field);
        }catch(RedisException $e){
        	$this->errCode = 502;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',err_function:hgetHash, errMsg:' . $e->getMessage();
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
    public function hmsetHash($key, $values)
    {
    	$this->clearERR();

    	$key = trim($key);

    	if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
        	$result = $this->$redis->hmset($key, $values);
        }catch(RedisException $e){
        	$this->errCode = 501;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',err_function:hmsetHash, errMsg:' . $e->getMessage();
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
    public function hmgetHash($key, $fields)
    {
    	$this->clearERR();

    	$key = trim($key);

    	if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
        	$result = $this->$redis->hmget($key, $fields);
        }catch(RedisException $e){
        	$this->errCode = 502;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',err_function:hmgetHash, errMsg:' . $e->getMessage();
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
    public function hgetallHash($key)
    {
    	$this->clearERR();

    	$key = trim($key);

    	if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
        	$result = $this->$redis->hgetall($key);
        }catch(RedisException $e){
        	$this->errCode = 502;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',err_function:hgetallHash, errMsg:' . $e->getMessage();
        }

        return $result;
    }


    /**
	 * 返回哈希表key中的所有域
     *
     * @param  string key
     * @return array 一个包含哈希表中所有域的表。当key不存在时，返回一个空表
     */
    public function hkeysHash($key)
    {
    	$this->clearERR();

    	$key = trim($key);

    	if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
        	$result = $this->$redis->hkeys($key);
        }catch(RedisException $e){
        	$this->errCode = 502;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',err_function:hkeysHash, errMsg:' . $e->getMessage();
        }

        return $result;
    }


    /**
	 * 返回哈希表key中的所有域
     *
     * @param  string key
     * @return array 一个包含哈希表中所有值的表。当key不存在时，返回一个空表
     */
    public function hvalsHash($key)
    {
    	$this->clearERR();

    	$key = trim($key);

    	if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
        	$result = $this->$redis->hvals($key);
        }catch(RedisException $e){
        	$this->errCode = 502;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',err_function:hvalsHash, errMsg:' . $e->getMessage();
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
    public function hdelHash($key, $fields)
    {
    	$this->clearERR();

    	$keys = trim($key);

    	if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
        	$result = $this->$redis->hdel($keys, $fields);
        }catch(RedisException $e){
        	$this->errCode = 501;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',err_function:hdelHash, errMsg:' . $e->getMessage();
        }

        return $result;
    }


    /**
     * 删除指定的键
     *
     * @param  一个键，或不确定数目的参数，每一个关键的数组：key1 key2 key3 … keyN
     * @return int 删除成功的项数
     */
    public function deleteKey()
    {
    	$this->clearERR();

    	$keys = func_get_args();

    	if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
        	$result = $this->$redis->delete($keys);
        }catch(RedisException $e){
        	$this->errCode = 501;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',err_function:deleteKey, errMsg:' . $e->getMessage();
        }

        return $result;
    }

}
<?php
require_once('define.php');

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
    protected $redis;

	/**
	 * 是否连接
	 * @var bool
	 */
    private $con;

	/**
	 * 错误编码
	 *
	 * @var int
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
	protected function clearERR()
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
        $this->connect();
    }


    /**
     * 初始化对象
     */
    private function connect()
    {
    	try{
    		$this->redis = new redis();
			$this->con = $this->redis->connect($this->host, $this->port);
    	} catch (RedisException $e) {
            $this->setErrMsg($e, ConnectError, __FUNCTION__);
			return false;
    	}
		if(!$this->con)
		{
			$e = new RedisException();
			$this->setErrMsg($e, ConnectError, __FUNCTION__);
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
    public function closeRedis()
    {
        if ($this->con) {
            $this->redis->close();
        }
        return true;
    }


    /**
     * 检查Redis连接,是否有效，无效则重新建立
     */
    protected function checkConnection()
    {
        if (!$this->con && !$this->redis)
        {
            $this->closeRedis();
            return $this->connect();
        }
        return true;
    }

    /**
     * 设置错误码和错误信息
     */
    protected function setErrMsg(RedisException $e, $err_code, $function)
    {
        $this->errCode = $err_code;
        $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',err_function:'.$function.', errMsg:' . $e->getMessage();
    }

    /**
     * 获取对应键值的数据类型
     *  none(key不存在) int(0)
        string(字符串) int(1)
        list(列表) int(3)
        set(集合) int(2)
        zset(有序集) int(4)
        hash(哈希表) int(5)
     *
     * @param $key
     * @return bool
     */
    public function getType($key)
    {
        $this->clearERR();

        if(!$this->checkConnection()){
            return false;
        }

        $result = false;

        try{
            $result = $this->redis->type($key);
        }catch(RedisException $e){
            $this->setErrMsg($e, ReadError, __FUNCTION__);
        }

        return $result;
    }

    /**
     * 删除指定的键
     *
     * @param  一个键，或不确定数目的参数，键值的数组：key1 key2 key3 … keyN
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
        	$result = $this->redis->delete($keys);
        }catch(RedisException $e){
            $this->setErrMsg($e, WriteError, __FUNCTION__);
        }

        return $result;
    }

}
<?php
/**
 * Created by PhpStorm.
 * Author: zhihaibang
 * Date: 2016/01/27
 * Time: 15:21
 */
class DB
{
    /**
     * 数据库机器ip
     *
     * @var string
     */
    private $host;

    /**
     * 数据库端口
     *
     * @var string
     */
    private $port;

    /**
     * 用户名
     *
     * @var string
     */
    private $user;

    /**
     * 密码
     *
     * @var string
     */
    private $password;

    /**
     * db对象
     *
     */
    private $db;
    /**
     * 数据库名称
     */
    private $db_name;

    /**
     * 错误编码
     *
     * @var int
     * 500:initial error
     * 501:execute sql error
     * 502:prepare sql error
     * 600:input parameters format error
     * 601:input parameters pdo type error
     */
    public $errCode = 0;

    /**
     * 错误信息
     *
     * @var string
     */
    public $errMsg = '';

    /**
     * pdo数据类型
     *
     * @var array
     */
    private $pdoDataType = array(PDO::PARAM_BOOL,PDO::PARAM_INT,PDO::PARAM_STR,PDO::PARAM_LOB);


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
     * @param string db_name  数据库名称
     * @param string user     用户名称
     * @param string password 密码
     */
    public function __construct($host, $port, $db_name, $user, $password)
    {
        $this->host = $host;
        $this->port = $port;
        $this->db_name = $db_name;
        $this->user = $user;
        $this->password = $password;
        $this->magic_quotes = get_magic_quotes_gpc();
        $this->init();
    }

    public function __destruct()
    {
        $this->closeDB();
    }


    /**
     * 初始化对象
     */
    private function init()
    {
        $dsn = "mysql:dbname={$this->db_name};host={$this->host}:{$this->port}";
        try {
            $this->db = new PDO($dsn,$this->user, $this->password);
        } catch (PDOException $e) {
            $this->errCode = 500;
            $this->errMsg = "db:{$this->host}:{$this->port}:{$this->db_name} Connection failed:" . $e->getMessage();
            return false;
        }
        return true;
    }

    /**
     * 关闭数据库连接
     */
    private function closeDB()
    {
        if ($this->db) {
            $this->db = null;
        }
        return true;
    }

    /**
     * 检查数据库连接,是否有效，无效则重新建立
     */
    private function checkConnection()
    {
        if (!$this->db)
        {
            $this->closeDB();
            return $this->init();
        }
        return true;
    }


    /**
     * 执行不带参数的select语句
     *
     * @param  string sql   sql语句
     * @return bool 正确返回true 否则返回false
     */
    public function getRows($sql)
    {
        $this->clearERR();
        $sql = trim($sql);

        if(!$this->checkConnection()){
            return false;
        }
        $this->db->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
        $stmt = $this->db->prepare($sql);

        if ($stmt === false){
            $this->errCode = 502;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',db:'.$this->db_name.',sql:'.$sql. ',errCode:502';
            return false;
        }

        $result = $stmt->execute();

        if ($result === false){
            $this->errCode = 501;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',db:'.$this->db_name.',sql:'.$sql. ',errMsg:' . $stmt->errorInfo()[2];
            return false;
        }
        $res = $stmt->fetchAll();
        return $res;
    }

    /**
     * 执行select语句
     * 绑定带问号的参数
     *
     * @param  string sql   sql语句
     * @param  array data   顺序传入的数据，key是int类型
     * @return bool 正确返回数据,错误返回false
     */
    public function getRowsQ($sql, $data)
    {
        $this->clearERR();
        $sql = trim($sql);

        if(!$this->checkConnection()){
            return false;
        }
        $this->db->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
        $stmt = $this->db->prepare($sql);

        if ($stmt === false){
            $this->errCode = 502;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',db:'.$this->db_name.',sql:'.$sql. ',errCode:502';
            return false;
        }

        foreach ($data as $k => $v)
        {
            if ( !is_numeric($k) ) {
                $this->errCode = 600;
                $this->errMsg = "input parameters format error";
                return false;
            }
            $v_str = $v;
            $stmt->bindValue($k+1, $v_str);
        }
        $result = $stmt->execute();

        if ($result === false){
            $this->errCode = 501;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',db:'.$this->db_name.',sql:'.$sql. ',errMsg:' . $stmt->errorInfo()[2];
            return false;
        }
        $res = $stmt->fetchAll();
        return $res;
    }

    /**
     * 执行select语句
     * 绑定带问号的参数，带类型
     *
     * @param  string sql   sql语句
     * @param  array data   顺序传入的数据，key是int类型,{'t'=>'','v'=>''},t是类型，v是值
     * @return bool 正确返回数据,错误返回false
     */
    public function getRowsQT($sql, $data)
    {
        $this->clearERR();
        $sql = trim($sql);

        if(!$this->checkConnection()){
            return false;
        }
        $this->db->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
        $stmt = $this->db->prepare($sql);

        if ($stmt === false){
            $this->errCode = 502;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',db:'.$this->db_name.',sql:'.$sql. ',errCode:502';
            return false;
        }

        foreach ($data as $k => $v)
        {
            if ( !is_numeric($k) ) {
                $this->errCode = 600;
                $this->errMsg = "input parameters format error";
                return false;
            }
            $t_str = $v['t'];
            if(!isset($t_str)){
                $this->errCode = 600;
                $this->errMsg = "input parameters format error";
                return false;
            }
            if(!in_array($t_str,$this->pdoDataType)){
                $this->errCode = 601;
                $this->errMsg = "input parameters type error";
                return false;
            }
            $v_str = $v['v'];
            if(!isset($v_str)){
                $this->errCode = 600;
                $this->errMsg = "input parameters format error";
                return false;
            }
            $stmt->bindValue($k+1, $v_str, $t_str);//bindParam第二个参数是引用传递
        }
        $result = $stmt->execute();

        if ($result === false){
            $this->errCode = 501;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',db:'.$this->db_name.',sql:'.$sql. ',errMsg:' . $stmt->errorInfo()[2];
            return false;
        }
        $res = $stmt->fetchAll();
        return $res;
    }


    /**
     * 执行select语句
     *  绑定带冒号的参数
     * @param  string sql    	sql语句
     * @param  array data   key是表字段名，value是值
     * @return bool 正确返回数据,错误返回false
     */
    public function getRowsC($sql, $data)
    {
        $this->clearERR();
        $sql = trim($sql);

        if(!$this->checkConnection()){
            return false;
        }
        $this->db->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
        $stmt = $this->db->prepare($sql);

        if ($stmt === false){
            $this->errCode = 502;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',db:'.$this->db_name.',sql:'.$sql. ',errCode:502';
            return false;
        }

        foreach ($data as $k => $v)
        {
            $n_str = ":" . $k;
            $v_str = $v;
            $stmt->bindValue($n_str, $v_str);
        }
        $result = $stmt->execute();

        if ($result === false){
            $this->errCode = 501;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',db:'.$this->db_name.',sql:'.$sql. ',errMsg:' . $stmt->errorInfo()[2];
            return false;
        }
        $res = $stmt->fetchAll();
        return $res;

    }


    /**
     * 执行select语句
     *  绑定带冒号的参数，带类型
     * @param  string sql    	sql语句
     * @param  array data   key是表字段名，value是json对象：{'t'=>'','v'=>''}
     * @return bool 正确返回数据,错误返回false
     */
    public function getRowsCT($sql, $data)
    {
        $this->clearERR();
        $sql = trim($sql);

        if(!$this->checkConnection()){
            return false;
        }
        $this->db->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
        $stmt = $this->db->prepare($sql);

        if ($stmt === false){
            $this->errCode = 502;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',db:'.$this->db_name.',sql:'.$sql. ',errCode:502';
            return false;
        }

        foreach ($data as $k => $v)
        {
            $n_str = ":" . $k;
            $t_str = $v['t'];
            if(!isset($t_str)){
                $this->errCode = 600;
                $this->errMsg = "input parameters format error";
                return false;
            }
            if(!in_array($t_str,$this->pdoDataType)){
                $this->errCode = 601;
                $this->errMsg = "input parameters type error";
                return false;
            }
            $v_str = $v['v'];
            if(!isset($v_str)){
                $this->errCode = 600;
                $this->errMsg = "input parameters format error";
                return false;
            }
            $stmt->bindValue($n_str, $v_str, $t_str);
        }
        $result = $stmt->execute();

        if ($result === false){
            $this->errCode = 501;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',db:'.$this->db_name.',sql:'.$sql. ',errMsg:' . $stmt->errorInfo()[2];
            return false;
        }
        $res = $stmt->fetchAll();
        return $res;
    }


    /**
     * 执行不带参数的sql语句(除select语句外的语句)
     *
     * @param  string sql   sql语句
     * @return bool 正确返回true 否则返回false
     */
    public function execSql($sql)
    {
        $this->clearERR();
        $sql = trim($sql);

        if(!$this->checkConnection()){
            return false;
        }
        $this->db->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
        $stmt = $this->db->prepare($sql);

        if ($stmt === false){
            $this->errCode = 502;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',db:'.$this->db_name.',sql:'.$sql. ',errCode:502';
            return false;
        }

        $result = $stmt->execute();
        if ($result === true) return true;

        $this->errCode = 501;
        $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',db:'.$this->db_name.',sql:'.$sql. ',errMsg:' . $stmt->errorInfo()[2];
        return false;
    }

    /**
     * 执行sql语句(除select语句外的语句)
     * 绑定带问号的参数
     *
     * @param  string sql   sql语句
     * @param  array data   顺序传入的数据，key是int类型
     * @return bool 正确返回true 否则返回false
     */
    public function execSqlQ($sql, $data)
    {
        $this->clearERR();
        $sql = trim($sql);

        if(!$this->checkConnection()){
            return false;
        }
        $this->db->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
        $stmt = $this->db->prepare($sql);

        if ($stmt === false){
            $this->errCode = 502;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',db:'.$this->db_name.',sql:'.$sql. ',errCode:502';
            return false;
        }

        foreach ($data as $k => $v)
        {
            if ( !is_numeric($k) ) {
                $this->errCode = 600;
                $this->errMsg = "input parameters format error";
                return false;
            }
            $v_str = $v;
            $stmt->bindValue($k+1, $v_str);
        }
        $result = $stmt->execute();

        if ($result === true) return true;

        $this->errCode = 501;
        $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',db:'.$this->db_name.',sql:'.$sql. ',errMsg:' . $stmt->errorInfo()[2];
        return false;
    }

    /**
     * 执行sql语句(除select语句外的语句)
     * 绑定带问号的参数，带类型
     *
     * @param  string sql   sql语句
     * @param  array data   顺序传入的数据，key是int类型,{'t'=>'','v'=>''},t是类型，v是值
     * @return bool 正确返回true 否则返回false
     */
    public function execSqlQT($sql, $data)
    {
        $this->clearERR();
        $sql = trim($sql);

        if(!$this->checkConnection()){
            return false;
        }
        $this->db->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
        $stmt = $this->db->prepare($sql);

        if ($stmt === false){
            $this->errCode = 502;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',db:'.$this->db_name.',sql:'.$sql. ',errCode:502';
            return false;
        }

        foreach ($data as $k => $v)
        {
            if ( !is_numeric($k) ) {
                $this->errCode = 600;
                $this->errMsg = "input parameters format error";
                return false;
            }
            $t_str = $v['t'];
            if(!isset($t_str)){
                $this->errCode = 600;
                $this->errMsg = "input parameters format error";
                return false;
            }
            if(!in_array($t_str,$this->pdoDataType)){
                $this->errCode = 601;
                $this->errMsg = "input parameters type error";
                return false;
            }
            $v_str = $v['v'];
            if(!isset($v_str)){
                $this->errCode = 600;
                $this->errMsg = "input parameters format error";
                return false;
            }
            $stmt->bindValue($k+1, $v_str,$t_str);
        }
        $result = $stmt->execute();

        if ($result === true) return true;
        $this->errCode = 501;
        $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',db:'.$this->db_name.',sql:'.$sql. ',errMsg:' . $stmt->errorInfo()[2];
        return false;
    }


    /**
     * 执行sql语句(除select语句外的语句)
     *  绑定带冒号的参数
     * @param  string sql    	sql语句
     * @param  array data   key是表字段名，value是值
     * @return bool 正确返回true 否则返回false
     */
    public function execSqlC($sql, $data)
    {
        $this->clearERR();
        $sql = trim($sql);

        if(!$this->checkConnection()){
            return false;
        }
        $this->db->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
        $stmt = $this->db->prepare($sql);

        if ($stmt === false){
            $this->errCode = 502;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',db:'.$this->db_name.',sql:'.$sql. ',errCode:502';
            return false;
        }

        foreach ($data as $k => $v)
        {
            $n_str = ":" . $k;
            $v_str = $v;
            $stmt->bindValue($n_str, $v_str);
        }
        $result = $stmt->execute();

        if ($result === true) return true;

        $this->errCode = 501;
        $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',db:'.$this->db_name.',sql:'.$sql. ',errMsg:' . $stmt->errorInfo()[2];
        return false;

    }


    /**
     * 执行sql语句(除select语句外的语句)
     *  绑定带冒号的参数，带类型
     * @param  string sql    	sql语句
     * @param  array data   key是表字段名，value是json对象：{'t'=>'','v'=>''}
     * @return bool 正确返回true 否则返回false
     */
    public function execSqlCT($sql, $data)
    {
        $this->clearERR();
        $sql = trim($sql);

        if(!$this->checkConnection()){
            return false;
        }
        $this->db->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
        $stmt = $this->db->prepare($sql);

        if ($stmt === false){
            $this->errCode = 502;
            $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',db:'.$this->db_name.',sql:'.$sql. ',errCode:502';
            return false;
        }

        foreach ($data as $k => $v)
        {
            $n_str = ":" . $k;
            $t_str = $v['t'];
            if(!isset($t_str)){
                $this->errCode = 600;
                $this->errMsg = "input parameters format error";
                return false;
            }
            if(!in_array($t_str,$this->pdoDataType)){
                $this->errCode = 601;
                $this->errMsg = "input parameters type error";
                return false;
            }
            $v_str = $v['v'];
            if(!isset($v_str)){
                $this->errCode = 600;
                $this->errMsg = "input parameters format error";
                return false;
            }
            $stmt->bindValue($n_str, $v_str, $t_str);
        }
        $result = $stmt->execute();

        if ($result === true) return true;

        $this->errCode = 501;
        $this->errMsg  = 'host:'.$this->host.',port:'.$this->port.',db:'.$this->db_name.',sql:'.$sql. ',errMsg:' . $stmt->errorInfo()[2];
        return false;

    }

}
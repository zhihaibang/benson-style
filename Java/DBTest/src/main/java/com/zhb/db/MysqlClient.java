package com.zhb.db;

import java.sql.DriverManager;
import java.sql.ResultSet;
import java.text.MessageFormat;

import org.apache.log4j.Logger;
import com.mysql.jdbc.Connection;
import com.mysql.jdbc.Statement;

public class MysqlClient {
	private static final Logger logger = Logger.getLogger(MysqlClient.class);
			
    private String ip = "192.168.144.119";
    private int port = 3306;
    private String user = "root";
    private String password = "mysql";
    
    private Connection conn;
    
    
    /**
     * Initial JMysql Parameters.
     * @param ip
     * @param port
     * @param user
     * @param password
     */
    public MysqlClient(String ip,int port,String user,String password){
    	this.ip = ip;
    	this.port = port;
    	this.user = user;
    	this.password = password;    	
    }
    
    
    public void getDbConnect(String database){
    	try { 
    	     Class.forName("com.mysql.jdbc.Driver");
    	     String url = "jdbc:mysql://"+ip+":"+port+"/"+database;
    	     conn = (Connection) DriverManager.getConnection(url, user, password);
    	         	     
    	     if(conn!=null && !conn.isClosed()) {
    	    	 logger.info(MessageFormat.format("mysql server connect sucessfully,ip:{0},port:{1},database:{2}", ip , port, database));
    	     }
    	} catch (Exception e){
    		e.printStackTrace();
    		logger.error(MessageFormat.format("fail to connect mysql server,ip:{0},port:{1},database:{2}", ip , port, database));
    		logger.error(e.getMessage(),e);
    	}
    }
    
    /**
     * query select语句
     * @param sql
     * @return
     */
    public ResultSet query(String sql){
    	ResultSet rs = null;
    	try { 
	    	Statement statement = (Statement) conn.createStatement();
	    	rs = statement.executeQuery(sql);
    	}catch (Exception e){
    		e.printStackTrace();
    		logger.error(MessageFormat.format("db query error,sql:{0}", sql));
    		logger.error(e.getMessage(),e);
    	}
    	return rs;
    }
    
    /**
     * insert,update,delete等语句
     * @param sql
     * @return
     */
    public boolean update(String sql){
    	int ret = 0;
    	try { 
	    	Statement statement = (Statement) conn.createStatement();
	    	ret = statement.executeUpdate(sql);
    	}catch (Exception e){
    		e.printStackTrace();
    		logger.error(MessageFormat.format("db update error,sql:{0}", sql));
    		logger.error(e.getMessage(),e);
    	}
    	if(ret > 0){
    		return true;
    	}
    	return false;
    }
    
    /**
     * close db
     */
    public void closeDB(){
    	try {
			conn.close();
    	}catch (Exception e){
    		e.printStackTrace();
    		logger.error("db close error");
    		logger.error(e.getMessage(),e);
    	}
    }
}

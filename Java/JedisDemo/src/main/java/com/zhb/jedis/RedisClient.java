package com.zhb.jedis;

import java.text.MessageFormat;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.log4j.Logger;

import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;
import redis.clients.jedis.JedisPoolConfig;
import redis.clients.jedis.exceptions.JedisConnectionException;

public class RedisClient {
	private static final Logger logger = Logger.getLogger(RedisClient.class);
	
    private JedisPool jedisPool = null;
    
    private String hostname_;
    private int port_;
    
 
    /**
     * 初始化连接池
     * @param hostname
     * @param port
     */
    public boolean initialPool(final String hostname, final int port)
    {
    	this.hostname_ = hostname;
    	this.port_ = port;
    	Jedis jedis = null;
    	try{
    		// 池基本配置 
            JedisPoolConfig config = new JedisPoolConfig();
            config.setMaxIdle(5);
            config.setTestOnBorrow(false);
            jedisPool = new JedisPool(config,hostname,port);
            jedis = jedisPool.getResource();
            logger.info(MessageFormat.format("redis server connect sucessfully,hostname:{0},port:{1}", hostname , port));
    	}catch(JedisConnectionException e){
    		logger.warn(MessageFormat.format("redis server connect fail,hostname:{0},port:{1}", hostname , port));
        	logger.error(e.getMessage(),e);
        	return false;
    	}finally{
    		jedisPool.returnResource(jedis);
    	}
        return true;
    }
    
    /**
     * ping redis节点
     * @return
     */
    public boolean ping(){
    	Jedis jedis = null;
    	if(jedisPool!=null && !jedisPool.isClosed()){
    		try{
    			jedis = jedisPool.getResource();
    			return true;
    		}catch(Exception e){
    			logger.warn(MessageFormat.format("redis server ping fail,hostname:{0},port:{1}", hostname_ , port_));
    			logger.error(e.getMessage(),e);
    	    	return initialPool(hostname_, port_);
    		}finally{
    			jedisPool.returnResource(jedis);
    		}
    	}
    	  	
    	logger.warn(MessageFormat.format("redis server ping fail,hostname:{0},port:{1}", hostname_ , port_));
    	return initialPool(hostname_, port_);
    }
    
    /**
     * set string
     * @param key
     * @param value
     */
    public void setString(String key,String value){
    	Jedis jedis = null;
        try {
          jedis = jedisPool.getResource();
          jedis.set(key, value);
          logger.debug("setString:"+key+","+value);
        } catch (Exception e) {
        	logger.error("failed to set string,"
        				+"redis ip:"+hostname_
        				+"port:"+port_);
        	logger.error(e.getMessage(),e);
        } finally {
        	jedisPool.returnResource(jedis);
        }
    }
    
    
    /**
     * get string
     * @param key
     * @param value
     */
    public String getString(String key){
    	Jedis jedis = null;
    	String value = null;
        try {
          jedis = jedisPool.getResource();
          value = jedis.get(key);
          if(value != null){
        	  logger.debug("getString:"+key+","+value);
          }
        } catch (Exception e) {
        	logger.error("failed to get string,"
        				+"redis ip:"+hostname_
        				+"port:"+port_);
        	logger.error(e.getMessage(),e);
        } finally {
        	jedisPool.returnResource(jedis);
        }
        return value;
    }
    
    /**
     * set HashMap
     * @param key
     * @param value
     */
    public void setHashMap(String key,Map<String,String> value){
    	Jedis jedis = null;
        try {
          jedis = jedisPool.getResource();
          jedis.hmset(key, value);
        } catch (Exception e) {
        	logger.error("failed to set HashMap,"
        				+"redis ip:"+hostname_
        				+"port:"+port_);
        	logger.error(e.getMessage(),e);
        } finally {
        	jedisPool.returnResource(jedis);
        }
    }
    
    /**
     * get AllHashMap
     * @param key
     * @return
     */
    public Map<String,String> getAllHashMap(String key){
    	Jedis jedis = null;
    	Map<String,String> value = null;
        try {
          jedis = jedisPool.getResource();
          value = jedis.hgetAll(key);
          if(value != null){
        	  logger.debug("getAllHashMap:"+key);
          }
        } catch (Exception e) {
        	logger.error("failed to get AllHashMap,"
        				+"redis ip:"+hostname_
        				+"port:"+port_);
        	logger.error(e.getMessage(),e);
        } finally {
        	jedisPool.returnResource(jedis);
        }
        return value;
    }
    
    /**
     * get HashMap
     * @param key
     * @param value_key
     * @return
     */
    public String getHashMap(String key,String value_key){
    	Jedis jedis = null;
    	String value = null;
        try {
          jedis = jedisPool.getResource();
          value = jedis.hget(key,value_key);
          if(value != null){
        	  logger.debug("getHashMap:"+key);
          }
        } catch (Exception e) {
        	logger.error("failed to get HashMap,"
        				+"redis ip:"+hostname_
        				+"port:"+port_);
        	logger.error(e.getMessage(),e);
        } finally {
        	jedisPool.returnResource(jedis);
        }
        return value;
    }
    
    /**
     * get MultiHashMap
     * @param key
     * @param value_key
     * @return
     */
    public List<String> getMultiHashMap(String key,String... value_key){
    	Jedis jedis = null;
    	List<String> value = null;
        try {
          jedis = jedisPool.getResource();
          value = jedis.hmget(key,value_key);
          if(value != null){
        	  logger.debug("getMultiHashMap:"+key);
          }
        } catch (Exception e) {
        	logger.error("failed to get multiHashMap,"
        				+"redis ip:"+hostname_
        				+"port:"+port_);
        	logger.error(e.getMessage(),e);
        } finally {
        	jedisPool.returnResource(jedis);
        }
        return value;
    }
    
    /**
     * get HashMapKeys
     * @param key
     * @return
     */
    public Set<String> getHashMapKeys(String key){
    	Jedis jedis = null;
    	Set<String> value = null;
        try {
          jedis = jedisPool.getResource();
          value = jedis.hkeys(key);
          if(value != null){
        	  logger.debug("getHashMapKeys:"+key);
          }
        } catch (Exception e) {
        	logger.error("failed to get HashMapKeys,"
        				+"redis ip:"+hostname_
        				+"port:"+port_);
        	logger.error(e.getMessage(),e);
        } finally {
        	jedisPool.returnResource(jedis);
        }
        return value;
    }
    
    /**
     * get HashMapValues
     * @param key
     * @return
     */
    public List<String> getHashMapValues(String key){
    	Jedis jedis = null;
    	List<String> value = null;
        try {
          jedis = jedisPool.getResource();
          value = jedis.hvals(key);
          if(value != null){
        	  logger.debug("getHashMapValues:"+key);
          }
        } catch (Exception e) {
        	logger.error("failed to get HashMapValues,"
        				+"redis ip:"+hostname_
        				+"port:"+port_);
        	logger.error(e.getMessage(),e);
        } finally {
        	jedisPool.returnResource(jedis);
        }
        return value;
    }
    
    /**
     * delete HashMapKey
     * @param key
     * @param value_key
     */
    public void deleteHashMapKey(String key,String value_key){
    	Jedis jedis = null;
    	try {
            jedis = jedisPool.getResource();
            jedis.hdel(key,value_key);
            logger.debug("deleteHashMapKey key="+key+",value_key="+value_key);
          } catch (Exception e) {
          	logger.error("failed to delete HashMapKey"
  	        			+",redis ip:"+hostname_
  	    				+",port:"+port_);
          	logger.error(e.getMessage(),e);
          } finally {
          	jedisPool.returnResource(jedis);
          }
    }
       
    /**
     * push到队列
     * @param key
     * @param value
     */   
    public void pushList(String key,String value){
    	Jedis jedis = null;
        try {
          jedis = jedisPool.getResource();
          jedis.rpush(key, value);
          logger.debug("pushList:"+key+","+value);
        } catch (Exception e) {
        	logger.error("failed to push to list,"
        				+"redis ip:"+hostname_
        				+"port:"+port_);
        	logger.error(e.getMessage(),e);
        } finally {
        	jedisPool.returnResource(jedis);
        }
    }
    
    /**
     * * pop队列
     * @param key
     */
    public String popList(String key){   	      	
    	Jedis jedis = null;
    	String value = null;
        try {
          jedis = jedisPool.getResource();
          value = jedis.lpop(key);
          if(value != null){
        	  logger.debug("popList:"+key+","+value);
          }
        } catch (Exception e) {
        	logger.error("failed to pop from list,"
	        			+"redis ip:"+hostname_
	    				+",port:"+port_);
        	logger.error(e.getMessage(),e);
        } finally {
        	jedisPool.returnResource(jedis);
        }
        return value;
    }
    
    /**
     * add to Set
     * @param key
     * @param value
     */
    public void addSet(String key,String value){
    	Jedis jedis = null;
        try {
          jedis = jedisPool.getResource();
          jedis.sadd(key, value);
          logger.debug("addSet"+key+","+value);
        } catch (Exception e) {
        	logger.error("failed to add to set,"
        				+"redis ip:"+hostname_
        				+"port:"+port_);
        	logger.error(e.getMessage(),e);
        } finally {
        	jedisPool.returnResource(jedis);
        }
    }
    
    /**
     * 获得set中所有元素
     * get SetMembers
     * @param key
     * @return
     */
    public Set<String> getSetMembers(String key){
    	Jedis jedis = null;
    	Set<String> value = null;
        try {
          jedis = jedisPool.getResource();
          value = jedis.smembers(key);
          if(value != null){
        	  logger.debug("getSetMembers:"+key);
          }
        } catch (Exception e) {
        	logger.error("failed to get SetMembers,"
        				+"redis ip:"+hostname_
        				+"port:"+port_);
        	logger.error(e.getMessage(),e);
        } finally {
        	jedisPool.returnResource(jedis);
        }
        return value;
    }
    
    /**
     * 删除set中某个元素
     * remove SetMember
     * @param key
     * @param value
     */
    public void removeSetMember(String key,String value){
    	Jedis jedis = null;
        try {
          jedis = jedisPool.getResource();
          jedis.srem(key,value);         
          logger.debug("removeSetMember:"+key+",value="+value);
        } catch (Exception e) {
        	logger.error("failed to remove SetMember,"
        				+"redis ip:"+hostname_
        				+"port:"+port_);
        	logger.error(e.getMessage(),e);
        } finally {
        	jedisPool.returnResource(jedis);
        }
    }
    
    /**
     * 删除key
     * @param key
     */
    public void deleteKey(String key){
    	Jedis jedis = null;
    	try {
            jedis = jedisPool.getResource();
            jedis.del(key);
            logger.debug("deleteKey key="+key);
          } catch (Exception e) {
          	logger.error("failed to delete key="+key
  	        			+",redis ip:"+hostname_
  	    				+",port:"+port_);
          	logger.error(e.getMessage(),e);
          } finally {
          	jedisPool.returnResource(jedis);
          }
    }
    
    
    
    
    public String getHostname(){
    	return this.hostname_;
    }
    
    public int getPort(){
    	return this.port_;
    }
    
}

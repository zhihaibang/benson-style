package com.zhb.jedis;

import org.apache.log4j.PropertyConfigurator;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args)
    {
    	PropertyConfigurator.configure("conf/log4j.properties");
		
		ApplicationContext context = new ClassPathXmlApplicationContext (new String[]{"conf/spring.xml"});
		
		Config config = (Config)context.getBean("Config");
		
		String ip = config.getIp();
		int port = config.getPort();
		
		RedisClient client = new RedisClient();
		client.initialPool(ip, port);
		
		client.setString("zhb", "value1");
		System.out.println(client.getString("zhb"));
		
		client.pushList("zhb1", "value1_list");
		System.out.println(client.popList("zhb1"));
		
    }
}

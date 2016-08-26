package com.zhb.db;

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
		String user = config.getUser();
		String password = config.getPassword();
		
    	DBDAO dao = new DBDAO(ip,port,user,password);
		dao.selectData();
		dao.Insert_t_login_version_analysis();
    }
}

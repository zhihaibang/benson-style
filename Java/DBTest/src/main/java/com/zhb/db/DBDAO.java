package com.zhb.db;

import java.sql.ResultSet;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

import org.apache.log4j.Logger;

public class DBDAO {
	private static final Logger logger = Logger.getLogger(DBDAO.class);
	
	private MysqlClient mysql_client;
	
	public DBDAO(String ip,int port,String user,String password){
		mysql_client = new MysqlClient(ip,port,user,password);
    }
	
	public void selectData() {
		try{
			mysql_client.getDbConnect("db_monitor_loginlog");
			String sql = "SELECT * FROM t_loginlog_20160304 where Fapp_id=100";
			ResultSet rs = mysql_client.query(sql);		
			while(rs.next()) {
				int appid = rs.getInt("Fapp_id");
				String os = rs.getString("Fos");
				String net =  rs.getString("Fnettype");
				System.out.println(appid + "\t" + os + "\t" + net);
			}
			rs.close();
		}catch (Exception e){
			e.printStackTrace();
			logger.error("selectData error");
		}
		mysql_client.closeDB();
	}
	
	public void Insert_t_login_version_analysis() {
		try{
			mysql_client.getDbConnect("db_monitor_loginlog");
			String sql = "";
			
			Date start = new Date();
			start.setYear(115);
			start.setMonth(7);
			start.setDate(31);
			SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
			
			ArrayList<String> type = new ArrayList<String>();
			type.add("jd");
			type.add("qq");
			type.add("wx");
			
			ArrayList<String> os = new ArrayList<String>();
			os.add("android");
			os.add("apple");
			os.add("ipad");
			os.add("m");
			os.add("all");
			
			for(int i=0;i<31;i++)
			{
				start.setDate(31-i);
				String dateStr = sdf.format(start);
				String typestr = type.get((int)(Math.random()*type.size()));
				String osstr = os.get((int)(Math.random()*os.size()));
				int total = (int)(Math.random()*100)+60;
				int success = (int)(Math.random()*60);
				double rate = ((double)success)/total;
				int appid = 100;
				if((31-i)%10==0){
					appid =110;
				}
				sql = "INSERT INTO t_login_version_analysis VALUES('"+dateStr+"',"+appid+",'"+typestr+"','"+osstr+"',"+total+","+success+","+rate+")";				
				mysql_client.update(sql);
				System.out.println(sql);
			}
		}catch (Exception e){
			e.printStackTrace();
			logger.error("Insert_t_login_version_analysis error");
		}
		mysql_client.closeDB();
	}
	
	
	
	
}

/*
 * =====================================================================================
 *
 *       Filename:  MysqlClient.cpp
 *
 *        Version:  1.0
 *        Created:  2016-08-22
 *
 *        Author:  zhihaibang
 *
 * =====================================================================================
 */
 
#include "MysqlClient.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <pthread.h>
#include "mysql/mysql.h"
#include "mysql/mysqld_error.h"
using namespace std;

MysqlClient::MysqlClient():is_connected(false), affected_rows(0), result(NULL){}

MysqlClient::~MysqlClient()
{
    if (result) {
		mysql_free_result(result);
	}
	CloseConnect();
    pthread_mutex_destroy(&lock);
}

int MysqlClient::Initialize(DbConfig db_config)
{
	pthread_mutex_init(&lock, NULL);
	
	config.db_host = db_config.db_host;
	config.db_port = db_config.db_port;
	config.db_account = db_config.db_account;
	config.db_password = db_config.db_password;
	config.db_name = db_config.db_name;
	config.charset = db_config.charset;
	
    if (config.db_host.empty() || config.db_account.empty() || config.db_password.empty() || config.db_name.empty() || config.db_port < 0)
	{
		sprintf(error_msg,"invalid mysql config");
		return -1;
	}
		
	return Connect();
}


int MysqlClient::Connect()
{	
	pthread_mutex_lock(&lock);
	if (is_connected)
    {
		pthread_mutex_unlock(&lock);
		return 0;
	}
		
	if (mysql_init(&connection) == NULL)
    {
        sprintf(error_msg,"mysql initialize fail, error: %s",mysql_error(&connection));
		pthread_mutex_unlock(&lock);
		return -1;
	}
			
    int ret = -1;
	if (mysql_real_connect(&connection,
                            config.db_host.c_str(),
                            config.db_account.c_str(),
                            config.db_password.c_str(),
                            config.db_name.c_str(),
                            config.db_port, NULL, 0))
    {
		is_connected = true;
		if(config.charset.size() == 0){
			ret = 0;
		}
		else if (mysql_set_character_set(&connection, config.charset.c_str()) == 0)
		{
		    ret = 0;
    	}
    	else
        {
            sprintf(error_msg,"mysql set character fail, error:%s",mysql_error(&connection));
            ret = -1;
        }
	}
	else
	{
	    sprintf(error_msg,"mysql connect failed, host(%s),port(%d),database(%s), error:%s",
                config.db_host.c_str(),
                config.db_port,
                config.db_name.c_str(),
                mysql_error(&connection));
        ret = -1;
	}
	
	pthread_mutex_unlock(&lock);
	
	return ret;
}

int MysqlClient::Reconnect()
{
    CloseConnect();
	return Connect();
}

int MysqlClient::CloseConnect()
{
	pthread_mutex_lock(&lock);
	if (is_connected && &connection)
	{
		mysql_close(&connection);
		is_connected = false;
	}
	pthread_mutex_unlock(&lock);
	return 0;
}

int MysqlClient::SetDb(const string &name) {
	if (mysql_ping(&connection) != 0)
    {
        sprintf(error_msg,"mysql connection goes down, error: %s. try reconnect",mysql_error(&connection));
		if (Reconnect() != 0) {
		    sprintf(error_msg,"reconnect failed. error: %s",mysql_error(&connection));
			return -1;
		}
	}

	int ret = mysql_select_db(&connection, name.c_str());
	if (ret == 0) {
		config.db_name = name;
		return 0;
	} else {
	    sprintf(error_msg,"set db failed. error: %s",mysql_error(&connection));
		return -1;
	}
}

int MysqlClient::ExecuteSql(const string &sql)
{
	if (mysql_ping(&connection) != 0) {
		sprintf(error_msg,"mysql connection is down, error: %s. try reconnect",mysql_error(&connection));
		if (Reconnect() != 0) {
		    sprintf(error_msg,"reconnect failed. error: %s",mysql_error(&connection));
			return -1;
		}
	}
	
	pthread_mutex_lock(&lock);
		
	if (mysql_query(&connection, sql.c_str()) != 0) {
        sprintf(error_msg,"execute sql failed. error: %s",mysql_error(&connection));
		pthread_mutex_unlock(&lock);
		return -1;
	}
	
    result = mysql_store_result(&connection);
	if (result == NULL && mysql_errno(&connection) != 0) {
		sprintf(error_msg,"store result failed. error: %s",mysql_error(&connection));
		pthread_mutex_unlock(&lock);
		return -1;
	}
	
	pthread_mutex_unlock(&lock);
	return 0;
}

int MysqlClient::SetDbCharacter()
{
	string sql = "set names utf8";
	return ExecuteSql(sql);
}

int MysqlClient::GetResult(ResultData &data)
{
	if (result == NULL) {
		return 0;
	}
	
	int feild_count = mysql_field_count(&connection);
	MYSQL_ROW sqlrow;
	while((sqlrow = mysql_fetch_row(result))) {
		unsigned count = 0;		
		vector<string> row;
		while (count < feild_count) {
			row.push_back(string(sqlrow[count]));
			count++;
		}
		data.push_back(row);
	}
	mysql_free_result(result);//只是释放空间，并不把result置为null
	result = NULL;//置为null，以后释放时需判断
	return 0;
}

long long MysqlClient::GetAffectedRows()
{
	return mysql_affected_rows(&connection);
}

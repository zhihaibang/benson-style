#ifndef MYSQLCLIENT_H
#define MYSQLCLIENT_H

#include <string>
#include <vector>
#include <pthread.h>
#include "mysql/mysql.h"
#include "mysql/mysqld_error.h"
using namespace std;

typedef vector< vector<string> > ResultData;

struct DbConfig{
    string db_host;
    int db_port;
    string db_account;
    string db_password;
    string db_name;
    string charset;
};

class MysqlClient
{
    public:
        MysqlClient();
        virtual ~MysqlClient();

        int Initialize(DbConfig db_config);
        int Connect();
        int Reconnect();
        int SetDb(const string &name);
        int CloseConnect();
        int ExecuteSql(const string &sql);
        int GetResult(ResultData &data);
        long long GetAffectedRows();
		int SetDbCharacter();
        char* GetErrMsg(){return error_msg;}

    private:
        pthread_mutex_t lock;
        bool		is_connected;
        int 		affected_rows;
        MYSQL       connection;
        MYSQL_RES	*result;
        DbConfig	config;
        char        error_msg[1024];
};

#endif // MYSQLCLIENT_H

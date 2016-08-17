#include <iostream>
#include <string>
#include <stdint.h>
#include <vector>
#include "MysqlClient.h"
using namespace std;

int main()
{
    string db_host = "192.168.144.119";
    int db_port = 3306;
    string db_account = "root";
    string db_password = "mysql";
    string db_name = "test";
    string charset = "utf8";

    struct DbConfig db_config;
    db_config.db_host = db_host;
    db_config.db_port = db_port;
    db_config.db_account = db_account;
    db_config.db_password = db_password;
    db_config.db_name = db_name;
    db_config.charset = charset;

    MysqlClient* mysql = new MysqlClient();
    int ret = mysql->Initialize(db_config);
    if(ret != 0)
    {
        cout<<mysql->GetErrMsg()<<endl;
        return -1;
    }

    string sql = "insert into Persons(Id_P,LastName,FirstName,Address,City) values (6,'王','小明','长安街','北京')";
    ret = mysql->ExecuteSql(sql);
    if(ret != 0)
    {
        cout<<mysql->GetErrMsg()<<endl;
        return -1;
    }

    ret = mysql->GetAffectedRows();
    if(ret <= 0)
    {
        cout<<"insert fail"<<endl;
    }
	
	ret = mysql->SetDbCharacter();
	if(ret != 0)
    {
        cout<<mysql->GetErrMsg()<<endl;
        return -1;
    }

    sql = "select * from Persons";
    ret = mysql->ExecuteSql(sql);
    if(ret != 0)
    {
        cout<<mysql->GetErrMsg()<<endl;
        return -1;
    }

    ResultData result;
    mysql->GetResult(result);
		
    ResultData::iterator it = result.begin();
    while(it != result.end())
    {
        vector<string> row = *it;
        vector<string>::iterator it_row = row.begin();
        while(it_row != row.end())
        {
            cout<<*it_row<<",";
			++it_row;
        }
        cout<<endl;
		++it;
    }
    return 0;
}

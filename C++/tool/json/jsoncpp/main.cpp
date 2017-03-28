#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "json/json.h"

using namespace std;

int genjson(Json::Value &root);
int readjson(string &upload_id,int &code);

int main()
{
	Json::Value root;
	genjson(root);

	Json::FastWriter fast_writer;
	string data_str = fast_writer.write(root);

	cout<<data_str<<endl;
	
	string upload_id;
	int code;
	readjson(upload_id,code);
	cout<<"upload_id:"<<upload_id<<"\ncode:"<<code<<endl;
    return 0;
}

int genjson(Json::Value &root)
{
	long long status = 1232;
	root["status"] = status;
	
	string ad = "0.45";
	double ads = atof(ad.c_str());
	cout<<ads<<endl;
	root["double"] = ads;
	
    root["msg"] = "";

    Json::Value data;
    data["email"] = "zhihaibang1";
    data["erp"] = "zhihaibang";

    Json::Value model_peer;
    model_peer["modelId"] = 23;
    model_peer["modeName"] = "测试mod";

    Json::Value bus_array;
    Json::Value bus_peer1;
    bus_peer1["busId"] = "112";
    bus_peer1["busName"] = "我的家园";
    bus_array.append(bus_peer1);

    Json::Value bus_peer2;
    bus_peer2["busId"] = "213";
    bus_peer2["busName"] = "你的家园";
    bus_array.append(bus_peer2);

    model_peer["businesses"] = bus_array;

    Json::Value model_array;
    model_array.append(model_peer);
	data["models"] = model_array;
	
	Json::Value nums;
	nums[0] = 1;
	nums[1] = 2;    
	data["nums"] = nums;
	
    root["data"] = data;

    return 0;
}

int readjson(string &upload_id,int &code)
{
	const char* str = "{\"uploadid\": \"UP000000\",\"code\": 100,\"msg\": \"\",\"files\": \"\"}";  
	
	Json::Reader reader(Json::Features::strictMode());
	Json::Value root;
	
	try//加try、catch防止解析错误出现coredump
	{ 
		if (reader.parse(str, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
		{  
			upload_id = root["uploadid"].asString();  // 访问节点，upload_id = "UP000000"  
			code = root["code"].asInt();    // 访问节点，code = 100 
		}
	}
	catch(...)
	{
		
	}
	return 0;
}

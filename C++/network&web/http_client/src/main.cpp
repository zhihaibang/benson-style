//============================================================================
// Name        : main.cpp
// Author      : zhb
// Version     : 1.0
// Description : http curl study
//============================================================================

#include <stdio.h>
#include <string>
#include "http_client.h"

using namespace std;

int main(int argc, const char *argv[])
{
	HttpClient *http_client = new HttpClient();
	string url = "http://apis.baidu.com/apistore/mobilenumber/mobilenumber";
	string response;
	int ret = http_client->Get(url,response);
	if(ret == CURLE_OK)
	{
		printf("get http success.\n\n%s\n",response.c_str());
	}
	else
	{
		printf("get http fail,ret=%d\n",ret);
	}

	response = "";
	url = "http://192.168.144.119:8081/json.php?biz=configcenter&mod=ResourcePool&act=UpdateServiceIpPorts";
	string params = "Fservice_name=Access&Fip_port_insert:192.168.120.123:14000&Fip_port_delete=";
	ret = http_client->Post(url,params,response);
	if(ret == CURLE_OK)
	{
		printf("\npost http success.\n\n%s\n",response.c_str());
	}
	else
	{
		printf("\npost http fail,ret=%d\n",ret);
	}
	
	delete http_client;
	return 0;
}

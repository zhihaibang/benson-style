#include <iostream>
#include <string>
#include "json/json.h"

using namespace std;

int main()
{
    Json::Value root;
    root["status"] = 1;
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
    root["data"] = data;

    Json::FastWriter fast_writer;
	string data_str = fast_writer.write(root);

	cout<<data_str<<endl;
    return 0;
}

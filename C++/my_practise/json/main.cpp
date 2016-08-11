#include <iostream>
#include <string>
#include "json/json.h"

using namespace std;

int main()
{
    Json::Value root;
	root["OperateTime"] = "zhb";
	root["timeStamp"] = "133242423";

    Json::FastWriter fast_writer;
	string data = fast_writer.write(root);

	cout<<data<<endl;
    return 0;
}

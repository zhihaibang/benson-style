#include <stdio.h>
#include <string>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

using namespace std;


void readTest()
{
    char json[] = "{\"carriorName\":\"日本电信\",\"deviceName\":\"iPhone5,2\",\"osName\":\"android\",\"osVersion\":\"8.1\",\"appId\":\"com.mmcshadow.doudizhu\",\"versionName\":\"1.0\",\"versionCode\":\"1.0\",\"deviceId\":\"11111111\"}";
    rapidjson::Document d;
    d.Parse<0>(json);
    printf("%s\n", d["carriorName"].GetString());
}

void writeTest()
{
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

    rapidjson::Value array(rapidjson::kArrayType);

    for (int i = 0; i < 10; i++)
    {
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("id", 1, allocator);
        object.AddMember("name", "test", allocator);
        object.AddMember("version", 1.01, allocator);
        object.AddMember("vip", true, allocator);

        //object.SetInt(i);
        array.PushBack(object, allocator);
    }

    document.AddMember("title", "PLAYER INFO", allocator);
    document.AddMember("players", array, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    string reststring = buffer.GetString();
    cout << reststring << endl;
}


void writeapi()
{
    rapidjson::Document root;
    root.SetObject();
    rapidjson::Document::AllocatorType& allocator = root.GetAllocator();

    root.AddMember("status", 1 , allocator);
    root.AddMember("msg", "" , allocator);

    rapidjson::Value data(rapidjson::kObjectType);//data

    string erp = "zhb1";
    data.AddMember("email", "zhb" , allocator);
    data.AddMember("erp", erp.c_str() , allocator);//类型不支持string类型

    rapidjson::Value modles(rapidjson::kArrayType);//models数组
    rapidjson::Value model(rapidjson::kObjectType);
    model.AddMember("modelId", 1002 , allocator);
    model.AddMember("modelName", "验签" , allocator);

    rapidjson::Value businesses(rapidjson::kArrayType);//businesses数组
    for(int i=0;i<2;i++)
    {
        rapidjson::Value business(rapidjson::kObjectType);
        business.AddMember("busId", i, allocator);
        business.AddMember("busName", "我的祖国", allocator);
        businesses.PushBack(business, allocator);
    }

    model.AddMember("businesses", businesses , allocator);
    modles.PushBack(model, allocator);

    data.AddMember("modles", modles , allocator);

    root.AddMember("data", data , allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    root.Accept(writer);
    string reststring = buffer.GetString();
    cout << reststring << endl;
}

int main(int argc, char const *argv[])
{
    writeapi();
    return 0;
}

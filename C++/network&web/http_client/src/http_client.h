/*
 * curl.h
 *
 *  Created on: 2016-12-23
 *  Author: zhb
 */
#ifndef HTTPCLIENT_H_
#define HTTPCLIENT_H_

#include <iostream>
#include <string>
#include "curl/curl.h"

using namespace std;


class HttpClient{
	public:

		/**
		* @brief HTTP GET请求
		* @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
		* @param strResponse 输出参数,返回的内容
		* @return 返回是否Post成功
		*/
		int Get(const string& strUrl, string& strResponse, int timeout=3);
		/**
		* @brief HTTP POST请求
		* @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
		* @param strPost 输入参数,使用如下格式para1=val1&para2=val2&…
		* @param strResponse 输出参数,返回的内容
		* @return 返回是否Post成功
		*/
		int Post(const string& strUrl, const string& strPost, string& strResponse, int timeout=3);
		
};

#endif /* HTTPCLIENT_H_ */

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <string>
#include <sstream>
#include <fcgi_stdio.h>
#include "webapp/webapplib.h"

using namespace webapp;
using namespace std;

int main()
{
	while (FCGI_Accept() >= 0)
    {
		webapp::g_out = "";
		//输出header
		webapp::http_head();
		
		Cgi cgi;
		
		string username = cgi["username"];
		string email = cgi["email"];
		string password = cgi["password"];
		
		Cookie cookie;
		
		DateTime now;
		DateTime expires = now + ( TIME_ONE_DAY*3 ); // Cookie有效期为三天
		cookie.set_cookie("usercookie", username, expires.gmt_datetime());
		
		string usercookie = cookie["usercookie"];
		
		stringstream ss;
		ss  <<"username: "<<username<<"\n"
			<<"email: "<<email<<"\n"
			<<"password: "<<password<<"\n"
			<<"usercookie: "<<usercookie<<"\n";
		
		webapp::g_out += ss.str();
					       		
		FCGI_printf("%s",webapp::g_out.c_str());
    }
    return 0;
}
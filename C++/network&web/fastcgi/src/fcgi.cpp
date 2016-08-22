#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fcgi_stdio.h>

using namespace std;
int main()
{
    /* Initialization Code */
    int count = 0;
    /* Start of response loop */
    while (FCGI_Accept() >= 0)
    {
    	char* server_name = getenv("SERVER_NAME");
    	char* query_string = getenv("QUERY_STRING");

        //* body of response loop /*/
        FCGI_printf("Content-type: text/html\r\n\r\n"
                "FastCGI Hello,ZHB! (C, fcgi_stdio library)"
                "\nRequest number %d running on host %s "
                "Process ID: %d\n, query string: %s",
                ++count,
                getenv("SERVER_NAME"), getpid(), query_string);
    }
    /* End of response loop */
    return 0;
}
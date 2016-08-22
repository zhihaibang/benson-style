#include "tinyxml.h"   
#include "tinystr.h"     
#include <iostream>     
#include <stdint.h>
#include <sstream>
#include <string.h>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
using namespace std;   

#define ETC_XML_DIR  "./etc/"

int GetPort(uint16_t &port);
int GetPortPeer(string file_name, uint16_t &port);
int VisitXML(TiXmlDocument *myDocument, uint16_t &port);


int main()
{  
	uint16_t port = 0;
    int ret = GetPort(port);
    if(ret == 0)
    {
    	cout<<"get port success"<<endl;
    }else{
    	cout<<"get port fail"<<endl;
    }
    cout<<"port:"<<port<<endl;
    return 0;
}

int GetPort(uint16_t &port)
{
	struct dirent *pDirEntry = NULL;
    DIR *pDir = NULL;
    if((pDir = opendir(ETC_XML_DIR)) == NULL)
    {
       return -1;
    }
    while(pDirEntry = readdir(pDir))
    {  
        if (strcmp(pDirEntry->d_name, ".") == 0 || strcmp(pDirEntry->d_name, "..") == 0)
        {
            continue;
        }

        stringstream ss;
        ss<<ETC_XML_DIR<<pDirEntry->d_name;

        struct stat buf; 
        stat(ss.str().c_str(), &buf); 
        if(!S_IFREG & buf.st_mode)
        { 
            continue;
        }

        string file_name = pDirEntry->d_name;
        string::size_type pos = file_name.find('.');
        if(pos == string::npos)
        {   
            continue;
        }

        string extension = file_name.substr(pos + 1);
        if(extension == "xml")
        {
        	int ret = GetPortPeer(ss.str(),port);
	    	if(ret == 0)
		    {
		        return 0;
		    }
        }   	
    }
    closedir(pDir);
    return -1;
}

int GetPortPeer(string file_name,uint16_t &port)
{
    TiXmlDocument *myDocument = new TiXmlDocument(file_name.c_str());
    bool flag = myDocument->LoadFile();
    if(!flag)
    {
        cout<<"cannot find xml file:"<<file_name<<endl;
        return -1;
    } 
    uint16_t t_port = 0;
    int ret = VisitXML(myDocument, t_port);
    if(0 != ret)
    {
        cout<<"xml parse error"<<endl;
        return -1;
    }
    port = t_port;
    return 0;
}


int VisitXML(TiXmlDocument *myDocument, uint16_t &port)
{  
    if (!myDocument) return -1;
    TiXmlElement *rootElement = myDocument->RootElement();
    if(strcmp(rootElement->Value(),"proxy") != 0)
    {
    	return -1;
    }
    TiXmlElement* aChild = rootElement->FirstChildElement();
    if(strcmp(aChild->Value(),"acceptor") != 0)
    {
    	return -1;
    }
    TiXmlElement* pChild = aChild->FirstChildElement();
    if(strcmp(pChild->Value(),"entry") != 0)
    {
    	return -1;
    }
    TiXmlAttribute *entryAttribute = pChild->FirstAttribute();
    while(entryAttribute != 0)
    {
        if(strcmp(entryAttribute->Name(),"port") == 0)
        {
            port = atoi(entryAttribute->Value());
            return 0;
        }
        entryAttribute = entryAttribute->Next();
    }
    return -1;
}

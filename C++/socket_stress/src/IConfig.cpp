#include "IConfig.h"
#include <stdio.h>
#include <iostream>
using namespace std;

void IConfig::free()
{
	delete this;
}

IConfig::IConfig() : m_iLoaded(0), m_nullRet("")
{	
}

int IConfig::loaded()
{
	return m_iLoaded;
}

IConfig * IConfig::instance()
{
	static IConfig * _ins  = 0;

	if(0 == _ins)
	{
		_ins = new IConfig;
	}
	return _ins;
}
/*************************************************
@Brief:         加载配置文件
@Param:         input,const string&,文件路径
@Return:        int，=0,加载成功，<0,,加载失败，返回错误
@Others:        svr初始化时候加载，失败svr不能启动
*************************************************/


int IConfig::loadConfigFile(const  char * s_cnf_file)
{
	if(NULL == s_cnf_file)
	{
		cout << "no config file" << endl;
		return -1;	
	}
	
	FILE * fp = 0;

	fp = fopen(s_cnf_file, "rb");
	if (NULL == fp)
	{
		cout << "file is not exist[" << s_cnf_file << "]" <<endl;
		return -1;
	}
	char szLine[MAX_CONF_ITEM_LENTH];
	char * pszRet = 0;
	string cSectionName;
	map<string,string> cSection;

	while (!feof(fp))
	{
		pszRet = fgets(szLine,MAX_CONF_ITEM_LENTH,fp);
		if (NULL == pszRet)
		{
			if (!cSectionName.empty())
			{
				m_contener[cSectionName] = cSection;				
			}			
			fclose(fp);
			m_iLoaded = 1;
			return 0;			
		}

		//去掉空格，\t\n\r字符
		int iNum = 0,i;
		for (i = 0 ; *(szLine + i); i++)
		{
			if (*(szLine + i) == ' ' || *(szLine + i) == '\t' || *(szLine + i) == '\r' || *(szLine + i) == '\n')
			{
				iNum++;
				continue;
			}
			if(*(szLine + i) == ';')
			{				
				*(szLine + i - iNum) = 0;	
				break;
			}
			*(szLine + i - iNum) = *(szLine + i);
		}
		*(szLine + i - iNum) = 0;
		// 跳过空白行
		if (i == iNum)
		{
			continue;
		}
		
		//新节开始
		if (szLine[0]=='[' && szLine[i-iNum-1] == ']')
		{
			if (!cSectionName.empty())
			{
				m_contener[cSectionName] = cSection;
			}		

			szLine[i-iNum-1] = 0;
			cSectionName = &szLine[1];
			cSection.clear();
		}
		else    //当前节中的一个配置项
		{
			char *pszName = strchr(szLine, '=');
			if (NULL == pszName)
			{
				cSection[szLine] = "";
			}
			else
			{
				*pszName = 0;
				pszName++;
				cSection[szLine] = pszName;
			}
			
		}
		
	}	
	if (!cSectionName.empty())
	{
		m_contener[cSectionName] = cSection;
	}
	m_iLoaded = 1;
	fclose(fp);
    return 0;
}

/*************************************************
@Brief:         获取配置文件int字段（外部使用，直接返回结构体数据）
@Param:         input,const string&,区域
@Param:         input,const string&,配置项名称
@Return:        int，=0,获取成功，<0,, 获取失败，返回错误
@Others:        svr运行时候获取，如果不能获取，证明配置有误
*************************************************/
int IConfig::getConfigInt(const string& s_section, const string& s_item)
{
    CONFIG_CONTENER::iterator iter = m_contener.find(s_section);

    if(iter == m_contener.end())
    {
    	 return -1;
    }

    map<string,string>::iterator nest_iter =  iter->second.find(s_item);
    if(iter->second.end() == nest_iter)
    	{
    		return -1;
    	}
    return  atoi( nest_iter->second.c_str());
}

/*************************************************
@Brief:         获取配置文件string字段（外部使用，直接返回结构体数据）
@Param:         input,const string&,区域
@Param:         input,const string&,配置项名称
@Return:        int，=0,获取成功，<0,, 获取失败，返回错误
@Others:        svr运行时候获取，如果不能获取，证明配置有误
*************************************************/
const char * IConfig::getConfigStr(const string& s_section, const string& s_item)
{
   CONFIG_CONTENER::iterator iter = m_contener.find(s_section);

    if(iter == m_contener.end())
    {
    	 return NULL;
    }

     map<string,string>::iterator nest_iter =  iter->second.find(s_item);
    if(iter->second.end() == nest_iter)
    	{
    		return NULL;
    	}
    return   nest_iter->second.c_str();
}

const string & IConfig::getCfgStr(const string& s_section, const string& s_item)
{
	CONFIG_CONTENER::iterator iter = m_contener.find(s_section);

    if(iter == m_contener.end())
    {
    	 return m_nullRet;
    }

     map<string,string>::iterator nest_iter =  iter->second.find(s_item);
    if(iter->second.end() == nest_iter)
    	{
    		return m_nullRet;
    	}
    return   nest_iter->second;
}


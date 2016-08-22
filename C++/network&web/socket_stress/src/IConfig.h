#ifndef _I_CONFIG_H_
#define _I_CONFIG_H_
#include <map>
#include <string>
#include <string.h>
#include <stdlib.h>

#define  MAX_CONF_ITEM_LENTH  2048

using std::string;
using std::map;

typedef map<string , map < string ,string> > CONFIG_CONTENER;

class IConfig
{
public:	
	static IConfig* instance();
	int loadConfigFile(const char * s_cnf_file);
	const char * getConfigStr(const string& s_section, const string& s_item);
	const string & getCfgStr(const string& s_section, const string& s_item);
	int getConfigInt(const string& s_section, const string& s_item);

	int loaded();

	void free();
protected:
	IConfig();
	IConfig(const IConfig& objLog);
	IConfig& operator = (const IConfig& objLog);

private:
	 CONFIG_CONTENER m_contener;
	 int m_iLoaded;
	const string m_nullRet;
};
#endif


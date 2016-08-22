#ifndef _I_LOG_H_
#define _I_LOG_H_

#include "Log.h"

#define MAX_LOG_LENGTH (1024*5)

#define LOG_LEV_ERROR      0
#define LOG_LEV_INFO       1
#define LOG_LEV_WARNING    2
#define LOG_LEV_DEBUG      3
#define LOG_LEV_TRACE      4

#define LOG_TYPE_NOR       ""           ///< Normal roll log.
#define LOG_TYPE_DAY       ".%Y%m%d"    ///< Day roll log.
#define LOG_TYPE_MON       ".%Y%m"      ///< Month roll log.

// log time format.
#define LOG_TFORMAT_0      "[%Y-%m-%d %H:%M:%S] "
#define LOG_TFORMAT_1      "%Y-%m-%d %H:%M:%S "
#define LOG_TFORMAT_2      "%Y%m%d%H%M%S "
#define LOG_TFORMAT_3      ""
#define LOG_TFORMAT_4      "%Y-%m-%d %H:%M:%S\t"


class IFlowLog
{
public:
	IFlowLog():m_iOffSet(0){};

	static IFlowLog * instance();
	//写日志，立即写入文件
	//int WriteLog(const char * s_info, const char *szFile = NULL, int iLine = 0, int iLogLevel = VIPSHOP_LOG_LEV_ERROR);
	int writeLog(int iLevel, const char    *pszFormat, ...);

  // add by baronyuan
  int writeLog(int iLevel, int lineno, const char *file, const char *func, const char * pszFormat,...);

	//设置日志到缓冲区，调用write时从缓冲区写入文件,可以多次重复调用追加日志
	int appendLog(const char    *pszFormat, ...);
	//将缓冲区中的日志写入到文件中
	int write(int iLogLevel = LOG_LEV_ERROR);

	//设置日志相关参数
	int setLogPara(const char *as_file = "../log/flow.log", int  ai_level  = LOG_LEV_ERROR, const char *as_tformat = LOG_TFORMAT_0,  const char * as_logtype = LOG_TYPE_DAY, int ai_maxfilelength = 1024*1024, int ai_maxfile =5 );
	void free();
	void clear(){m_iOffSet = 0;};
	~IFlowLog(){};
protected:
private:
	IFlowLog(const IFlowLog& objLog);
	IFlowLog& operator = (const IFlowLog& objLog);

	char m_szLog[MAX_LOG_LENGTH];
	clib_log m_Logger;
	int  m_iOffSet;
};



class ILog
{
public:
	ILog():m_iOffSet(0){};

	static ILog * instance();

	//设置日志相关参数
	int setLogPara(const char *as_file = "../log/error.log", int  ai_level  = LOG_LEV_ERROR, const char *as_tformat = LOG_TFORMAT_0,  const char * as_logtype = LOG_TYPE_DAY, int ai_maxfilelength = 1024*1024, int ai_maxfile =5 );

	//写日志，立即写入文件
	int writeLog(int iLevel, const char    *pszFormat, ...);

    int writeLog(int iLevel, int lineno, const char *file, const char *func, const char * pszFormat,...);


	//设置日志到缓冲区，调用write时从缓冲区写入文件,可以多次重复调用追加日志
	int appendLog(const char    *pszFormat, ...);
	const char * getLog();
	void clear(){m_iOffSet = 0;};

	inline int error(){return write(LOG_LEV_ERROR);};
	inline int info(){return write(LOG_LEV_INFO);};
	inline int warning(){return write(LOG_LEV_WARNING);};
	inline int debug(){return write(LOG_LEV_DEBUG);};
	inline int trace(){return write(LOG_LEV_TRACE);};

	void free();
	~ILog(){};
protected:
private:
	ILog(const ILog& objLog);
	ILog& operator = (const ILog& objLog);

	//将缓冲区中的日志写入到文件中
	int write(int iLogLevel);

	char m_szLog[MAX_LOG_LENGTH];
	clib_log m_Logger;
	int  m_iOffSet;
};


#endif


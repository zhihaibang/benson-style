#include "ILog.h"


void IFlowLog::free()
{
	delete this;
}

IFlowLog * IFlowLog::instance()
{
	static IFlowLog * _ins = 0;
	if(0 == _ins)
	{
		_ins = new IFlowLog;
		_ins->setLogPara();
	}
	return _ins;
}



int IFlowLog::writeLog(int iLevel, const char * pszFormat,...)
{
	int     i_ret = 0;
	if (iLevel > m_Logger.get_level())
	{
		return 0;
	}

   	va_list ap;

    if ( NULL == pszFormat || strlen(pszFormat) <= 0 ) {
      	  return(-1);
  	  }

  	 va_start( ap, pszFormat );
   	 i_ret = m_Logger.apwrite( iLevel, pszFormat, ap );
    	va_end( ap );
   	 return( i_ret );
}


int IFlowLog::writeLog(int iLevel, int lineno, const char *file, const char *func, const char * pszFormat,...)
{
	int     i_ret = 0;
	if (iLevel > m_Logger.get_level())
	{
		return 0;
	}
   	va_list ap;

    	if ( NULL == pszFormat || strlen(pszFormat) <= 0 ) {
      	  return(-1);
  	  }

  	 va_start( ap, pszFormat );
   	 i_ret = m_Logger.apwrite( iLevel, lineno, file, func, pszFormat, ap );
    	va_end( ap );
   	 return( i_ret );
}


int IFlowLog::appendLog(const char    *pszFormat, ...)
{
	 int     i_ret = 0;
   	 va_list ap;

   	 if ( NULL == pszFormat || strlen(pszFormat) <= 0 ) {
       	 return(-1);
  	  }

   	 va_start( ap, pszFormat );
  	 i_ret = vsnprintf( m_szLog + m_iOffSet , MAX_LOG_LENGTH, pszFormat, ap );
   	 va_end( ap );
	 m_iOffSet += i_ret;
   	 return( i_ret );
}

int IFlowLog::write(int iLogLevel)
{
	if(0 == m_iOffSet) return 0;
	m_iOffSet = 0;
	if (iLogLevel > m_Logger.get_level())
	{
		return 0;
	}
	return m_Logger.write(m_szLog, iLogLevel);
}


int IFlowLog::setLogPara(const char *as_file, int  ai_level, const char *as_tformat,  const char * as_logtype , int ai_maxfilelength, int ai_maxfile )
{
	int iRet;
	memset(m_szLog, 0, sizeof(m_szLog));
	iRet = m_Logger.set_file(as_file);
	if( 0 != iRet)    return iRet;

	iRet = m_Logger.set_timeformat(as_tformat);
	if( 0 != iRet)    return iRet;

	iRet = m_Logger.set_level(ai_level);
	if( 0 != iRet)    return iRet;

	iRet = m_Logger.set_maxfile(ai_maxfile);
	if( 0 != iRet)    return iRet;

	iRet = m_Logger.set_maxsize(ai_maxfilelength);
	if( 0 != iRet)    return iRet;

	iRet = m_Logger.set_logtype(as_logtype);
	if( 0 != iRet)    return iRet;

	return 0;
}



ILog * ILog::instance()
{
	static ILog * _ins = 0;
	if(0 == _ins)
	{
		_ins = new ILog;
		_ins->setLogPara();
	}
	return _ins;
}

const char * ILog::getLog()
{
	return m_szLog;
}
int ILog::writeLog(int iLevel, const char * pszFormat,...)
{
	int     i_ret = 0;
	if (iLevel > m_Logger.get_level())
	{
		return 0;
	}
   	va_list ap;

    	if ( NULL == pszFormat || strlen(pszFormat) <= 0 ) {
      	  return(-1);
  	  }

  	 va_start( ap, pszFormat );
   	 i_ret = m_Logger.apwrite( iLevel, pszFormat, ap );
    	va_end( ap );
   	 return( i_ret );
}


int ILog::writeLog(int iLevel, int lineno, const char *file, const char *func, const char * pszFormat,...)
{
	int     i_ret = 0;
	if (iLevel > m_Logger.get_level())
	{
		return 0;
	}
   	va_list ap;

    	if ( NULL == pszFormat || strlen(pszFormat) <= 0 ) {
      	  return(-1);
  	  }

  	 va_start( ap, pszFormat );
   	 i_ret = m_Logger.apwrite( iLevel, lineno, file, func, pszFormat, ap );
    	va_end( ap );
   	 return( i_ret );
}


int ILog::appendLog(const char    *pszFormat, ...)
{
	 int     i_ret = 0;
   	 va_list ap;

   	 if ( NULL == pszFormat || strlen(pszFormat) <= 0 ) {
       	 return(-1);
  	  }

   	 va_start( ap, pszFormat );
  	 i_ret = vsnprintf( m_szLog + m_iOffSet, MAX_LOG_LENGTH, pszFormat, ap );
   	 va_end( ap );
	 m_iOffSet += i_ret;
   	 return( i_ret );
}


int ILog::write(int iLogLevel)
{
	if(0 == m_iOffSet) return 0;
	m_iOffSet = 0;
	if (iLogLevel > m_Logger.get_level())
	{
		return 0;
	}
	return m_Logger.write(m_szLog, iLogLevel);
}

int ILog::setLogPara(const char *as_file, int  ai_level , const char *as_tformat,  const char * as_logtype , int ai_maxfilelength, int ai_maxfile )
{
	int iRet;
	memset(m_szLog, 0, sizeof(m_szLog));
	iRet = m_Logger.set_file(as_file);
	if( 0 != iRet)    return iRet;

	iRet = m_Logger.set_timeformat(as_tformat);
	if( 0 != iRet)    return iRet;

	iRet = m_Logger.set_level(ai_level);
	if( 0 != iRet)    return iRet;

	iRet = m_Logger.set_maxfile(ai_maxfile);
	if( 0 != iRet)    return iRet;

	iRet = m_Logger.set_maxsize(ai_maxfilelength);
	if( 0 != iRet)    return iRet;

	iRet = m_Logger.set_logtype(as_logtype);
	if( 0 != iRet)    return iRet;

	return 0;
}

void ILog::free()
{
	delete this;
}



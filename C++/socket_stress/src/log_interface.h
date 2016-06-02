/**
 * @file log_interface.h
 * @brief 日志接口文件
 */
#ifndef __LOG_INTERFACE_H__
#define __LOG_INTERFACE_H__

#include "ILog.h"

#define LOG_INIT(prefix, level)     ILog::instance()->setLogPara(prefix, level, LOG_TFORMAT_0, LOG_TYPE_DAY, 0, 0)
#define LOG_UNINIT()                ILog::instance()->free()
#define ERROR_LOG(fmt, vargs...)    ILog::instance()->writeLog(LOG_LEV_ERROR, __LINE__, __FILE__, __func__, fmt, ##vargs)
#define INFO_LOG(fmt, vargs...)     ILog::instance()->writeLog(LOG_LEV_INFO, __LINE__, __FILE__, __func__, fmt, ##vargs)
#define WARNING_LOG(fmt, vargs...)  ILog::instance()->writeLog(LOG_LEV_WARNING, __LINE__, __FILE__, __func__, fmt, ##vargs)
#define DEBUG_LOG(fmt, vargs...)    ILog::instance()->writeLog(LOG_LEV_DEBUG, __LINE__, __FILE__, __func__, fmt, ##vargs)
#define TRACE_LOG(fmt, vargs...)    ILog::instance()->writeLog(LOG_LEV_TRACE, __LINE__, __FILE__, __func__, fmt, ##vargs)

#define FLOWLOG_INIT(prefix, level)     IFlowLog::instance()->setLogPara(prefix, level, LOG_TFORMAT_0, LOG_TYPE_DAY, 0, 0)
#define FLOWLOG_UNINIT()                IFlowLog::instance()->free()
#define ERROR_FLOWLOG(fmt, vargs...)    IFlowLog::instance()->writeLog(LOG_LEV_ERROR, __LINE__, __FILE__, __func__, fmt, ##vargs)
#define INFO_FLOWLOG(fmt, vargs...)     IFlowLog::instance()->writeLog(LOG_LEV_INFO, __LINE__, __FILE__, __func__, fmt, ##vargs)
#define WARNING_FLOWLOG(fmt, vargs...)  IFlowLog::instance()->writeLog(LOG_LEV_WARNING, __LINE__, __FILE__, __func__, fmt, ##vargs)
#define DEBUG_FLOWLOG(fmt, vargs...)    IFlowLog::instance()->writeLog(LOG_LEV_DEBUG, __LINE__, __FILE__, __func__, fmt, ##vargs)
#define TRACE_FLOWLOG(fmt, vargs...)    IFlowLog::instance()->writeLog(LOG_LEV_TRACE, __LINE__, __FILE__, __func__, fmt, ##vargs)


#define LOG_INIT_PTR(logPtr, prefix, level)     (logPtr)->setLogPara(prefix, level, LOG_TFORMAT_0, LOG_TYPE_DAY, 0, 0)
#define LOG_UNINIT_PTR(logPtr)                  (logPtr)->free()
#define ERROR_LOG_PTR(logPtr, fmt, vargs...)    (logPtr)->writeLog(LOG_LEV_ERROR, __LINE__, __FILE__, __func__, fmt, ##vargs)
#define INFO_LOG_PTR(logPtr, fmt, vargs...)     (logPtr)->writeLog(LOG_LEV_INFO, __LINE__, __FILE__, __func__, fmt, ##vargs)
#define WARNING_LOG_PTR(logPtr, fmt, vargs...)  (logPtr)->writeLog(LOG_LEV_WARNING, __LINE__, __FILE__, __func__, fmt, ##vargs)
#define DEBUG_LOG_PTR(logPtr, fmt, vargs...)    (logPtr)->writeLog(LOG_LEV_DEBUG, __LINE__, __FILE__, __func__, fmt, ##vargs)
#define TRACE_LOG_PTR(logPtr, fmt, vargs...)    (logPtr)->writeLog(LOG_LEV_TRACE, __LINE__, __FILE__, __func__, fmt, ##vargs)

#define FLOWLOG_INIT_PTR(logPtr, prefix, level)     (logPtr)->setLogPara(prefix, level, LOG_TFORMAT_0, LOG_TYPE_DAY, 0, 0)
#define FLOWLOG_UNINIT_PTR(logPtr)                  (logPtr)->free()
#define ERROR_FLOWLOG_PTR(logPtr, fmt, vargs...)    (logPtr)->writeLog(LOG_LEV_ERROR, __LINE__, __FILE__, __func__, fmt, ##vargs)
#define INFO_FLOWLOG_PTR(logPtr, fmt, vargs...)     (logPtr)->writeLog(LOG_LEV_INFO, __LINE__, __FILE__, __func__, fmt, ##vargs)
#define WARNING_FLOWLOG_PTR(logPtr, fmt, vargs...)  (logPtr)->writeLog(LOG_LEV_WARNING, __LINE__, __FILE__, __func__, fmt, ##vargs)
#define DEBUG_FLOWLOG_PTR(logPtr, fmt, vargs...)    (logPtr)->writeLog(LOG_LEV_DEBUG, __LINE__, __FILE__, __func__, fmt, ##vargs)
#define TRACE_FLOWLOG_PTR(logPtr, fmt, vargs...)    (logPtr)->writeLog(LOG_LEV_TRACE, __LINE__, __FILE__, __func__, fmt, ##vargs)


#endif //!__LOG_INTERFACE_H__

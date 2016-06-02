#ifndef _CLIB_LOG_H_
#define _CLIB_LOG_H_

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

// version.
#define CLIB_LOG_VERSTR         "1.0.1"
#define CLIB_LOG_VERNUM         10001

// file-path type.
#define CLIB_LOG_PATH_ABSOLUTE  1   ///< absolute address.
#define CLIB_LOG_PATH_RELATIVE  2   ///< relative address.

// defaule log file home-path.
#define CLIB_LOG_PATH           "/tmp"

// log type.
#define CLIB_LOG_TYPE_NOR       ""           ///< Normal roll log.
#define CLIB_LOG_TYPE_DAY       ".%Y%m%d"    ///< Day roll log.
#define CLIB_LOG_TYPE_MON       ".%Y%m"      ///< Month roll log.

// log time format.
#define CLIB_LOG_TFORMAT_0      "[%Y-%m-%d %H:%M:%S] "
#define CLIB_LOG_TFORMAT_1      "%Y-%m-%d %H:%M:%S "
#define CLIB_LOG_TFORMAT_2      "%Y%m%d%H%M%S "
#define CLIB_LOG_TFORMAT_3      ""
#define CLIB_LOG_TFORMAT_4      "%Y-%m-%d %H:%M:%S\t"

// log level.
#define CLIB_LOG_LEV_ERROR      0
#define CLIB_LOG_LEV_INFO       1
#define CLIB_LOG_LEV_WARNING    2
#define CLIB_LOG_LEV_DEBUG      3
#define CLIB_LOG_LEV_TRACE      4

// system define.
#define CLIB_LOG_SLEN_SHOR      8
#define CLIB_LOG_SLEN_NORM      64
#define CLIB_LOG_SLEN_LONG      256
#define CLIB_LOG_SLEN_TEXT      102400

#define CLIB_LOG_SIZE_UNLI      0                   ///< Unlimited
#define CLIB_LOG_SIZE_SMAL      1024 * 1024         ///< 1MB
#define CLIB_LOG_SIZE_NORM      1024 * 1024 * 10    ///< 10MB
#define CLIB_LOG_SIZE_LARG      1024 * 1024 * 100   ///< 100MB
#define CLIB_LOG_SIZE_HUGE      1024 * 1024 * 1024  ///< 1GB

// end of line.
#define CLIB_LOG_EOL_LF         "\n"
#define CLIB_LOG_EOL_CRLF       "\r\n"
#define CLIB_LOG_EOL_NULL       ""

// return.
#define CLIB_LOG_OK             0
#define CLIB_LOG_ERR_NULL       401
#define CLIB_LOG_ERR_FILE       402


class clib_log
{
private:
    char ms_file[CLIB_LOG_SLEN_LONG];
    char ms_tformat[CLIB_LOG_SLEN_NORM];
    char ms_eol[CLIB_LOG_SLEN_SHOR];
    int  mi_level;
    int  mi_maxfile;
    int  mi_maxsize;
    char ms_logtype[CLIB_LOG_SLEN_NORM];

    /**
     * Check if it is needed to change log filename.
     *
     * @param   as_file     Check log filename.
     * @return              0: succ, !=0: fail.
     */
    int check_file( char *as_file );

public:
    /**
     * Initial clib_log class.
     *
     * @param   as_file     Log filename.
     * @param   as_tformat  Log timeformat, define in CLIB_LOG_TFORMAT_x.
     * @param   ai_level    Log level, log will write smaller than this level.
     * @param   ai_maxfile  Max log file number (0 mean no change file, maxsize no use).
     * @param   ai_maxsize  Max log file size (Byte, 0 mean unlimited).
     * @param   as_eol      End of line.
     * @param   as_logtype  Log filename type, define in CLIB_LOG_TYPE_x.
     * @param   ai_pathtype Log filename path type (define CLIB_LOG_PATH_xxx).
     */
    clib_log( const char *as_file     = NULL,
                        const char *as_tformat  = CLIB_LOG_TFORMAT_0,
                        int         ai_level    = CLIB_LOG_LEV_ERROR,
                        int         ai_maxfile  = 5,
                        int         ai_maxsize  = CLIB_LOG_SIZE_NORM,
                        const char *as_logtype  = CLIB_LOG_TYPE_NOR,
                        const char *as_eol      = CLIB_LOG_EOL_LF,
                        int         ai_pathtype = CLIB_LOG_PATH_ABSOLUTE );


    /**
     * Destory clib_log class.
     */
    ~clib_log( void );


    /**
     * Set the log filename.
     *
     * @param   as_file     Log filename.
     * @param   ai_pathtype Log filename path type (define CLIB_LOG_PATH_xxx).
     * @return              0: succ, !=0: fail.
     */
    int set_file( const char *as_file,
                            int         ai_pathtype = CLIB_LOG_PATH_ABSOLUTE );


    /**
     * Set time format.
     *
     * @param   as_tformat  Time format, also see strftime() function use.
     * @return              0: succ, !=0: fail.
     */
    int set_timeformat( const char *as_tformat );


    /**
     * Set log level.
     *
     * @param   ai_level    Log level.
     * @return              0: succ, !=0: fail.
     */
    int set_level( int ai_level );
    /**
     * Get log level.
     *
     * @param   
     * @return mi_level
     */
    int get_level();


    /**
     * Set max file.
     *
     * @param   ai_maxfile  Max log files.
     * @return              0: succ, !=0: fail.
     */
    int set_maxfile( int ai_maxfile );


    /**
     * Set max size.
     *
     * @param   ai_maxsize  Max log size.
     * @return              0: succ, !=0: fail.
     */
    int set_maxsize( int ai_maxsize );


    /**
     * Set Log suffix type.
     *
     * @param   as_logtype  Log filename type, define in CLIB_LOG_TYPE_x.
     * @return              0: succ, !=0: fail.
     */
    int set_logtype( const char *as_logtype );


    /**
     * Set the end of line.
     *
     * @param   as_eol      End of line.
     * @return              0: succ, !=0: fail.
     */
    int set_eol( const char *as_eol );


    /**
     * Write log.
     *
     * @param   as_msg      Message info.
     * @param   ai_level    Level num.
     * @return              0: succ, !=0: fail.
     */
    int write( const char *as_msg,
                         int        ai_level = CLIB_LOG_LEV_ERROR );


    /**
     * Write log using va_list.
     *
     * @param   ai_level        Level num.
     * @param   as_msgformat    Message format.
     * @param   ast_ap          Message va_list.
     * @return                  >= 0: succ, < 0: fail.
     */
    int apwrite( const int     ai_level,
                           const char    *as_msgformat,
                           va_list ast_ap );

    // add by baronyuan
    int apwrite(const int     ai_level,
                int           lineno,
                const char    *file,
                const char    *func,
                const char    *as_msgformat,
                va_list ast_ap );

    /**
     * Write log with variable param.
     *
     * @param   ai_level        Level num.
     * @param   as_msgformat    Message format.
     * @return                  >= 0: succ, < 0: fail.
     */
    int vwrite( const int  ai_level,
                          const char *as_msgformat,
                          ... );


}; // class clib_log.


#endif



#include "Log.h"
#include <sstream>

using namespace std;

static const char* clib_log_lev_str[5] = {
	"ERROR",
	"INFO",
	"WARNING",
	"DEBUG",
	"TRACE",
};


/* {{{ int clib_log_write() */
/**
 * Write Log.
 *
 * @param   as_file       Log filename/directory.
 * @param   as_msg        Log message.
 * @param   ai_len        Message length.
 * @return                0: succ, other: fail.
 */
int clib_log_write( const char *as_file, const char *as_msg, int ai_len )
{
    int fd;
    fd = open( as_file, O_WRONLY|O_APPEND|O_CREAT, 0666 );
    if( fd > 0 ) {
        write( fd, as_msg, ai_len );
        close( fd );
    } else {
        return(-1);
    } // if
    return(0);
}
/* }}} */


/* {{{ int clib_clog::check_file() */
/**
 * Check if need to change log filename.
 *
 * @param   as_file     Check log filename.
 * @return              0: succ, !=0: fail.
 */
int clib_log::check_file( char *as_file )
{
    int     i = 0;
    struct  stat st_stat;
    char    s_oldfile[CLIB_LOG_SLEN_LONG];
    char    s_newfile[CLIB_LOG_SLEN_LONG];

   // memset( s_oldfile, 0, sizeof(s_oldfile) );
  //  memset( s_newfile, 0, sizeof(s_newfile) );

    if ( as_file == NULL || as_file[0] == 0 ) {
        return( CLIB_LOG_ERR_NULL );
    } // if

    if ( mi_maxfile > 0 && mi_maxsize > 0 ) {
        stat( as_file, &st_stat );
        if ( st_stat.st_size > mi_maxsize ) {
            for ( i = mi_maxfile; i > 0; i-- ) {
                if ( i == 1 ) {
                    snprintf( s_newfile, sizeof(s_newfile), "%s.%d", as_file, i );
                    rename( as_file, s_newfile );
                } else {
                    snprintf( s_oldfile, sizeof(s_oldfile), "%s.%d", as_file, i-1 );
                    snprintf( s_newfile, sizeof(s_newfile), "%s.%d", as_file, i );
                    rename( s_oldfile, s_newfile );
                } // if
            } // for
        } // if ( st_stat.st_size > mi_maxsize )
    } // if ( mi_maxfile > 0 && mi_maxsize > 0 )
    return( CLIB_LOG_OK );
}
/* }}} */


/* {{{ clib_clog::clib_log() */
/**
 * Initial clib_log class.
 *
 * @param   as_file     Log filename.
 * @param   as_tformat  Log timeformat, define in CLIB_LOG_TFORMAT_x.
 * @param   ai_level    Log level, log will write smaller than this level.
 * @param   ai_maxfile  Max log file number (0 mean no change file, maxsize no use).
 * @param   ai_maxsize  Max log file size (Byte, 0 mean unlimited).
 * @param   as_logtype  Log filename type, define in CLIB_LOG_TYPE_x.
 * @param   as_eol      End of line.
 * @param   ai_pathtype Log filename path type (define CLIB_LOG_PATH_xxx).
 */
clib_log::clib_log( const char *as_file,
                    const char *as_tformat,
                    int         ai_level,
                    int         ai_maxfile,
                    int         ai_maxsize,
                    const char *as_logtype,
                    const char *as_eol,
                    int         ai_pathtype )
{
    //memset( ms_file,   0, sizeof(ms_file) );

    if ( as_file != NULL ) {
        if ( ai_pathtype == CLIB_LOG_PATH_RELATIVE ) {
            snprintf( ms_file, sizeof(ms_file), "%s/%s",
                        CLIB_LOG_PATH, as_file );
        } else {
            snprintf( ms_file, sizeof(ms_file), "%s", as_file );
        } // if
    } // if ( as_file != NULL )

    if ( NULL != as_tformat ) {
        snprintf( ms_tformat, sizeof(ms_tformat), "%s", as_tformat );
    } else {
        snprintf( ms_tformat, sizeof(ms_tformat), "%s", CLIB_LOG_TFORMAT_0 );
    } // if

    if ( ai_maxfile >= 0 ) mi_maxfile = ai_maxfile;
    else mi_maxfile = 5;

    if ( ai_maxsize >= 0 ) mi_maxsize = ai_maxsize; // Byte
    else mi_maxsize = CLIB_LOG_SIZE_NORM;

    if ( ai_level >= CLIB_LOG_LEV_ERROR ) mi_level = ai_level;
    else mi_level = CLIB_LOG_LEV_ERROR;

    if ( NULL != as_logtype ) {
        snprintf( ms_logtype, sizeof(ms_logtype), "%s", as_logtype );
    } else {
        snprintf( ms_logtype, sizeof(ms_logtype), "%s", CLIB_LOG_TYPE_NOR );
    } // if

    if ( NULL != as_eol ) {
        snprintf( ms_eol, sizeof(ms_eol), "%s", as_eol );
    } else {
        snprintf( ms_eol, sizeof(ms_eol), "%s", CLIB_LOG_EOL_LF );
    } // if

}
/* }}} */


/* {{{ clib_clog::~clib_log() */
/**
 * Destory clib_log class.
 */
clib_log::~clib_log( void )
{
}
/* }}} */


/* {{{ int clib_clog::set_file() */
/**
 * Set the log filename.
 *
 * @param   as_file     Log filename.
 * @param   ai_pathtype Log filename path type (define CLIB_LOG_PATH_xxx).
 * @return              0: succ, !=0: fail.
 */
int clib_log::set_file( const char *as_file,
                        int         ai_pathtype )
{
    if ( NULL == as_file ) return(-1);

  //  memset( ms_file,   0, sizeof(ms_file) );

    if ( ai_pathtype == CLIB_LOG_PATH_RELATIVE ) {
        snprintf( ms_file, sizeof(ms_file), "%s/%s",
                    CLIB_LOG_PATH, as_file );
    } else {
        snprintf( ms_file, sizeof(ms_file), "%s", as_file );
    } // if

    return(0);
}
/* }}} */


/* {{{ int clib_clog::set_timeformat() */
/**
 * Set time format.
 *
 * @param   as_tformat  Time format, also see strftime() function use.
 * @return              0: succ, !=0: fail.
 */
int clib_log::set_timeformat( const char *as_tformat )
{
    if ( NULL == as_tformat ) return(-1);
 //   memset( ms_tformat, 0, sizeof(ms_tformat) );
    snprintf( ms_tformat, sizeof(ms_tformat), "%s", as_tformat );
    return(0);
}
/* }}} */


/* {{{ int clib_clog::set_level() */
/**
 * Set log level.
 *
 * @param   ai_level    Log level.
 * @return              0: succ, !=0: fail.
 */
int clib_log::set_level( int ai_level )
{
    if ( ai_level < CLIB_LOG_LEV_ERROR ) return(-1);
    mi_level = ai_level;
    return(0);
}
/* }}} */

int clib_log::get_level()
{
    return mi_level;
}


/* {{{ int clib_clog::set_maxfile() */
/**
 * Set max file.
 *
 * @param   ai_maxfile  Max log files.
 * @return              0: succ, !=0: fail.
 */
int clib_log::set_maxfile( int ai_maxfile )
{
    if ( ai_maxfile < 0 ) return(-1);
    mi_maxfile = ai_maxfile;
    return(0);
}
/* }}} */


/* {{{ int clib_clog::set_maxsize() */
/**
 * Set max size.
 *
 * @param   ai_maxsize  Max log size.
 * @return              0: succ, !=0: fail.
 */
int clib_log::set_maxsize( int ai_maxsize )
{
    if ( ai_maxsize < 0 ) return(-1);
    mi_maxsize = ai_maxsize;
    return(0);
}
/* }}} */


/* {{{ int clib_clog::set_logtype() */
/**
 * Set Log suffix type.
 *
 * @param   as_logtype  Log filename type, define in CLIB_LOG_TYPE_x.
 * @return              0: succ, !=0: fail.
 */
int clib_log::set_logtype( const char *as_logtype )
{
    if ( NULL != as_logtype ) {
        snprintf( ms_logtype, sizeof(ms_logtype), "%s", as_logtype );
    } else {
        snprintf( ms_logtype, sizeof(ms_logtype), "%s", CLIB_LOG_TYPE_NOR );
    } // if
    return( CLIB_LOG_OK );
}
/* }}} */


/* {{{ int clib_clog::set_eol() */
/**
 * Set the end of line.
 *
 * @param   as_eol      End of line.
 * @return              0: succ, !=0: fail.
 */
int clib_log::set_eol( const char *as_eol )
{
    if ( NULL == as_eol ) return(-1);
  //  memset( ms_eol, 0, sizeof(ms_eol) );
    snprintf( ms_eol, sizeof(ms_eol), "%s", as_eol );
    return(0);
}
/* }}} */


/* {{{ int clib_clog::write() */
/**
 * Write log.
 *
 * @param   as_msg      Message info.
 * @param   ai_level    Level num.
 * @return              0: succ, !=0: fail.
 */
int clib_log::write( const char *as_msg,
                     int        ai_level )
{
    time_t t;
    struct tm tm_re;
    struct tm *ptm = NULL;
    char   s_buf[CLIB_LOG_SLEN_NORM]  ;
    char   s_msg[CLIB_LOG_SLEN_TEXT]  ;
    char   s_file[CLIB_LOG_SLEN_LONG] ;
    int    i_ret = 0;
    int length;
    time( &t );
    ptm = localtime_r( &t, &tm_re );

    if ( ai_level > mi_level ) {
        i_ret = 0;
    } else if ( ms_file[0] == 0
                    || NULL == as_msg
                    || as_msg[0] == 0) {
        i_ret = -1;
    } else {
        if ( ms_tformat[0] != 0  ) {
            strftime( s_buf, sizeof(s_buf), ms_tformat, ptm );
            length = snprintf( s_msg, sizeof(s_msg), "%s%s%s", s_buf, as_msg, ms_eol );
        } else {
            length = snprintf( s_msg, sizeof(s_msg), "%s%s", as_msg, ms_eol );
        } // if

        strftime( s_buf,  sizeof(s_buf),  ms_logtype, ptm );
        snprintf( s_file, sizeof(s_file), "%s%s", ms_file, s_buf );
        this->check_file( s_file );
        i_ret = clib_log_write( s_file, s_msg, length );

    } // if ( ai_level > mi_level )
    return( i_ret );
}
/* }}} */


/* {{{ int clib_clog::apwrite() */
/**
 * Write log using va_list.
 *
 * @param   ai_level        Level num.
 * @param   as_msgformat    Message format.
 * @param   ast_ap          Message va_list.
 * @return                  >= 0: succ, < 0: fail.
 */
int clib_log::apwrite( const int     ai_level,
                       const char    *as_msgformat,
                       va_list ast_ap )
{
    char    s_msg[CLIB_LOG_SLEN_TEXT];
    int     i_ret = 0;

    if ( NULL == as_msgformat || as_msgformat[0] == 0 ) {
        return(-1);
    } // if

   // memset( s_msg, 0, sizeof(s_msg) );
    i_ret = vsnprintf( s_msg, sizeof(s_msg), as_msgformat, ast_ap);
    if ( i_ret < 0 ) {
        return( i_ret );
    } // if

    i_ret = this->write( s_msg, ai_level );
    return( i_ret );
}
/* }}} */

// add by baronyuan
/* {{{ int clib_clog::apwrite() */
/**
 * Write log using va_list.
 *
 * @param   ai_level        Level num.
 * @param   as_msgformat    Message format.
 * @param   ast_ap          Message va_list.
 * @return                  >= 0: succ, < 0: fail.
 */
int clib_log::apwrite( const int     ai_level,
                       int           lineno,
                       const char    *file,
                       const char    *func,
                       const char    *as_msgformat,
                       va_list ast_ap )
{
    char    s_msg[CLIB_LOG_SLEN_TEXT];
    int     i_ret = 0;

    if ( NULL == as_msgformat || as_msgformat[0] == 0 ) {
        return(-1);
    } // if

   // memset( s_msg, 0, sizeof(s_msg) );
    i_ret = vsnprintf( s_msg, sizeof(s_msg), as_msgformat, ast_ap);
    if ( i_ret < 0 ) {
        return( i_ret );
    } // if

	const char* log_lev_str = (ai_level >= CLIB_LOG_LEV_ERROR &&
					ai_level <= CLIB_LOG_LEV_TRACE) ? clib_log_lev_str[ai_level] : clib_log_lev_str[0];
    stringstream ss;
    ss << "\t" << log_lev_str << "\t" << file << "\t" << func << "(" << lineno << ")\t" << s_msg;

    i_ret = this->write( ss.str().c_str(), ai_level );
    return( i_ret );
}
/* }}} */
// add by baronyuan

/* {{{ int clib_clog::vwrite() */
/**
 * Write log with variable param.
 *
 * @param   ai_level        Level num.
 * @param   as_msgformat    Message format.
 * @return                  >= 0: succ, < 0: fail.
 */
int clib_log::vwrite( const int  ai_level,
                      const char *as_msgformat,
                      ... )
{
    int     i_ret = 0;
    va_list ap;

    if ( NULL == as_msgformat || as_msgformat[0] == 0) {
        return(-1);
    } // if

    va_start( ap, as_msgformat );
    i_ret = this->apwrite( ai_level, as_msgformat, ap );
    va_end( ap );
    return( i_ret );
}
/* }}} */



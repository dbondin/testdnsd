#ifndef __TESTDNSD_GLOBALS_H__
#define __TESTDNSD_GLOBALS_H__

#include "config.h"
#include "lookupdb.h"
#include "data_queue.h"

extern lookupdb DB;
extern int SOCKET;
extern data_queue INQ;
extern data_queue OUTQ;

#ifndef XXLOG
#if TESTDNSD_DAEMON == 1
 #include<syslog.h>
 #define XXLOG_INIT() openlog(TESTDNSD_SYSLOG_IDENT, LOG_PID | LOG_NDELAY, LOG_USER)
 #define XXLOG(...) syslog ( LOG_INFO , __VA_ARGS__ )
#else
 #include<stdio.h>
 #define XXLOG_INIT() {}
 #define XXLOG(...) printf ( __VA_ARGS__ )
#endif /* TESTDNSD_DAEMON == 1 */
#endif /* XXLOG */

#endif /* __TESTDNSD_GLOBALS_H__ */

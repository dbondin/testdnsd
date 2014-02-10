#include "config.h"
#include "globals.h"

#if TESTDNSD_DAEMON == 1
 #include <stdarg.h>
 #include <syslog.h>
#else /* TESTDNSD_DAEMON == 1 */
 #include <stdarg.h>
 #include <stdio.h>
#endif /* TESTDNSD_DAEMON == 1 */

lookupdb DB;
int SOCKET;
data_queue INQ;
data_queue OUTQ;

void XXLOG_INIT() {
#if TESTDNSD_DAEMON == 1
    openlog(TESTDNSD_SYSLOG_IDENT, LOG_PID | LOG_NDELAY, LOG_USER);
#else /* TESTDNSD_DAEMON == 1 */
    /* Do nothing */
#endif /* TESTDNSD_DAEMON == 1 */
}

void XXLOG(const char * format, ...) {
#if TESTDNSD_DAEMON == 1
    va_list ap;
    va_start(ap, format);
    vsyslog(LOG_INFO, format, ap);
    va_end(ap);
#else
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    printf("\n");
    va_end(ap);
#endif /* TESTDNSD_DAEMON == 1 */
}

void XXLOG_DEBUG(const char * format, ...) {
#if TESTDNSD_DEBUG == 1
 #if TESTDNSD_DAEMON == 1
    va_list ap;
    va_start(ap, format);
    vsyslog(LOG_DEBUG, format, ap);
    va_end(ap);
 #else /* TESTDNSD_DAEMON == 1 */
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    printf("\n");
    va_end(ap);
 #endif /* TESTDNSD_DAEMON == 1 */
#else /* TESTDNSD_DEBUG == 1 */
    /* Nothing here */
#endif /* TESTDNSD_DEBUG == 1 */
}

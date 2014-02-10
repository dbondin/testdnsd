#ifndef __TESTDNSD_CONFIG_H__
#define __TESTDNSD_CONFIG_H__

#ifndef TESTDNSD_DAEMON
#define TESTDNSD_DAEMON 0
#endif /* TESTDNSD_DAEMON */

#ifndef TESTDNSD_DEBUG
#define TESTDNSD_DEBUG 0
#endif /* TESTDNSD_DEBUG */

#ifndef TESTDNSD_SYSLOG_IDENT
#define TESTDNSD_SYSLOG_IDENT "testdnds"
#endif /* TESTDNSD_SYSLOG_IDENT */

#ifndef TESTDNSD_BIND_ADDR
#define TESTDNSD_BIND_ADDR "0.0.0.0"
#endif /* TESTDNSD_BIND_ADDR */

#ifndef TESTDNSD_BIND_PORT
#define TESTDNSD_BIND_PORT 5353
#endif /* TESTDNSD_BIND_PORT */

#ifndef TESTDNSD_LOOKUPDB_FILE
#define TESTDNSD_LOOKUPDB_FILE "/tmp/lookupdb.txt"
#endif /* TESTDNSD_LOOKUPDB_FILE */

#ifndef TESTDNSD_INQ_SIZE
#define TESTDNSD_INQ_SIZE 1024
#endif /* TESTDNSD_INQ_SIZE */

#ifndef TESTDNSD_OUTQ_SIZE
#define TESTDNSD_OUTQ_SIZE 1024
#endif /* TESTDNSD_OUTQ_SIZE */

#ifndef TESTDNSD_PROC_THREAD_COUNT
#define TESTDNSD_PROC_THREAD_COUNT 8
#endif /* TESTDNSD_PROC_THREAD_COUNT */

#define TESTDNSD_MAX_PACKET_SIZE 512 /* According to RCF-1035 */

#endif /* __TESTDNSD_CONFIG_H__ */

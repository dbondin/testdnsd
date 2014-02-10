#ifndef __TESTDNSD_GLOBALS_H__
#define __TESTDNSD_GLOBALS_H__

#include "config.h"
#include "lookupdb.h"
#include "data_queue.h"

extern lookupdb DB;
extern int SOCKET;
extern data_queue INQ;
extern data_queue OUTQ;

void XXLOG_INIT();
void XXLOG(const char * format, ...);
void XXLOG_DEBUG(const char * format, ...);

#endif /* __TESTDNSD_GLOBALS_H__ */

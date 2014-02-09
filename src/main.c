#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <stdlib.h>

#include "config.h"
#include "lookupdb.h"

lookupdb DB;

int main(int argc, char ** argv) {

    /* Became a daemon process */
    if(daemon(0, 0)) {
        perror("Unable to became a daemon process");
        return 1;
    }

    /* Initialize syslog */
    openlog(TESTDNSD_SYSLOG_IDENT, LOG_PID | LOG_NDELAY, LOG_USER);

    /* Started now */
    syslog(LOG_INFO, "Started");

    if(lookupdb_init(&DB)) {
        syslog(LOG_ERR, "Failed initializing lookup database. Exiting");
        return 2;
    }

    syslog(LOG_INFO, "Starting lookup batabase data load from file");
    if(lookupdb_load(&DB, "")) {
        syslog(LOG_ERR, "Failed loading lookup batabase data from file. Exiting");
        return 3;
    }
    syslog(LOG_INFO, "Finished lookup batabase data load from file. %ld items loaded", lookupdb_size(&DB));

    closelog();

    return 0;
}

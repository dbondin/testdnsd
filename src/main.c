#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#include "globals.h"
#include "config.h"
#include "lookupdb.h"
#include "receiver.h"
#include "data_queue.h"
#include "processor.h"

int main(int argc, char ** argv) {

    int i;
    pthread_t tids[TESTDNSD_PROC_THREAD_COUNT];
    struct sockaddr_in my_addr;

    /* Initialize syslog */
    XXLOG_INIT();

    /* Started now */
    XXLOG("Starting");

    /* Dealing with lookup database */
    if(lookupdb_init(&DB)) {
        XXLOG("Failed initializing lookup database. Exiting");
        exit(1);
    }

    XXLOG("Starting lookup batabase data load from file");
    if(lookupdb_load(&DB, TESTDNSD_LOOKUPDB_FILE)) {
        XXLOG("Failed loading lookup batabase data from file. Exiting");
        exit(1);
    }
    XXLOG("Finished lookup batabase data load from file. %ld items loaded", lookupdb_size(&DB));

    /* Dealing with in/out queues */
    XXLOG("Preparing in/out queues");
    if(data_queue_init(&INQ, TESTDNSD_INQ_SIZE) ||
       data_queue_init(&OUTQ, TESTDNSD_OUTQ_SIZE)) {
        XXLOG("Failed initializing in/out queues");
        exit(1);
    }

    /* Dealing with udp socket */
    XXLOG("Opening receiving socket at %s:%d", TESTDNSD_BIND_ADDR, TESTDNSD_BIND_PORT);
    SOCKET = socket(AF_INET, SOCK_DGRAM, 0);
    if(SOCKET == -1) {
        XXLOG("Failed creating socket");
        exit(1);
    }
    my_addr.sin_family = AF_INET;
    if(!inet_aton(TESTDNSD_BIND_ADDR, &(my_addr.sin_addr))) {
	XXLOG("Invalid TESTDNSD_BIND_ADDDR value '%s'. Exiting", TESTDNSD_BIND_ADDR);
	exit(1);
    }
    my_addr.sin_port = htons(TESTDNSD_BIND_PORT);
    if(bind(SOCKET, (struct sockaddr *) &my_addr, sizeof(my_addr))) {
	XXLOG("Error binding socket. Exiting (%s)", strerror(errno));
	exit(1);
    }
    XXLOG("Socket opened");

    XXLOG("Creating processing threads");
    /* Starting processing threads */
    for(i=0; i<TESTDNSD_PROC_THREAD_COUNT; i++) {
	if(pthread_create(&tids[i], NULL, processor_thread_fn, NULL)) {
	    XXLOG("Error creating processor threads. Exiting");
	    exit(1);
	}
    }
    XXLOG("%d processing threads created successfully", i);

    /* Became a daemon process */
    if(TESTDNSD_DAEMON) {
	if(daemon(0, 0)) {
	    XXLOG("Unable to became a daemon process. Exiting");
	    exit(1);
	}
    }

    /* Receiver infinite loop starts here */
    receiver();

    /* Finishing up (just in case) */
    close(SOCKET);
    closelog();

    return 0;
}

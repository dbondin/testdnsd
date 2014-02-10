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
#include <pwd.h>
#include <grp.h>

#include "globals.h"
#include "config.h"
#include "lookupdb.h"
#include "receiver.h"
#include "data_queue.h"
#include "processor.h"
#include "sender.h"

int main(int argc, char ** argv) {

    int i;
    pthread_t prc_tids[TESTDNSD_PROC_THREAD_COUNT];
    processor_thread_fn_arg_t prc_args[TESTDNSD_PROC_THREAD_COUNT];
    pthread_t snd_tid;
    struct sockaddr_in my_addr;
    struct passwd * user_info;
    struct group * group_info;

    /* Initialize syslog */
    XXLOG_INIT();

    /* Started now */
    XXLOG("Starting");

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

    /* If running as ROOT - switch to non-privileged user */
    if(getuid() == 0) {
	XXLOG("Changing user:group to %s:%s", TESTDNSD_RUN_USER, TESTDNSD_RUN_GROUP);
	user_info = getpwnam(TESTDNSD_RUN_USER);
	if(user_info == NULL) {
	    XXLOG("Can't find user info. Exiting");
	    exit(1);
	}
	group_info = getgrnam(TESTDNSD_RUN_GROUP);
	if(group_info == NULL) {
	    XXLOG("Can't find group info. Exiting");
	    exit(1);
	}
	XXLOG("UID:GID = %d:%d", user_info->pw_uid, group_info->gr_gid);
	if(setgid(group_info->gr_gid)) {
	    XXLOG("Error changing GID. Exiting");
	    exit(1);	    
	}
	if(setuid(user_info->pw_uid)) {
	    XXLOG("Error changing UID. Exiting");
	    exit(1);	    
	}
    }

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

    XXLOG("Creating sender threads");
    if(pthread_create(&snd_tid, NULL, sender_thread_fn, NULL)) {
	XXLOG("Error creating sender thread. Exiting");
	exit(1);
    }
    XXLOG("Sender thread created successfully");

    XXLOG("Creating processing threads");
    /* Starting processing threads */
    for(i=0; i<TESTDNSD_PROC_THREAD_COUNT; i++) {
	prc_args[i].prc_id = i;
	if(pthread_create(&prc_tids[i], NULL, processor_thread_fn, &(prc_args[i]))) {
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

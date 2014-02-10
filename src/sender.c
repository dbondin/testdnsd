#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>

#include "config.h"
#include "globals.h"
#include "lookupdb.h"

void * sender_thread_fn(void * args) {

    int status;
    data_queue_element dqe;

    XXLOG("SND: Started");

    while(1) {
	/* Getting next portion of the data to send */
	if(data_queue_get_data(&OUTQ, &dqe) == 0) {
	    status = sendto(SOCKET,
			    (void *) dqe.data,
			    dqe.data_len,
			    0,
			    (struct sockaddr *) &(dqe.addr),
			    sizeof(struct sockaddr));
	    if(status != -1) {
		XXLOG_DEBUG("SND: reply sent");
	    }
	    else {
		XXLOG("SND: error sending reply (%s)", strerror(errno));
	    }
	}
    }

    return NULL;
}

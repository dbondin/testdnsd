#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "globals.h"
#include "receiver.h"
#include "dns_header.h"

#define RECEIVER_DEBUG 1

void receiver() {

    ssize_t bytes_read;
    char read_buffer[RECEIVER_READ_BUFFER_SIZE];
    struct sockaddr_in src_addr;
    socklen_t src_len;

    XXLOG("Starting receiver");

    while(1) {

#ifdef RECEIVER_DEBUG
	XXLOG("Waiting for data");
#endif /* RECEIVER_DEBUG */

	bytes_read = recvfrom(SOCKET,
			      (void *) read_buffer,
			      RECEIVER_READ_BUFFER_SIZE,
			      0,
			      (struct sockaddr *) &src_addr,
			      &src_len);
	if(bytes_read == -1) {
	    if(errno == EINTR) {
		continue;
	    }
	    else {
		/* Shit happens */
		XXLOG("Receiver failed (%s)", strerror(errno));
		return;
	    } 
	}

	XXLOG("Receiver receives %d bytes", bytes_read);

	if(bytes_read > 0) {
#ifdef RECEIVER_DEBUG
	XXLOG("Putting data to the queue");
#endif /* RECEIVER_DEBUG */
	    if(data_queue_put_data(&INQ, &src_addr, (void *) read_buffer, bytes_read)) {
		XXLOG("Receiver error: input queue operation failure");
	    }
#ifdef RECEIVER_DEBUG
	    else {
		XXLOG("Putting data to the queue success");
	    }
#endif /* RECEIVER_DEBUG */
	}
    }

    XXLOG("Receiver finished");
    return;
}

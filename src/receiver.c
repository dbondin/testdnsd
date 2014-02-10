#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "globals.h"
#include "receiver.h"
#include "dns_header.h"

void receiver() {

    ssize_t bytes_read;
    char read_buffer[RECEIVER_READ_BUFFER_SIZE];
    struct sockaddr_in src_addr;
    socklen_t src_len;

    XXLOG("Starting receiver");

    while(1) {

	XXLOG_DEBUG("Waiting for data");

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
	    XXLOG_DEBUG("Putting data to the queue");
	    if(data_queue_put_data(&INQ, &src_addr, (void *) read_buffer, bytes_read)) {
		XXLOG("Receiver error: input queue operation failure");
	    }
	    else {
		XXLOG_DEBUG("Putting data to the queue success");
	    }
	}
    }

    XXLOG("Receiver finished");
    return;
}

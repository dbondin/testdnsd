#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>

#include "data_queue.h"

int data_queue_init(data_queue * dq, int size) {
    if(dq == (void *) 0 || size <= 0) {
	return 1;
    }

    dq->data = (data_queue_element *) malloc(sizeof(data_queue_element) * size);

    if(dq->data == (void *) 0) {
	return 2;
    }

    if(pthread_mutex_init(&(dq->mutex), (void *) 0)) {
	free((void *)dq->data);
	return 3;
    }

    if(pthread_cond_init(&(dq->cond), (void *) 0)) {
	pthread_mutex_destroy(&(dq->mutex));
	free((void *)dq->data);
	return 4;	
    }

    dq->size = size;
    dq->count = 0;
    
    return 0;
}

int data_queue_get_size(const data_queue * dq) {
    if(dq == (void *) 0) {
	return 0;
    }
    return dq->size;
}

int data_queue_get_count(data_queue * dq) {
    int res = -1;
    if(dq != (void *) 0) {
	if(!pthread_mutex_lock(&(dq->mutex))) {
	    res = dq->count;
	    pthread_mutex_unlock(&(dq->mutex));
	}
    }
    return res;
}

int data_queue_get_data(data_queue * dq, data_queue_element * data) {
    int res = -1;
    if(dq != (void *) 0) {
	if(!pthread_mutex_lock(&(dq->mutex))) {
	    while(1) {
		if(dq->count == 0) {
		    pthread_cond_wait(&(dq->cond), &(dq->mutex));
		    continue;
		}

		dq->count --;
		if(data != (void *) 0) {
		    memcpy((void *) data,
			   (void *) &(dq->data[dq->count]),
			   sizeof(data_queue_element));
		}
		res = 0;
		break;
	    }


	    pthread_mutex_unlock(&(dq->mutex));
	}
    }
    return res;
}
int data_queue_put_data(data_queue * dq, const struct sockaddr_in * addr, const void * data, int data_len) {
    int res = -1;
    if(dq != (void *) 0 &&
       data != (void *) 0 &&
       addr != (void *) 0 &&
       data_len > 0 &&
       data_len <= TESTDNSD_MAX_PACKET_SIZE) {

	if(!pthread_mutex_lock(&(dq->mutex))) {
	    if(dq->count < dq->size) {
		memcpy((void *) &(dq->data[dq->count].addr),
		       (void *) addr,
		       sizeof(addr));
		memcpy((void *) &(dq->data[dq->count].data),
		       (void *) data,
		       data_len);
		dq->data[dq->count].data_len = data_len;
		dq->count ++;
		res = 0;
		pthread_cond_signal(&(dq->cond));
	    }
	    pthread_mutex_unlock(&(dq->mutex));
	}	
    }
    return res;
}

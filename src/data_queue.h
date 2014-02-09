#ifndef __TESTDNSD_DATA_QUEUE_H__
#define __TESTDNSD_DATA_QUEUE_H__

#include <netinet/in.h>
#include <pthread.h>

#include "config.h"

struct _data_queue_element {
    struct sockaddr_in addr;
    char data[TESTDNSD_MAX_PACKET_SIZE];
    int data_len;
};

typedef struct _data_queue_element data_queue_element;

struct _data_queue {
    int size;
    int count;
    data_queue_element * data;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

typedef struct _data_queue data_queue;

int data_queue_init(data_queue * dq, int size);
int data_queue_get_size(const data_queue * dq);
int data_queue_get_count(data_queue * dq);
int data_queue_get_data(data_queue * dq, data_queue_element * data);
int data_queue_put_data(data_queue * dq, const struct sockaddr_in * addr, const void * data, int data_len);

#endif /* __TESTDNSD_DATA_QUEUE_H__ */

#ifndef __TESTDNSD_PROCESSOR_H__
#define __TESTDNSD_PROCESSOR_H__

typedef struct _processor_thread_fn_arg_t processor_thread_fn_arg_t;
struct _processor_thread_fn_arg_t {
    int prc_id;
};

void * processor_thread_fn(void * args);

#endif /* __TESTDNSD_PROCESSOR_H__ */

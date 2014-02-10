#ifndef __TESTDNSD_DNS_HEADER_H__
#define __TESTDNSD_DNS_HEADER_H__

#include <stdint.h>

#pragma pack(1)
struct _dns_header {
    uint16_t id;
    uint16_t flags;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
};
#pragma pack()

typedef struct _dns_header dns_header;

struct _dns_question {
    char * qname;
    uint16_t qtype;
    uint16_t qclass;
};

typedef struct _dns_question dns_question;

int dns_question_load(const void * data, dns_question * dnsq);

#endif /* __TESTDNSD_DNS_HEADER_H__ */

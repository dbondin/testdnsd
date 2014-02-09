#ifndef __TESTDNSD_DNS_HEADER_H__
#define __TESTDNSD_DNS_HEADER_H__

#include <stdint.h>

#pragma pack(1)
struct _dns_header {
    uint16_t id;
    uint16_t flags;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t arcount;
};
#pragma pack()

typedef struct _dns_header dns_header;

#endif /* __TESTDNSD_DNS_HEADER_H__ */

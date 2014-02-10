#include <string.h>
#include <arpa/inet.h>

#include "dns_header.h"

int dns_question_load(const void * data, dns_question * dnsq) {
    int res = -1;
    if(data != NULL && dnsq != NULL) {
	int qname_len;
	dnsq->qname = (char *) data;
	qname_len = strlen(dnsq->qname);
	dnsq->qtype = ntohs(*(uint16_t *)(data + qname_len + 1));
	dnsq->qclass = ntohs(*(uint16_t *)(data + qname_len + 1 + sizeof(dnsq->qtype)));
	res = 0;
    }
    return res;
}

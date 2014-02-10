#include "config.h"
#include "globals.h"
#include "processor.h"
#include "dns_header.h"
#include "lookupdb.h"

void qname2str(const char * qname, char * str) {
    int i = 0;
    int z = 0;
    while(qname[i] != 0) {
	if(z == 0) {
	    if(i != 0) {
		str[i-1] = '.';
	    }
	    z = (unsigned char) qname[i];
	}
	else {
	    str[i-1] = qname[i];
	    z--;
	}
	i++;
    }
    str[i-1] = 0;
}

void * processor_thread_fn(void * args) {

    dns_header h;
    data_queue_element dqe;
    dns_question dnsq;
    char name[TESTDNSD_MAX_PACKET_SIZE];
    lookupdb_addr * ldb_addr;

    while(1) {
	if(data_queue_get_data(&INQ, &dqe)) {
	    /* Error getting data from input queue. Ignore */
#if TESTDNSD_DEBUG == 1
	    XXLOG_DEBUG("Processor: Error getting data from input queue. Ignoring");
#endif /* TESTDNSD_DEBUG == 1 */
	    continue;
	}
	if(dqe.data_len >= sizeof(dns_header)) {
	    /* Get the header data (big/little endian transform) */
	    h.id = ntohs(((dns_header *) dqe.data)->id);
	    h.flags = ntohs(((dns_header *) dqe.data)->flags);
	    h.qdcount = ntohs(((dns_header *) dqe.data)->qdcount);
	    h.ancount = ntohs(((dns_header *) dqe.data)->ancount);
	    h.nscount = ntohs(((dns_header *) dqe.data)->nscount);
	    h.arcount = ntohs(((dns_header *) dqe.data)->arcount);

#if TESTDNSD_DEBUG == 1
	    XXLOG_DEBUG("Processor: got id=0x%.4x, flags=0x%.4x, qdcount=%u ancount=%u nscount=%u arcount=%u",
			h.id,
			h.flags,
			h.qdcount,
			h.ancount,
			h.nscount,
			h.arcount);
	    XXLOG_DEBUG("Processor: %d %d %d", (h.flags & 0x8000), (h.flags & 0x7800), (h.flags & 0x0200));
#endif /* TESTDNSD_DEBUG == 1 */

	    if((h.flags & 0x8000) == 0 /* QR == 0 -  it is a query */
	       && (h.flags & 0x7800) == 0 /* OPCODE == 0 - it is standart query */
	       && (h.flags & 0x0200) == 0 /* TR == 0 - not truncated */
	       && h.qdcount == 1 /* Number of questions in query. Only supporting single question for now */
	       && h.ancount == 0 /* No support for this */
	       && h.nscount == 0 /* No support for this */
	       && h.arcount == 0 /* No support for this */
	       )
	    {
		/* Check the dns question contents */
		if(!dns_question_load((void *)(dqe.data + sizeof(dns_header)), &dnsq)) {
#if TESTDNSD_DEBUG == 1
		    qname2str(dnsq.qname, name);
		    XXLOG_DEBUG("Processor: qname='%s' qtype=0x%x qclass=0x%x",
				name,
				dnsq.qtype,
				dnsq.qclass);
#endif /* TESTDNSD_DEBUG == 1 */
		    if(dnsq.qtype == 1 /* 'A' - the only one supported for now */
		       && dnsq.qclass == 1 /* 'IN' */) {
			/* Search for a data in DB  */
			ldb_addr = lookupdb_search(&DB, name);
			if(ldb_addr != NULL) {
#if TESTDNSD_DEBUG == 1
			    XXLOG_DEBUG("Processor: FOUND");
#endif /* TESTDNSD_DEBUG == 1 */
			}
			else {
#if TESTDNSD_DEBUG == 1
			    XXLOG_DEBUG("Processor: NOT FOUND");
#endif /* TESTDNSD_DEBUG == 1 */
			}
		    }
		}
	    }
	}
	/* Send something back with error */

	//TODO send error response here !!!

#if TESTDNSD_DEBUG == 1
	XXLOG_DEBUG("Processor: Error response will be sent");
#endif /* TESTDNSD_DEBUG == 1 */
    }

    return NULL;
}

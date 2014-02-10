#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "lookupdb.h"
#include "globals.h"

static lookupdb_entry * new_lookupdb_entry();
static lookupdb_addr * new_lookupdb_addr();
static void lookupdb_add_entry(lookupdb * ldb, const char * name, int name_len, const struct in_addr * iaddr);
static void lookupdb_entry_add_addr(lookupdb_entry * ep, const struct in_addr * iaddr);

int lookupdb_init(lookupdb * ldb) {
    if(ldb == NULL) {
	return -1;
    }
    ldb->size = 0;
    ldb->entry = new_lookupdb_entry();
    if(ldb->entry == NULL) {
	return -1;
    }
    return 0;
}

int lookupdb_load(lookupdb * ldb, const char * filename) {
    FILE * f;
    char name[256];
    int name_len;
    char addr[32];
    int i;
    struct in_addr iaddr;

    if(ldb != NULL && filename != NULL) {
	XXLOG("LookupDB: Opening file '%s'", filename);
	f = fopen(filename, "r");
	if(f != NULL) {
	    XXLOG("LookupDB: File opened. Reading. Bad lines will be ignored");
	    while(fscanf(f, "%255s %31s", name, addr) != EOF) {
		name_len = strlen(name);
		if(name_len != 0) {
		    if(inet_aton(addr, &iaddr) != 0) {
			/* Normalize the name  */
			for(i=0; i<name_len; i++) {
			    name[i] = 0x7f & tolower(name[i]);
			}
			
			/* Add the entry into database */
			lookupdb_add_entry(ldb, name, name_len, &iaddr);
		    }
		}
	    }
	    XXLOG("LookupDB: Total %ld items read", ldb->size);
	    fclose(f);
	    return 0;
	}
	XXLOG("LookupDB: Failed to open the file");
    }

    return -1;
}

long lookupdb_size(const lookupdb * ldb) {
    if(ldb == NULL) {
	return -1;
    }
    return ldb->size;
}

lookupdb_addr * lookupdb_search(const lookupdb * ldb, const char * name) {

    lookupdb_entry * ep;
    int idx;

    if(ldb != NULL && name != NULL) {
	ep = ldb->entry;
	for(;*name != 0; name ++) {
	    if(ep != NULL) {
		idx = 0x7f & tolower(*name);
		ep = ep->next[idx];
	    }
	    else {
		break;
	    }
	}
	if(ep != NULL) {
	    return ep->addr_list;
	}
    }

    return NULL;
}

int lookupdb_clear(lookupdb * ldb) {
    /* NOT IMPLEMENTED YET */
    return 0;
}

lookupdb_entry * new_lookupdb_entry() {
    lookupdb_entry * e = (lookupdb_entry *) malloc(sizeof(lookupdb_entry));
    if(e != NULL) {
	e->addr_list = NULL;
        memset((void *) e->next, 0, sizeof(e->next));
    }
    return e;
}

void lookupdb_add_entry(lookupdb * ldb, const char * name, int name_len, const struct in_addr * iaddr) {
    int i;
    lookupdb_entry * ep = ldb->entry;
    int idx;

    for(i=0; i<name_len; i++) {
	idx = (int) name[i];
	if(ep->next[idx] == NULL) {
	    ep->next[idx] = new_lookupdb_entry();
	}
	if(ep->next[idx] == NULL) {
	    /* Memory allocation error probably. Fix logic here */
	    XXLOG("LDB: ADD_ENTRY: Memory allocation error");
	    return;
	}
	ep = ep->next[idx];
    }

    lookupdb_entry_add_addr(ep, iaddr);
    ldb->size ++;

    return;
}

void lookupdb_entry_add_addr(lookupdb_entry * ep, const struct in_addr * iaddr) {
    lookupdb_addr * last = NULL;
    lookupdb_addr * next = ep->addr_list;
    while(next != NULL) {
	last = next;
	next = next->next;
    }
    next = new_lookupdb_addr(iaddr);
    if(next != NULL) {
	if(last == NULL) {
	    ep->addr_list = next;
	}
	else {
	    last->next = next;
	}
    }

    return;
}

static lookupdb_addr * new_lookupdb_addr(const struct in_addr * iaddr) {
    lookupdb_addr * addr;
    addr = (lookupdb_addr *) malloc(sizeof(lookupdb_addr));
    if(addr != NULL) {
	addr->addr = *iaddr;
	addr->next = NULL;
    }
    return addr;
}

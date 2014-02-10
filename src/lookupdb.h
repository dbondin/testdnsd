#ifndef __TESTDNSD_LOOKUPDB_H__
#define __TESTDNSD_LOOKUPDB_H__

#include <netinet/in.h>

#define LOOKUPDB_ENTRY_NEXT_MAX 128
#define LOOKUPDB_ENTRY_NEXT_MASK 0x7f

typedef struct _lookupdb_addr lookupdb_addr;
struct _lookupdb_addr {
    struct in_addr addr;
    lookupdb_addr * next;
};

typedef struct _lookupdb_entry lookupdb_entry;
struct _lookupdb_entry {
    lookupdb_entry * next[LOOKUPDB_ENTRY_NEXT_MAX];
    lookupdb_addr * addr_list;
};

typedef struct _lookupdb lookupdb;
struct _lookupdb {
    unsigned long int size;
    lookupdb_entry * entry;
};

int lookupdb_init(lookupdb * ldb);
int lookupdb_load(lookupdb * ldb, const char * filename);
long lookupdb_size(const lookupdb * ldb);
lookupdb_addr * lookupdb_search(const lookupdb * ldb, const char * name);
int lookupdb_clear(lookupdb * ldb);

#endif /* __TESTDNSD_LOOKUPDB_H__ */

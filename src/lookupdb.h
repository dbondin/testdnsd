#ifndef __TESTDNSD_LOOKUPDB_H__
#define __TESTDNSD_LOOKUPDB_H__

struct _lookupdb {
    int foo;
};

typedef struct _lookupdb lookupdb;

int lookupdb_init(lookupdb * ldb);
int lookupdb_load(lookupdb * ldb, const char * filename);
long lookupdb_size(const lookupdb * ldb);

#endif /* __TESTDNSD_LOOKUPDB_H__ */

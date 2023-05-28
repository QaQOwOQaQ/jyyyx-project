#ifndef HASH_TABLE_GUARD
#define HASH_TABLE_GUARD

// false(0) and true(1)
typedef enum bool_t {
    FALSE, TRUE
} boolean;

typedef struct chain_t {
    void *data;
    struct chain_t *next;
} Chain;

typedef struct hashtable_t {
    unsigned int numBucktes;
    Chain **buckets;    // a pointer to pointer array
} Hashtable;

Hashtable* hashtable_init(int numBuckets);
int hashtable_insert(Hashtable *ht, void *data, int datalen);
void* hashtable_find(Hashtable *ht, void *data, 
            int datalen, boolean(*cmp)(const void *lhs, const void *rhs));
void hashtable_destory(Hashtable *ht);

#endif
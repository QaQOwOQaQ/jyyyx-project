#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <hash_table.h>

/* Good habit to assert the memory we malloc if malloc_false
   Good habit to set NULL when we free a block of memory
   For void* data, we must know it's length, else we cant use it 
*/

/* only we know the length of obj can we use it successfully */
/* from Redis hashcode funciotn */
static unsigned int hash_code(const void *obj, int len)
{
    unsigned int hash = 5381;
    unsigned char *tmp = (unsigned char*)malloc(len);
    assert(tmp);
    memcpy(tmp, obj, len);
    unsigned char *buf = tmp;
    while(len -- )
    {
        hash = ((hash << 5) + hash) +  (*buf ++ );
    }
    if(tmp != NULL)
    {
        free(tmp);
        tmp = NULL; // good habit
    }
    buf = NULL; // good habit
    return hash;
}

Hashtable* hashtable_init(int numBuckets) 
{
    if(numBuckets <= 0) 
    {
        return NULL;
    }
    Hashtable *ht = (Hashtable *)malloc(sizeof(Hashtable));
    assert(ht);
    ht->numBucktes = numBuckets;
    ht->buckets = (Chain **)malloc(sizeof(Chain) * ht->numBucktes);
    for(int i = 0; i < ht->numBucktes; i ++ )
    {
        ht->buckets[i] = NULL;
    }
    return ht;
}

// return -1 if insert error, else return 0
int hashtable_insert(Hashtable *ht, void *data, int datalen)
{
    if(ht == NULL)
    {
        return -1;
    }
    // hash code  value
    unsigned int hc = hash_code(data, datalen);
    // convert hash code to bucket number
    unsigned int bkn = hc % ht->numBucktes;
#ifdef DEBUG
    printf("HashCode: %u; BucketNumber: %u.\n", hc, bkn);
#endif
    // insert, head insert in link
    Chain *newchain = (Chain*)malloc(sizeof(Chain));
    assert(newchain);
    newchain->next = NULL;
    newchain->data = data;

    if(ht->buckets[bkn] == NULL) 
    {
        ht->buckets[bkn] = newchain;
    }
    else 
    {
        newchain->next = ht->buckets[bkn];
        ht->buckets[bkn] = newchain;
    }
    return 0;
}

// return NULL if not find, else return something we want
// there I return the find_times [which is malloc in mmemory]
// and need caller to free !
void* hashtable_find(Hashtable *ht, void *data, 
        int datalen, boolean(*equal)(const void *lhs, const void *rhs))
{
    if(ht == NULL)
    {
        return NULL;
    }
    unsigned int hc = hash_code(data, datalen);
    unsigned int bkn = hc % ht->numBucktes;
    // put it in tmp_obj can let compile optmizame it and simplify our code
    Chain *curChain = ht->buckets[bkn];
    // not a good design to return a malloc memory
    // but it's easy
    unsigned long findtimes = 0;
    if(curChain != NULL)
    {
        while(curChain)
        {
            ++ findtimes;
            if(equal(curChain->data, data))
            {
#ifdef DEBUG
                printf("find successful ---> %d times.\n", findtimes);
#endif
                // return curChain->data;
                return (void*)(findtimes);
            }
            else 
            {
                curChain = curChain->next;
            }
        }
    }
#ifdef DEBUG
    printf("find fault ---> %d times.\n", findtimes);
#endif
    return NULL;
}

// assist to help destory hashtable
static void chain_destory(Chain *chain)
{
    if(chain == NULL)
    {
        return ;
    }
    while(chain != NULL)
    {
        Chain *delnode = chain;
        free(delnode);
        chain = chain->next;
        delnode = NULL;
    }
}

void hashtable_destory(Hashtable *ht)
{
    if(ht == NULL || !ht->numBucktes)
    {
        return ;
    }
    unsigned int bkl = ht->numBucktes;// bucket len
    for(int i = 0; i < bkl; i ++ )
    {
        if(ht->buckets[bkl] != NULL)
        {
            chain_destory(ht->buckets[i]);
            ht->buckets[i] = NULL;
        }
    }
    ht->buckets = NULL;
}
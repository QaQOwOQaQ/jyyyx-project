#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <hash_table.h>

#define NUM_BUCKETS (1024)
#define MAX_NAME_LENGTH (64)
#define TEST_PERSION_COUNT (10000)

typedef struct person_t {
    int sno;
    char cname[MAX_NAME_LENGTH];
} Person;

// you cant declare as:
// ...(const Person *p1, const Person *p2)
// because the type of Person* and void* is different
// so compile will think those two function is different
// if you pass this function to that which use void
// it will wrong!
static boolean person_cmp(const void *p1, const void *p2)
{
    if(p1 == NULL || p2 == NULL)    
    {
        // don't gorget to check NULL
        return FALSE;
    }
    if(((Person*)p1)->sno == ((Person*)p2)->sno)  return TRUE;
    return FALSE;
}

int main()
{
    Person p[TEST_PERSION_COUNT];
    char buf[MAX_NAME_LENGTH];
    Hashtable *ht = hashtable_init(NUM_BUCKETS);
    for(int i = 0; i < TEST_PERSION_COUNT; i ++ )
    {
        sprintf(buf, "NAME-%d", i);
        // if you pursue high performance
        // you should avoid using like strcpy, memcpy... functions
        // which write or read memory
        strcpy(p[i].cname, buf);
        p[i].sno = i;
        hashtable_insert(ht, &p[i], sizeof(Person));
    }
    srand(time(NULL));
    for(int i = 0; i < 30; i ++ )
    {
        int idx = rand() % TEST_PERSION_COUNT;
        void *tmp = NULL;
        tmp = hashtable_find(ht, &p[idx], sizeof(Person), person_cmp);
        if(tmp)
        {
            printf("find success:[%0d %s] [%2lu times]\n", p[idx].sno, p[idx].cname, (unsigned long)tmp);
        }
        else 
        {
            printf("find fault:[%d %s]\n", p[idx].sno, p[idx].cname);
        }
    }
    
    hashtable_destory(ht);
    return 0;
}
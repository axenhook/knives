#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../hashtab.h"

typedef struct test_hashtab_node
{
    unsigned long long key;       // 
    unsigned long long value;       // 
    hashtab_node_t hnode; // ÔÚhashtabÖÐµÇ¼Ç
} test_hashtab_node_t;


unsigned int test_hash_func(hashtab_t *h, void *key)
{
    unsigned long long id = (unsigned long long)key;

    return id % h->slots_num;
}

int hash_compare_key(hashtab_t *h, void *key1, void *value)
{
    unsigned long long id1 = (unsigned long long)key1;
    test_hashtab_node_t *v = (test_hashtab_node_t *)value;

    if (id1 > v->key)
        return 1;
    else if (id1 == v->key)
        return 0;
    else 
        return -1;
}

void hash_print(void *value)
{
    test_hashtab_node_t *v = (test_hashtab_node_t *)value;
    
    printf("(%llu,%llu)", v->key, v->value);
}



int hash_print2(void *value, void *arg)
{
    test_hashtab_node_t *v = (test_hashtab_node_t *)value;

    printf("(%llu,%llu)", v->key, v->value);

    return 0;
}

#define offsetof(TYPE, MEMBER)	((size_t)&((TYPE *)0)->MEMBER)

int main(int argc, char *argv[])
{
    hashtab_t *h;
    hashtab_info_t stat;
    test_hashtab_node_t node[100];
    test_hashtab_node_t *hnode;

    h = hashtab_create(test_hash_func, hash_compare_key, 4, offsetof(test_hashtab_node_t, hnode));
    assert(h != NULL);

    hnode = node;
    
    hnode->key = 10; hnode->value = hnode->key+100;
    hashtab_insert(h, (void *)hnode->key, hnode); hnode++;
    hnode->key = 11; hnode->value = hnode->key+100;
    hashtab_insert(h, (void *)hnode->key, hnode); hnode++;
    hnode->key = 12; hnode->value = hnode->key+100;
    hashtab_insert(h, (void *)hnode->key, hnode); hnode++;
    hnode->key = 14; hnode->value = hnode->key+100;
    hashtab_insert(h, (void *)hnode->key, hnode); hnode++;
    hnode->key = 15; hnode->value = hnode->key+100;
    hashtab_insert(h, (void *)hnode->key, hnode); hnode++;
    hnode->key = 16; hnode->value = hnode->key+100;
    hashtab_insert(h, (void *)hnode->key, hnode); hnode++;
    hnode->key = 18; hnode->value = hnode->key+100;
    hashtab_insert(h, (void *)hnode->key, hnode); hnode++;


    assert(((test_hashtab_node_t *)hashtab_search(h, (void *)10))->value == 10 + 100);
    assert(((test_hashtab_node_t *)hashtab_search(h, (void *)11))->value == 11 + 100);
    assert(((test_hashtab_node_t *)hashtab_search(h, (void *)12))->value == 12 + 100);
    assert(((test_hashtab_node_t *)hashtab_search(h, (void *)14))->value == 14 + 100);
    assert(((test_hashtab_node_t *)hashtab_search(h, (void *)15))->value == 15 + 100);
    assert(((test_hashtab_node_t *)hashtab_search(h, (void *)16))->value == 16 + 100);
    assert(((test_hashtab_node_t *)hashtab_search(h, (void *)18))->value == 18 + 100);
    assert(hashtab_search(h, (void *)9) == NULL);
    assert(hashtab_search(h, (void *)17) == NULL);
    assert(hashtab_search(h, (void *)13) == NULL);

    hashtab_print(h, hash_print);

    // 
    printf("\n");
    hashtab_map(h, hash_print2, NULL);
    printf("\n");

    hashtab_stat(h, &stat);
    assert(stat.max_chain_len == 3);
    assert(stat.slots_used == 3);


    assert(((test_hashtab_node_t *)hashtab_delete(h, (void *)11))->value == 11 + 100);
    assert(((test_hashtab_node_t *)hashtab_delete(h, (void *)15))->value == 15 + 100);
    assert(((test_hashtab_node_t *)hashtab_delete(h, (void *)10))->value == 10 + 100);
    assert(hashtab_delete(h, (void *)11) == NULL);
    assert(hashtab_delete(h, (void *)15) == NULL);
    assert(hashtab_delete(h, (void *)10) == NULL);

    printf("\n");
    hashtab_map(h, hash_print2, NULL);
    printf("\n");
    
    hashtab_stat(h, &stat);
    assert(stat.max_chain_len == 2);
    assert(stat.slots_used == 2);

    hnode->key = 10; hnode->value = hnode->key+100;
    hashtab_insert(h, (void *)hnode->key, hnode); hnode++;
    hnode->key = 11; hnode->value = hnode->key+100;
    hashtab_insert(h, (void *)hnode->key, hnode); hnode++;
    hnode->key = 15; hnode->value = hnode->key+100;
    hashtab_insert(h, (void *)hnode->key, hnode); hnode++;
    assert(((test_hashtab_node_t *)hashtab_search(h, (void *)11))->value == 11 + 100);
    assert(((test_hashtab_node_t *)hashtab_search(h, (void *)15))->value == 15 + 100);
    assert(((test_hashtab_node_t *)hashtab_search(h, (void *)10))->value == 10 + 100);

    printf("\n");
    hashtab_map(h, hash_print2, NULL);
    printf("\n");

    hashtab_stat(h, &stat);
    assert(stat.max_chain_len == 3);
    assert(stat.slots_used == 3);

    hashtab_destroy(h);

    return 0;
}




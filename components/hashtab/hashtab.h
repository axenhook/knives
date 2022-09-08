

#ifndef __HASHTAB_H__
#define __HASHTAB_H__

#define HASHTAB_ERROR_EXIST          2

typedef struct hashtab_node hashtab_node_t;

struct hashtab_node
{
    //void *key;
    //void *value;
    hashtab_node_t *next;
};

typedef struct hashtab hashtab_t;

struct hashtab
{
    hashtab_node_t **htable;           /* hash table */
    unsigned int slots_num;                                         /* number of slots in hash table */
    unsigned int key_num;                                         /* number of elements in hash table */
    unsigned int offset; //  hash node���û��ṹ���е�ƫ��
    unsigned int (*keyhash)(hashtab_t *h, void *key);            /* hash function */
    int (*keycmp)(hashtab_t *h, void *key, void *value);   /* key comparison function */
};


// ����hash��
hashtab_t *hashtab_create(unsigned int (*keyhash)(hashtab_t *h, void *key),
                          int (*keycmp)(hashtab_t *h, void *key, void *value),
                          unsigned int slots_num, unsigned int offset);

// ��hash���в���key,value
int hashtab_insert(hashtab_t *h, void *key, void *value);

// ��hash����ɾ��ָ��key
void *hashtab_delete(hashtab_t *h, void *key);  

// ����key������value
void *hashtab_search(hashtab_t *h, void *key);

// ����hash��
void hashtab_destroy(hashtab_t *h);


// ��hash���е�����key,value����apply����
int hashtab_map(hashtab_t *h, int (*apply)(void *value, void *arg), void *args);  

// hash��ͳ����Ϣ
typedef struct hashtab_info {  
    unsigned long slots_used;  
    unsigned long max_chain_len;  
} hashtab_info_t;  

// ͳ��hash����ʹ���˶��ٸ�slot��slot�������������
void hashtab_stat(hashtab_t *h, hashtab_info_t *info);  

// ��ӡhash����slot��key,value��Ϣ
void hashtab_print(hashtab_t *h, void (*print)(void *value));  

// ��hash����ժ����һ��key�����������key��dat
void *hashtab_pop_first(hashtab_t *h);

#endif


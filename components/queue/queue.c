
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>

#include "queue.h"

queue_t *queue_create(int size)
{
    queue_t *q = NULL;

    q = (queue_t *)malloc(sizeof(queue_t));
    if (NULL == q)
    {
        return NULL;
    }

    q->memb = (void **)malloc(sizeof(void *) * size);
    if (NULL == q->memb)
    {
        free(q);
        return NULL;
    }

    q->head = 0;
    q->tail = 0;
    q->cnt = 0;
    q->size = size;

    return q;
}

int queue_push(queue_t *q, void *memb)
{
    assert(q);

    if (q->cnt == q->size)
    {
        return -ERR_QUEUE_FULL;
    }

    q->memb[q->tail++] = memb;
    q->tail %= q->size;
    q->cnt++;

    return 0;
}

int queue_pop(queue_t *q, void **memb)
{
    assert(q);

    if (q->cnt == 0)
    {
        return -ERR_QUEUE_EMPTY;
    }

    *memb = q->memb[q->head++];
    q->head %= q->size;
    q->cnt--;

    return 0;
}

int queue_get_size(queue_t *q)
{
    assert(q);

    return q->cnt;
}

void queue_destroy(queue_t *q)
{
    assert(q);

    free(q->memb);
    free(q);
}


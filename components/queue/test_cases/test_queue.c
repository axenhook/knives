#include <stdio.h>
#include <stddef.h>
#include <assert.h>

#include "../queue.h"


int main(int argc, char *argv[])
{
#define Q_SIZE 7
#define MEMB   90
        
    void *q = NULL;
    long push_msg = 0;
    void *pop_msg = NULL;

    assert((q = queue_create(Q_SIZE)) != NULL);

    push_msg = MEMB;
    assert(queue_push(q, (void *)push_msg) == 0); push_msg++;
    assert(queue_push(q, (void *)push_msg) == 0); push_msg++;
    assert(queue_push(q, (void *)push_msg) == 0); push_msg++;
    assert(queue_push(q, (void *)push_msg) == 0); push_msg++;
    assert(queue_push(q, (void *)push_msg) == 0); push_msg++;
    assert(queue_push(q, (void *)push_msg) == 0); push_msg++;
    assert(queue_push(q, (void *)push_msg) == 0); push_msg++;
    assert(queue_push(q, (void *)push_msg) == -ERR_QUEUE_FULL);
    assert(queue_get_size(q) == Q_SIZE);

    assert(queue_pop(q, &pop_msg) == 0); assert((long)pop_msg == MEMB);
    assert(queue_pop(q, &pop_msg) == 0); assert((long)pop_msg == MEMB+1);
    assert(queue_pop(q, &pop_msg) == 0); assert((long)pop_msg == MEMB+2);
    assert(queue_pop(q, &pop_msg) == 0); assert((long)pop_msg == MEMB+3);
    assert(queue_pop(q, &pop_msg) == 0); assert((long)pop_msg == MEMB+4);
    assert(queue_pop(q, &pop_msg) == 0); assert((long)pop_msg == MEMB+5);
    assert(queue_pop(q, &pop_msg) == 0); assert((long)pop_msg == MEMB+6);
    assert(queue_pop(q, &pop_msg) == -ERR_QUEUE_EMPTY);
    assert(queue_get_size(q) == 0);
    
    assert(queue_push(q, (void *)push_msg) == 0); push_msg++;
    assert(queue_push(q, (void *)push_msg) == 0); push_msg++;
    assert(queue_push(q, (void *)push_msg) == 0); push_msg++;
    
    assert(queue_pop(q, &pop_msg) == 0); assert((long)pop_msg == MEMB+7);
    assert(queue_pop(q, &pop_msg) == 0); assert((long)pop_msg == MEMB+8);
    assert(queue_pop(q, &pop_msg) == 0); assert((long)pop_msg == MEMB+9);
    assert(queue_pop(q, &pop_msg) == -ERR_QUEUE_EMPTY);
    assert(queue_get_size(q) == 0);

    queue_destroy(q);

    return 0;
}


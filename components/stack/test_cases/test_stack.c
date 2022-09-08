#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#include "../stack.h"

int main(int argc, char *argv[])
{
#define S_SIZE 7
#define MEMB   90
        
    void *q = NULL;
    long push_msg = 0;
    void *pop_msg = NULL;

    assert((q = stack_create(S_SIZE)) != NULL);

    push_msg = MEMB;
    assert(stack_push(q, (void *)push_msg) == 0); push_msg++;
    assert(stack_push(q, (void *)push_msg) == 0); push_msg++;
    assert(stack_push(q, (void *)push_msg) == 0); push_msg++;
    assert(stack_push(q, (void *)push_msg) == 0); push_msg++;
    assert(stack_push(q, (void *)push_msg) == 0); push_msg++;
    assert(stack_push(q, (void *)push_msg) == 0); push_msg++;
    assert(stack_push(q, (void *)push_msg) == 0); push_msg++;
    assert(stack_push(q, (void *)push_msg) == -ERR_STACK_FULL);
    assert(stack_get_size(q) == S_SIZE);

    assert(stack_pop(q, &pop_msg) == 0); assert((long)pop_msg == MEMB+6);
    assert(stack_pop(q, &pop_msg) == 0); assert((long)pop_msg == MEMB+5);
    assert(stack_pop(q, &pop_msg) == 0); assert((long)pop_msg == MEMB+4);
    assert(stack_pop(q, &pop_msg) == 0); assert((long)pop_msg == MEMB+3);
    assert(stack_pop(q, &pop_msg) == 0); assert((long)pop_msg == MEMB+2);
    assert(stack_pop(q, &pop_msg) == 0); assert((long)pop_msg == MEMB+1);
    assert(stack_pop(q, &pop_msg) == 0); assert((long)pop_msg == MEMB+0);
    assert(stack_pop(q, &pop_msg) == -ERR_STACK_EMPTY);
    assert(stack_get_size(q) == 0);
    
    assert(stack_push(q, (void *)push_msg) == 0); push_msg++;
    assert(stack_push(q, (void *)push_msg) == 0); push_msg++;
    assert(stack_push(q, (void *)push_msg) == 0); push_msg++;
    
    assert(stack_pop(q, &pop_msg) == 0); assert((long)pop_msg == MEMB+9);
    assert(stack_pop(q, &pop_msg) == 0); assert((long)pop_msg == MEMB+8);
    assert(stack_pop(q, &pop_msg) == 0); assert((long)pop_msg == MEMB+7);
    assert(stack_pop(q, &pop_msg) == -ERR_STACK_EMPTY);
    assert(stack_get_size(q) == 0);

    stack_destroy(q);

    return 0;
}


#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#include "../stack.h"


stack_t *stack_create(int size)
{
    stack_t *s = NULL;

    s = malloc(sizeof(stack_t));
    if (NULL == s)
    {
        return NULL;
    }

    s->memb = malloc(sizeof(void*) * size);
    if (NULL == s->memb)
    {
        free(s);
        return NULL;
    }

    s->num = 0;
    s->max = size;

    return s;
}

int stack_push(stack_t *s, void *memb)
{
    assert(s);
    assert(memb);

    if (s->num >= s->max)
    {
        return -ERR_STACK_FULL;
    }

    s->memb[s->num++] = memb;

    return 0;
}

int stack_pop(stack_t *s, void **memb)
{
    assert(s);
    assert(memb);

    if (s->num <= 0)
    {
        return -ERR_STACK_EMPTY;
    }

    *memb = s->memb[--s->num];

    return 0;
}

int stack_get_size(stack_t *s)
{
    assert(s);

    return s->num;
}

void stack_destroy(stack_t *s)
{
    assert(s);

    free(s->memb);
    free(s);
}

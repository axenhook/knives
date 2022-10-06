
#include <assert.h>
#include <stddef.h>

#include "ring_buffer.h"

void ring_buffer_reset(ring_buffer_t *rb)
{
    rb->head = 0;
    rb->tail = 0;
    rb->length = 0;

    return;
}

int ring_buffer_write(ring_buffer_t *rb, rb_member_t member)
{
    assert(rb != NULL);

    if (rb->length >= RING_BUFFER_SIZE)
    {
        return -ERR_RING_BUFFER_FULL;
    }

    rb->member[rb->tail] = member;

    //smp_wmb(); // SPSC queue should enable

    if (++rb->tail >= RING_BUFFER_SIZE)
    {
        rb->tail = 0;
    }

    rb->length++;

    return 0;
}

int ring_buffer_read(ring_buffer_t *rb, rb_member_t *member)
{
    assert(rb != NULL);
    assert(member != NULL);

    if (rb->length == 0)
    {
        return -ERR_RING_BUFFER_EMPTY;
    }

    *member = rb->member[rb->head];
    
    //smp_mb(); // SPSC queue should enable

    if (++rb->head >= RING_BUFFER_SIZE)
    {
        rb->head = 0;
    }

    rb->length--;

    return 0;
}

int ring_buffer_length(ring_buffer_t *rb)
{
    assert(rb != NULL);

    return rb->length;
}

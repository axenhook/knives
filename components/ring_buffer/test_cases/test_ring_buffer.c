#include <stdio.h>
#include <stddef.h>
#include <assert.h>

#include "../ring_buffer.h"


int main(int argc, char *argv[])
{
    ring_buffer_t rb;
    int member = 0;

    ring_buffer_reset(&rb);
    
    for (int i = 0; i < RING_BUFFER_SIZE; i++)
    {
        assert(ring_buffer_write(&rb, i) == 0);
    }

    assert(ring_buffer_length(&rb) == RING_BUFFER_SIZE);
    assert(ring_buffer_write(&rb, member) == -ERR_RING_BUFFER_FULL);
    assert(ring_buffer_length(&rb) == RING_BUFFER_SIZE);
    
    for (int i = 0; i < RING_BUFFER_SIZE; i++)
    {
        assert(ring_buffer_read(&rb, &member) == 0);
        assert(member == i);
    }

    assert(ring_buffer_length(&rb) == 0);
    assert(ring_buffer_read(&rb, &member) == -ERR_RING_BUFFER_EMPTY);
    assert(ring_buffer_length(&rb) == 0);
    
    return 0;
}


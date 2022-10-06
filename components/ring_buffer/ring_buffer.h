
#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__


#ifdef __cplusplus
extern "C" {
#endif

#define RING_BUFFER_SIZE   1024

typedef enum tagERR_RING_BUFFER_E
{
    ERR_RING_BUFFER_FULL = 100,
    ERR_RING_BUFFER_EMPTY,
} ERR_RING_BUFFER_E;


typedef int rb_member_t;

typedef struct ring_buffer
{
    rb_member_t member[RING_BUFFER_SIZE];
    int head;  
    int tail;
    int length; 
} ring_buffer_t;

void ring_buffer_reset(ring_buffer_t *rb);
int ring_buffer_write(ring_buffer_t *rb, rb_member_t member);
int ring_buffer_read(ring_buffer_t *rb, rb_member_t *member);
int ring_buffer_length(ring_buffer_t *rb);


#ifdef __cplusplus
}
#endif

#endif

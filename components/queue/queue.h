#ifndef __QUEUE_H__
#define __QUEUE_H__


#ifdef __cplusplus
extern "C" {
#endif /* End of __cplusplus */

typedef enum tagQUEUE_ERROR_CODE_E
{
    ERR_QUEUE_FULL=1,
    ERR_QUEUE_EMPTY,
} QUEUE_ERROR_CODE_E; /* End of tagQUEUE_ERROR_CODE_E */


typedef struct
{
    void **memb;
    int head;
    int tail;
    int cnt;
    int size;
} queue_t; /* End of queue_t */

extern queue_t *queue_create(int size);
extern int queue_push(queue_t *q, void *memb);
extern int queue_pop(queue_t *q, void **memb);
extern int queue_get_size(queue_t *q);
extern void queue_destroy(queue_t *q);

#ifdef __cplusplus
}
#endif /* End of __cplusplus */

#endif /* End of __QUEUE_H__ */



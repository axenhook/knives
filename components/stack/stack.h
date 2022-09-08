#ifndef __STACK_H__
#define __STACK_H__

#ifdef __cplusplus
extern "C" {
#endif /* End of __cplusplus */

typedef enum tagSTACK_ERROR_CODE_E
{
    ERR_STACK_FULL = 1,
    ERR_STACK_EMPTY,
} STACK_ERROR_CODE_E; /* End of tagSTACK_ERROR_CODE_E */

typedef struct
{
    void **memb;
    int num;
    int max;
} stack_t; /* End of stack_t */

extern stack_t *stack_create(int size);
extern int stack_push(stack_t *s, void *memb);
extern int stack_pop(stack_t *s, void **memb);
extern int stack_get_size(stack_t *s);
extern void stack_destroy(stack_t *s);

#ifdef __cplusplus
}
#endif /* End of __cplusplus */


#endif /* End of __STACK_H__ */



#ifndef _BOUNDED_BUFFER_H
#define _BOUNDED_BUFFER_H
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

/* do not use any global variables in this file */

struct bounded_buffer
{
    struct bounded_buffer *next_node;
    void *data;
    sem_t *mutex;
    sem_t *full;
    sem_t *empty;

    /* define all necessary variables here */
    struct bounded_buffer *head;
    struct bounded_buffer *tail;
};

/* do not change the following function definitions */

/* Initialize a buffer; size is the max number of items in the buffer*/
void bounded_buffer_init(struct bounded_buffer *buffer, int size);

/** Add item to the tail of the buffer. If the buffer is full, wait
 * till the buffer is not full. This function should be thread-safe. */
void bounded_buffer_push(struct bounded_buffer *buffer, void *item);

/** Remove an item from the head of the buffer. If the buffer is empty,
 * wait till the buffer is not empty. Return the removed item.
 * This function should be thread-safe. */
void *bounded_buffer_pop(struct bounded_buffer *buffer);

/* Release the buffer */
void bounded_buffer_destroy(struct bounded_buffer *buffer);

#endif

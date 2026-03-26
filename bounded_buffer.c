#include "bounded_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>

/*Do not use any global variables for implementation*/

void bounded_buffer_init(struct bounded_buffer *buffer, int size)
{
    sem_t *mutex = malloc(sizeof(sem_t));
    sem_t *full = malloc(sizeof(sem_t));
    sem_t *empty = malloc(sizeof(sem_t));
    sem_init(mutex, 0, 1);
    sem_init(full, 0, 0);
    sem_init(empty, 0, size);

    struct bounded_buffer *current_node = buffer;
    for (int i = 0; i < size - 1; i++)
    {
        struct bounded_buffer *next_node = malloc(sizeof(struct bounded_buffer));
        current_node->data = NULL;
        current_node->next_node = next_node;
        current_node->mutex = mutex;
        current_node->full = full;
        current_node->empty = empty;
        current_node = next_node;
    }
    current_node->data = NULL;
    current_node->next_node = buffer;
    current_node->mutex = mutex;
    current_node->full = full;
    current_node->empty = empty;

    buffer->head = buffer;
    buffer->tail = buffer;
}

void bounded_buffer_push(struct bounded_buffer *buffer, void *item)
{
    sem_wait(buffer->empty);
    sem_wait(buffer->mutex);

    if (buffer->tail->data != NULL)
        printf("error: buffer is full\n");
    buffer->tail->data = item;
    buffer->tail = buffer->tail->next_node;

    sem_post(buffer->mutex);
    sem_post(buffer->full);
}

void *bounded_buffer_pop(struct bounded_buffer *buffer)
{
    sem_wait(buffer->full);
    sem_wait(buffer->mutex);

    if (buffer->head->data == NULL)
        printf("error: buffer is empty\n");
    void *item = buffer->head->data;
    buffer->head->data = NULL;
    buffer->head = buffer->head->next_node;

    sem_post(buffer->mutex);
    sem_post(buffer->empty);
    return item;
}

void bounded_buffer_destroy(struct bounded_buffer *buffer)
{
    sem_destroy(buffer->mutex);
    sem_destroy(buffer->full);
    sem_destroy(buffer->empty);
    free(buffer->mutex);
    free(buffer->full);
    free(buffer->empty);

    struct bounded_buffer *current = buffer->next_node;
    while (current != buffer)
    {
        struct bounded_buffer *next = current->next_node;
        free(current);
        current = next;
    }
}

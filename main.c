#include "bounded_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

struct bounded_buffer queue;

void *producer(void *ptr);
void *consumer(void *ptr);

int main()
{
    /* initialize the queue */
    bounded_buffer_init(&queue, 5);

    /* create producer and consumer threads */
    pthread_t threads[5];

    for (int i = 0; i < 5; i++)
    {
        int *id = malloc(sizeof(int));
        *id = i;
        if (i < 3)
        {
            pthread_create(&threads[i], NULL, &producer, (void *)id);
        }
        else
        {
            pthread_create(&threads[i], NULL, &consumer, (void *)id);
        }
    }

    sleep(5);
    exit(0);
}

/* this is the function executed by the producer thread.
   It should generate a number of messages and push them into the queue */
void *producer(void *ptr)
{
    int id = *((int *)ptr);
    free(ptr);
    // push message "thread (id) (digit 0-9)"
    for (int i = 0; i < 10; i++)
    {
        char *msg = malloc(64);
        sprintf(msg, "thread %d %d", id, i);
        printf("thread %d pushing value %d", id, i);
        bounded_buffer_push(&queue, msg);
    }
    return NULL;
}

/* this is the function executed by the consumer thread.
   It should pop messages from the queue and print them */
void *consumer(void *ptr)
{
    int id = *((int *)ptr);
    free(ptr);
    // keep popping and printing messages using the format (id) thread popped message (message)
    while (1)
    {
        char *msg = (char *)bounded_buffer_pop(&queue);
        printf("%d thread popped message %s\n", id, msg);
        free(msg);
    }

    return NULL;
}

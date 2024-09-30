#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

static void *thread_function(void *arg);

int main(void)
{
    pthread_t threads[NUM_THREADS];
    int       thread_args[NUM_THREADS];
    int       created_threads;
    int       ret;

    created_threads = 0;
    ret             = EXIT_SUCCESS;

    for(int i = 0; i < NUM_THREADS; i++)
    {
        int result;

        thread_args[i] = i;
        result         = pthread_create(&threads[i], NULL, thread_function, &thread_args[i]);

        if(result != 0)
        {
            perror("pthread_create");
            ret = EXIT_FAILURE;
            break;
        }

        created_threads++;
    }

    for(int j = 0; j < created_threads; j++)
    {
        int result;

        result = pthread_join(threads[j], NULL);

        if(result != 0)
        {
            perror("pthread_join");
            ret = EXIT_FAILURE;
        }
    }

    printf("All threads that were created have finished execution.\n");

    return ret;
}

static void *thread_function(void *arg)
{
    int value;
    int thread_num;

    thread_num = *(int *)arg;
    value      = *(int *)arg;
    printf("Thread %d is running. Argument passed: %d\n", thread_num, value);

    return NULL;
}

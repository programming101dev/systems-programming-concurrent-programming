#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static void *thread_function(void *arg);

int main(void)
{
    const int argument = 42;
    pthread_t thread;
    int       ret = EXIT_SUCCESS;
    int       result;

    result = pthread_create(&thread, NULL, thread_function, (void *)&argument);

    if(result != 0)
    {
        perror("pthread_create");
        ret = EXIT_FAILURE;
        goto done;
    }

    result = pthread_join(thread, NULL);

    if(result != 0)
    {
        perror("pthread_join");
        ret = EXIT_FAILURE;
        goto done;
    }

    printf("Thread has finished execution.\n");

done:
    return ret;
}

static void *thread_function(void *arg)
{
    int value;

    value = *(int *)arg;
    printf("Thread is running. Argument passed: %d\n", value);

    return NULL;
}

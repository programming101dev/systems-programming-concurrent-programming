#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void *thread_function(void *arg);

int main(void)
{
    pthread_attr_t  attr;
    pthread_t       thread;
    int             result;
    int             return_code;
    struct timespec ts;

    return_code = EXIT_SUCCESS;
    result      = pthread_attr_init(&attr);

    if(result != 0)
    {
        perror("pthread_attr_init failed");
        return_code = EXIT_FAILURE;
        goto done;
    }

    result = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if(result != 0)
    {
        perror("pthread_attr_setdetachstate failed");
        return_code = EXIT_FAILURE;
        goto cleanup_attr;
    }

    result = pthread_create(&thread, &attr, thread_function, NULL);

    if(result != 0)
    {
        perror("pthread_create failed");
        return_code = EXIT_FAILURE;
        goto cleanup_attr;
    }

    ts.tv_sec  = 1;
    ts.tv_nsec = 0;
    result     = nanosleep(&ts, NULL);

    if(result != 0)
    {
        perror("nanosleep failed");
        return EXIT_FAILURE;
    }

cleanup_attr:
    result = pthread_attr_destroy(&attr);

    if(result != 0)
    {
        perror("pthread_attr_destroy failed");
        return_code = EXIT_FAILURE;
    }

done:
    printf("Exiting from main\n");

    return return_code;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static void *thread_function(void *arg)
{
    struct timespec ts;

    printf("Detached thread is running\n");
    ts.tv_sec  = 2;
    ts.tv_nsec = 0;
    nanosleep(&ts, NULL);
    printf("Detached thread finished after main thread exit.\n");

    return NULL;
}

#pragma GCC diagnostic pop

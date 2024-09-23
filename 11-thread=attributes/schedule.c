#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static void *thread_function(void *arg);

int main(void)
{
    pthread_attr_t     attr;
    pthread_t          thread;
    struct sched_param param;
    int                result;
    int                return_code = EXIT_SUCCESS;

    result = pthread_attr_init(&attr);

    if(result != 0)
    {
        perror("pthread_attr_init failed");
        return_code = EXIT_FAILURE;
        goto done;
    }

    result = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);

    if(result != 0)
    {
        perror("pthread_attr_setschedpolicy failed");
        return_code = EXIT_FAILURE;
        goto cleanup_attr;
    }

    param.sched_priority = sched_get_priority_max(SCHED_FIFO);
    result               = pthread_attr_setschedparam(&attr, &param);

    if(result != 0)
    {
        perror("pthread_attr_setschedparam failed");
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

    result = pthread_join(thread, NULL);

    if(result != 0)
    {
        perror("pthread_join failed");
        return_code = EXIT_FAILURE;
    }

cleanup_attr:
    result = pthread_attr_destroy(&attr);

    if(result != 0)
    {
        perror("pthread_attr_destroy failed");
        return_code = EXIT_FAILURE;
    }

done:
    return return_code;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// cppcheck-suppress constParameterCallback
static void *thread_function(void *arg)
{
    printf("High priority thread is running\n");
    return NULL;
}

#pragma GCC diagnostic pop

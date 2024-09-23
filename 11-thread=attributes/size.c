#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static void *thread_function(void *arg);

int main(void)
{
    pthread_attr_t attr;
    pthread_t      thread;
    size_t         stack_size;
    int            result;
    int            return_code;

    stack_size  = (size_t)(2) * 1024 * 1024;
    return_code = EXIT_SUCCESS;
    result      = pthread_attr_init(&attr);

    if(result != 0)
    {
        perror("pthread_attr_init failed");
        return_code = EXIT_FAILURE;
        goto done;
    }

    result = pthread_attr_setstacksize(&attr, stack_size);

    if(result != 0)
    {
        perror("pthread_attr_setstacksize failed");
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
    printf("Thread is running\n");
    return NULL;
}

#pragma GCC diagnostic pop

#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

static void *increment_resource(void *arg);

int main(void)
{
    pthread_t  threads[10];
    atomic_int shared_resource;
    int        return_code;

    shared_resource = 0;
    return_code     = EXIT_SUCCESS;

    for(int i = 0; i < 10; i++)
    {
        int result;

        result = pthread_create(&threads[i], NULL, increment_resource, &shared_resource);

        if(result != 0)
        {
            fprintf(stderr, "Error creating thread %d: %d\n", i, result);
            return_code = EXIT_FAILURE;
            goto done;
        }
    }

    for(int i = 0; i < 10; i++)
    {
        int result;

        result = pthread_join(threads[i], NULL);

        if(result != 0)
        {
            fprintf(stderr, "Error joining thread %d: %d\n", i, result);
            return_code = EXIT_FAILURE;
            goto done;
        }
    }

    printf("All threads have finished execution. Final shared resource value: %d\n", atomic_load(&shared_resource));

done:
    return return_code;
}

static void *increment_resource(void *arg)
{
    atomic_int *shared_resource;
    pthread_t   thread_id;
    int         new_value;

    shared_resource = (atomic_int *)arg;
    thread_id       = pthread_self();
    new_value       = atomic_fetch_add(shared_resource, 1) + 1;
    printf("Thread %lu incremented shared resource to %d\n", (unsigned long)thread_id, new_value);

    return NULL;
}

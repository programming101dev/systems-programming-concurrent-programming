#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    pthread_mutex_t *lock;
    int             *shared_resource;
} thread_data_t;

static int   initialize_mutex(pthread_mutex_t *lock);
static void  destroy_mutex(pthread_mutex_t *lock);
static void *increment_resource(void *arg);

int main(void)
{
    pthread_t       threads[10];
    pthread_mutex_t lock;
    int             shared_resource;
    thread_data_t   thread_data;
    int             result;
    int             return_code;
    int             created_threads;

    shared_resource = 0;
    return_code     = EXIT_SUCCESS;
    created_threads = 0;
    result          = initialize_mutex(&lock);

    if(result != 0)
    {
        return_code = EXIT_FAILURE;
        goto done;
    }

    thread_data.lock            = &lock;
    thread_data.shared_resource = &shared_resource;

    for(int i = 0; i < 10; i++)
    {
        result = pthread_create(&threads[i], NULL, increment_resource, &thread_data);

        if(result != 0)
        {
            perror("pthread_create");
            return_code = EXIT_FAILURE;
            break;
        }

        created_threads++;
    }

    for(int i = 0; i < created_threads; i++)
    {
        result = pthread_join(threads[i], NULL);

        if(result != 0)
        {
            perror("pthread_join");
            return_code = EXIT_FAILURE;
        }
    }

    printf("All threads have finished execution. Final shared resource value: %d\n", shared_resource);
    destroy_mutex(&lock);

done:
    return return_code;
}

static int initialize_mutex(pthread_mutex_t *lock)
{
    int result;
    int ret;

    result = pthread_mutex_init(lock, NULL);

    if(result != 0)
    {
        perror("pthread_mutex_init");
        ret = -1;
    }
    else
    {
        ret = 0;
    }

    return ret;
}

static void destroy_mutex(pthread_mutex_t *lock)
{
    int result;

    result = pthread_mutex_destroy(lock);

    if(result != 0)
    {
        perror("pthread_mutex_destroy");
    }
}

static void *increment_resource(void *arg)
{
    thread_data_t   *data;
    pthread_mutex_t *lock;
    int             *shared_resource;
    pthread_t        thread_id;
    int              result;

    data            = (thread_data_t *)arg;
    lock            = data->lock;
    shared_resource = data->shared_resource;
    thread_id       = pthread_self();
    result          = pthread_mutex_lock(lock);

    if(result != 0)
    {
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }

    (*shared_resource)++;
    printf("Thread %lu modified the shared resource. New value: %d\n", (unsigned long)thread_id, *shared_resource);
    result = pthread_mutex_unlock(lock);

    if(result != 0)
    {
        perror("pthread_mutex_unlock");
        pthread_exit(NULL);
    }

    return NULL;
}

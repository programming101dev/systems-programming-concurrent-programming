#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct thread_data
{
    pthread_cond_t  *cond;
    pthread_mutex_t *lock;
    int             *shared_resource;
};

static int   initialize_cond_var(pthread_cond_t *cond, pthread_mutex_t *lock);
static void *wait_for_condition(void *arg);
static void *signal_condition(void *arg);

int main(void)
{
    pthread_t          thread1;
    pthread_t          thread2;
    pthread_cond_t     cond;
    pthread_mutex_t    lock;
    int                shared_resource;
    struct thread_data thread_data;
    int                return_code = EXIT_SUCCESS;
    int                result;
    struct timespec    ts;

    shared_resource = 0;

    printf("Initializing condition variable and mutex...\n");
    result = initialize_cond_var(&cond, &lock);

    if(result != 0)
    {
        return_code = EXIT_FAILURE;
        goto cleanup_cond_mutex;
    }

    printf("Condition variable and mutex initialized.\n");
    thread_data.cond            = &cond;
    thread_data.lock            = &lock;
    thread_data.shared_resource = &shared_resource;
    printf("Creating thread 1 to wait for the condition...\n");
    result = pthread_create(&thread1, NULL, wait_for_condition, &thread_data);

    if(result != 0)
    {
        perror("pthread_create (thread1)");
        return_code = EXIT_FAILURE;
        goto cleanup_cond_mutex;
    }

    ts.tv_sec  = 0;
    ts.tv_nsec = 500000000L;
    nanosleep(&ts, NULL);

    printf("Creating thread 2 to signal the condition...\n");
    result = pthread_create(&thread2, NULL, signal_condition, &thread_data);

    if(result != 0)
    {
        perror("pthread_create (thread2)");
        return_code = EXIT_FAILURE;
        goto cleanup_thread1;
    }

    printf("Waiting for thread 1 to finish...\n");
    result = pthread_join(thread1, NULL);

    if(result != 0)
    {
        perror("pthread_join (thread1)");
        return_code = EXIT_FAILURE;
        goto cleanup_thread2;
    }

    printf("Waiting for thread 2 to finish...\n");
    result = pthread_join(thread2, NULL);

    if(result != 0)
    {
        perror("pthread_join (thread2)");
        return_code = EXIT_FAILURE;
    }

cleanup_thread2:
    if(result != 0)
    {
        pthread_join(thread2, NULL);
    }

cleanup_thread1:
    if(result != 0)
    {
        pthread_join(thread1, NULL);
    }

cleanup_cond_mutex:
    printf("Cleaning up: destroying condition variable and mutex...\n");
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&lock);

    return return_code;
}

int initialize_cond_var(pthread_cond_t *cond, pthread_mutex_t *lock)
{
    int result;

    result = pthread_cond_init(cond, NULL);

    if(result != 0)
    {
        perror("pthread_cond_init");
        return -1;
    }

    result = pthread_mutex_init(lock, NULL);

    if(result != 0)
    {
        perror("pthread_mutex_init");
        pthread_cond_destroy(cond);
        return -1;
    }

    return 0;
}

void *wait_for_condition(void *arg)
{
    struct thread_data *data;
    int                 result;

    data = (struct thread_data *)arg;
    printf("Thread 1: Waiting for the condition...\n");
    result = pthread_mutex_lock(data->lock);

    if(result != 0)
    {
        perror("pthread_mutex_lock");
        return NULL;
    }

    while(*(data->shared_resource) == 0)
    {
        printf("Thread 1: Condition not met, waiting...\n");
        result = pthread_cond_wait(data->cond, data->lock);

        if(result != 0)
        {
            perror("pthread_cond_wait");
            pthread_mutex_unlock(data->lock);
            return NULL;
        }
    }

    printf("Thread 1: Condition met, proceeding...\n");
    result = pthread_mutex_unlock(data->lock);

    if(result != 0)
    {
        perror("pthread_mutex_unlock");
        return NULL;
    }

    return NULL;
}

void *signal_condition(void *arg)
{
    struct thread_data *data;
    int                 result;

    data = (struct thread_data *)arg;
    printf("Thread 2: Signaling the condition...\n");
    result = pthread_mutex_lock(data->lock);

    if(result != 0)
    {
        perror("pthread_mutex_lock");
        return NULL;
    }

    *(data->shared_resource) = 1;
    result                   = pthread_cond_signal(data->cond);

    if(result != 0)
    {
        perror("pthread_cond_signal");
        pthread_mutex_unlock(data->lock);
        return NULL;
    }

    printf("Thread 2: Condition signaled.\n");
    result = pthread_mutex_unlock(data->lock);

    if(result != 0)
    {
        perror("pthread_mutex_unlock");
        return NULL;
    }

    return NULL;
}

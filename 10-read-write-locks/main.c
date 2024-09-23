#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

struct thread_data
{
    pthread_rwlock_t *rwlock;
    int              *shared_resource;
    int               value_to_write;
};

static int   initialize_rwlock(pthread_rwlock_t *rwlock);
static void  destroy_rwlock(pthread_rwlock_t *rwlock);
static void *read_shared_resource(void *arg);
static void *write_shared_resource(void *arg);

int main(void)
{
    const int          write_values[NUM_THREADS] = {1, 2, 3, 4, 5};
    pthread_t          readers[NUM_THREADS];
    pthread_t          writers[NUM_THREADS];
    pthread_rwlock_t   rwlock;
    int                shared_resource;
    struct thread_data reader_data[NUM_THREADS];
    struct thread_data writer_data[NUM_THREADS];
    int                result;
    int                created_readers;
    int                created_writers;
    int                return_code;

    shared_resource = 0;
    created_readers = 0;
    created_writers = 0;
    return_code     = EXIT_SUCCESS;

    result = initialize_rwlock(&rwlock);

    if(result != 0)
    {
        return_code = EXIT_FAILURE;
        goto done;
    }

    for(int i = 0; i < NUM_THREADS; i++)
    {
        reader_data[i].rwlock          = &rwlock;
        reader_data[i].shared_resource = &shared_resource;
        result                         = pthread_create(&readers[i], NULL, read_shared_resource, &reader_data[i]);

        if(result != 0)
        {
            perror("pthread_create (reader)");
            return_code = EXIT_FAILURE;
            break;
        }

        created_readers++;

        writer_data[i].rwlock          = &rwlock;
        writer_data[i].shared_resource = &shared_resource;
        writer_data[i].value_to_write  = write_values[i];
        result                         = pthread_create(&writers[i], NULL, write_shared_resource, &writer_data[i]);

        if(result != 0)
        {
            perror("pthread_create (writer)");
            return_code = EXIT_FAILURE;
            break;
        }

        created_writers++;
    }

    for(int i = 0; i < created_readers; i++)
    {
        result = pthread_join(readers[i], NULL);

        if(result != 0)
        {
            perror("pthread_join (reader)");
            return_code = EXIT_FAILURE;
        }
    }

    for(int i = 0; i < created_writers; i++)
    {
        result = pthread_join(writers[i], NULL);

        if(result != 0)
        {
            perror("pthread_join (writer)");
            return_code = EXIT_FAILURE;
        }
    }

    destroy_rwlock(&rwlock);

done:
    return return_code;
}

static int initialize_rwlock(pthread_rwlock_t *rwlock)
{
    int result;
    int ret;

    result = pthread_rwlock_init(rwlock, NULL);

    if(result != 0)
    {
        perror("pthread_rwlock_init");
        ret = -1;
    }
    else
    {
        ret = 0;
    }

    return ret;
}

static void destroy_rwlock(pthread_rwlock_t *rwlock)
{
    int result;

    result = pthread_rwlock_destroy(rwlock);

    if(result != 0)
    {
        perror("pthread_rwlock_destroy");
    }
}

static void *read_shared_resource(void *arg)
{
    struct thread_data *data;
    pthread_rwlock_t   *rwlock;
    const int          *shared_resource;
    int                 result;

    data            = (struct thread_data *)arg;
    rwlock          = data->rwlock;
    shared_resource = data->shared_resource;
    result          = pthread_rwlock_rdlock(rwlock);

    if(result != 0)
    {
        perror("pthread_rwlock_rdlock");
        pthread_exit(NULL);
    }

    printf("Reading shared resource: %d\n", *shared_resource);
    result = pthread_rwlock_unlock(rwlock);

    if(result != 0)
    {
        perror("pthread_rwlock_unlock");
        pthread_exit(NULL);
    }

    return NULL;
}

static void *write_shared_resource(void *arg)
{
    struct thread_data *data            = (struct thread_data *)arg;
    pthread_rwlock_t   *rwlock          = data->rwlock;
    int                *shared_resource = data->shared_resource;
    int                 value_to_write  = data->value_to_write;
    int                 result;

    result = pthread_rwlock_wrlock(rwlock);

    if(result != 0)
    {
        perror("pthread_rwlock_wrlock");
        pthread_exit(NULL);
    }

    *shared_resource = value_to_write;
    printf("Writing to shared resource: %d\n", *shared_resource);
    result = pthread_rwlock_unlock(rwlock);

    if(result != 0)
    {
        perror("pthread_rwlock_unlock");
        pthread_exit(NULL);
    }

    return NULL;
}

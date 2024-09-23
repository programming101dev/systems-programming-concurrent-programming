#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    pthread_attr_t attr;
    int            result;
    int            return_code = EXIT_SUCCESS;

    result = pthread_attr_init(&attr);

    if(result != 0)
    {
        perror("pthread_attr_init failed");
        return_code = EXIT_FAILURE;
        goto done;
    }

    // Set and use attributes here

    result = pthread_attr_destroy(&attr);

    if(result != 0)
    {
        perror("pthread_attr_destroy failed");
        return_code = EXIT_FAILURE;
    }

done:
    return return_code;
}

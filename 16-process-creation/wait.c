#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int   status;
    pid_t pid;

    status = EXIT_SUCCESS;
    pid    = fork();

    if(pid == -1)
    {
        perror("fork failed");
        status = EXIT_FAILURE;
        goto cleanup;
    }

    if(pid == 0)
    {
        printf("Child process\n");
        exit(42);    // NOLINT(concurrency-mt-unsafe,-warnings-as-errors)
    }
    else
    {
        int   child_status;
        pid_t wait_result;

        wait_result = wait(&child_status);

        if(wait_result == -1)
        {
            perror("wait failed");
            status = EXIT_FAILURE;
            goto cleanup;
        }

        if(WIFEXITED(child_status))
        {
            printf("Child exited with status %d\n", WEXITSTATUS(child_status));
        }
        else
        {
            printf("Child did not exit normally\n");
            status = EXIT_FAILURE;
        }
    }

cleanup:
    return status;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;
    int   status = EXIT_SUCCESS;
    int   child_status;
    pid_t wait_result;

    // Fork the process
    pid = fork();
    if(pid == -1)
    {
        perror("fork failed");
        status = EXIT_FAILURE;
        goto cleanup;
    }

    if(pid == 0)
    {
        // Child process
        printf("Child process\n");
        _exit(42);
    }
    else
    {
        // Parent process
        wait_result = waitpid(pid, &child_status, 0);
        if(wait_result == -1)
        {
            perror("waitpid failed");
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

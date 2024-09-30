#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;
    int   status = EXIT_SUCCESS;

    pid = fork();

    if(pid == -1)
    {
        perror("fork failed");
        status = EXIT_FAILURE;
        goto cleanup;
    }

    if(pid == 0)
    {
        printf("This is the child process\n");
    }
    else
    {
        printf("This is the parent process, child PID: %d\n", pid);
    }

cleanup:
    return status;
}

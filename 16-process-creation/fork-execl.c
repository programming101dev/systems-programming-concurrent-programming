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
        int execl_result = execl("/bin/ls", "ls", NULL);
        if(execl_result == -1)
        {
            perror("execl failed");
            status = EXIT_FAILURE;
            goto cleanup;
        }
    }
    else
    {
        // Parent process
        printf("Parent process, child PID: %d\n", pid);

        // Wait for the child process to finish
        wait_result = waitpid(pid, &child_status, 0);
        if(wait_result == -1)
        {
            perror("waitpid failed");
            status = EXIT_FAILURE;
            goto cleanup;
        }

        // Check if the child exited normally
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int process_child(void);
static int process_parent(pid_t pid);

int main(void)
{
    int   status;
    pid_t pid;

    pid = fork();

    if(pid == -1)
    {
        perror("fork failed");
        status = EXIT_FAILURE;
        goto done;
    }

    if(pid == 0)
    {
        status = process_child();
    }
    else
    {
        status = process_parent(pid);
    }

done:
    return status;
}

static int process_child(void)
{
    execl("/bin/ls", "ls", NULL);
    perror("execl failed");

    return EXIT_FAILURE;
}

static int process_parent(pid_t pid)
{
    pid_t wait_result;
    int   child_status;
    int   ret;

    printf("Parent process, child PID: %d\n", pid);
    wait_result = waitpid(pid, &child_status, 0);

    if(wait_result == -1)
    {
        perror("waitpid failed");
        ret = EXIT_FAILURE;
        goto done;
    }

    if(WIFEXITED(child_status))
    {
        printf("Child exited with status %d\n", WEXITSTATUS(child_status));
    }
    else
    {
        printf("Child did not exit normally\n");
        ret = EXIT_FAILURE;
        goto done;
    }

    ret = EXIT_SUCCESS;

done:
    return ret;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef void (*parent_func_t)(pid_t, void *);
typedef void (*child_func_t)(void *);

static void handle_fork(parent_func_t parent_func, void *parent_data, child_func_t child_func, void *child_data);
static void example_parent_func(pid_t child_pid, void *data);
static void example_child_func(void *data);

int main(void)
{
    handle_fork(example_parent_func, NULL, example_child_func, NULL);

    return EXIT_SUCCESS;
}

static void handle_fork(parent_func_t parent_func, void *parent_data, child_func_t child_func, void *child_data)
{
    pid_t pid;
    int   status;

    pid = fork();

    if(pid == -1)
    {
        perror("fork failed");
        return;    // No exit in parent, return control to caller
    }

    if(pid == 0)
    {
        if(child_func != NULL)
        {
            child_func(child_data);
        }

        exit(EXIT_SUCCESS);    // NOLINT(concurrency-mt-unsafe,-warnings-as-errors)
    }
    else if(parent_func != NULL)
    {
        parent_func(pid, parent_data);
    }

    do
    {
        pid_t wpid;

        wpid = waitpid(pid,
                       &status,
                       WUNTRACED
#ifdef WCONTINUED
                           | WCONTINUED
#endif
        );

        if(wpid == -1)
        {
            perror("waitpid");
            return;
        }

        if(WIFEXITED(status))
        {
            printf("Child exited, status=%d\n", WEXITSTATUS(status));
        }
        else if(WIFSIGNALED(status))
        {
            printf("Child killed (signal %d)\n", WTERMSIG(status));
        }
        else if(WIFSTOPPED(status))
        {
            printf("Child stopped (signal %d)\n", WSTOPSIG(status));
#ifdef WCONTINUED
        }
        else if(WIFCONTINUED(status))
        {
            printf("Child continued\n");
#endif
        }
        else
        {
            printf("Unexpected status (0x%x)\n", status);
        }
    } while(!WIFEXITED(status) && !WIFSIGNALED(status));
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// cppcheck-suppress constParameterCallback
static void example_parent_func(pid_t child_pid, void *data)
{
    printf("Parent: handling child process with PID %d\n", child_pid);
}

#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static void example_child_func(void *data)
{
    execl("/bin/ls", "ls", NULL);
    perror("execl failed");
    exit(EXIT_FAILURE);    // NOLINT(concurrency-mt-unsafe,-warnings-as-errors)
}

#pragma GCC diagnostic pop

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;
    pid_t ppid;

    pid  = getpid();
    ppid = getppid();
    printf("Process ID: %d\n", pid);
    printf("Parent Process ID: %d\n", ppid);

    return EXIT_SUCCESS;
}

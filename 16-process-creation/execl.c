#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int result;

    printf("Before execl\n");
    result = execl("/bin/ls", "ls", NULL);
    printf("After execl %d\n", result);
    perror("execl failed");

    return EXIT_FAILURE;
}

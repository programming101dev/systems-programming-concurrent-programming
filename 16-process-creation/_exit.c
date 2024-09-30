#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    printf("Process is exiting with a status\n");
    _exit(EXIT_FAILURE);
}

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("Process is terminating normally\n");
    exit(EXIT_SUCCESS);    // NOLINT(concurrency-mt-unsafe,-warnings-as-errors)
}

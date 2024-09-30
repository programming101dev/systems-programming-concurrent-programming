#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

void       display_resource_limits(int resource, const char *resource_name);
static int set_resource_limit(int resource, rlim_t new_soft_limit, rlim_t new_hard_limit);

int main(void)
{
    const rlim_t CPU_SOFT_LIMIT       = 10;
    const rlim_t CPU_HARD_LIMIT       = 20;
    const rlim_t FILE_SIZE_SOFT_LIMIT = (rlim_t)(1024) * 1024;
    const rlim_t FILE_SIZE_HARD_LIMIT = (rlim_t)(2) * 1024 * 1024;
    int status;
    int result;

    status = EXIT_SUCCESS;
    display_resource_limits(RLIMIT_CPU, "CPU Time");
    display_resource_limits(RLIMIT_FSIZE, "File Size");

    result = set_resource_limit(RLIMIT_CPU, CPU_SOFT_LIMIT, CPU_HARD_LIMIT);

    if(result != 0)
    {
        status = EXIT_FAILURE;
    }

    result = set_resource_limit(RLIMIT_FSIZE, FILE_SIZE_SOFT_LIMIT, FILE_SIZE_HARD_LIMIT);

    if(result != 0)
    {
        status = EXIT_FAILURE;
    }

    display_resource_limits(RLIMIT_CPU, "CPU Time");
    display_resource_limits(RLIMIT_FSIZE, "File Size");

    return status;
}

void display_resource_limits(int resource, const char *resource_name)
{
    struct rlimit limit;
    int           result;

    result = getrlimit(resource, &limit);

    if(result == -1)
    {
        perror("getrlimit failed");
        return;
    }

    printf("%s: Soft limit = %llu, Hard limit = %llu\n", resource_name, (unsigned long long)limit.rlim_cur, (unsigned long long)limit.rlim_max);
}

static int set_resource_limit(int resource, rlim_t new_soft_limit, rlim_t new_hard_limit)
{
    struct rlimit limit;
    int           result;

    if(new_soft_limit > new_hard_limit)
    {
        fprintf(stderr, "Error: Soft limit cannot exceed hard limit.\n");
        return -1;
    }

    limit.rlim_cur = new_soft_limit;
    limit.rlim_max = new_hard_limit;
    result = setrlimit(resource, &limit);

    if(result == -1)
    {
        perror("setrlimit failed");
        return -1;
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

void       display_resource_limits(int resource, const char *resource_name);
static int set_resource_limit(int resource, rlim_t new_soft_limit, rlim_t new_hard_limit);

int main(void)
{
    const rlim_t CPU_SOFT_LIMIT       = 10;    // CPU soft limit in seconds
    const rlim_t CPU_HARD_LIMIT       = 20;    // CPU hard limit in seconds
    const rlim_t FILE_SIZE_SOFT_LIMIT = (rlim_t)(1024) * 1024;    // File size soft limit in bytes (1 MB)
    const rlim_t FILE_SIZE_HARD_LIMIT = (rlim_t)(2) * 1024 * 1024;    // File size hard limit in bytes (2 MB)

    int status = EXIT_SUCCESS;
    int result;

    // Display current limits for CPU time and file size
    display_resource_limits(RLIMIT_CPU, "CPU Time");
    display_resource_limits(RLIMIT_FSIZE, "File Size");

    // Set new limits for CPU time and file size
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

    // Display updated limits
    display_resource_limits(RLIMIT_CPU, "CPU Time");
    display_resource_limits(RLIMIT_FSIZE, "File Size");

    return status;
}

// Function to display current resource limits
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

// Function to set a new resource limit
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

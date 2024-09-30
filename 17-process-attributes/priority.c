#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int get_nice_value(void);
static int set_nice_value(int increment);

int main(void)
{
    int current_nice = get_nice_value();
    if(current_nice == -1)
    {
        return EXIT_FAILURE;
    }
    printf("Current nice value: %d\n", current_nice);

    if(set_nice_value(5) != 0)
    {    // Increase nice value by 5
        return EXIT_FAILURE;
    }

    printf("New nice value: %d\n", get_nice_value());

    return EXIT_SUCCESS;
}

// Function to get the current nice value of a process
static int get_nice_value(void)
{
    int nice_value = nice(0);    // Get current nice value

    if(nice_value == -1 && errno != 0)
    {
        perror("Failed to get current nice value");
        return -1;
    }
    return nice_value;
}

// Function to set a new nice value for a process
static int set_nice_value(int increment)
{
    int result;

    errno  = 0;
    result = nice(increment);

    if(result == -1 && errno != 0)
    {
        perror("Failed to set nice value");
        return -1;
    }

    return 0;
}

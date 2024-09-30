#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    uid_t uid;
    uid_t euid;
    gid_t gid;
    gid_t egid;

    uid  = getuid();
    euid = geteuid();
    gid  = getgid();
    egid = getegid();
    printf("User ID: %d, Effective User ID: %d\n", uid, euid);
    printf("Group ID: %d, Effective Group ID: %d\n", gid, egid);

    return EXIT_SUCCESS;
}

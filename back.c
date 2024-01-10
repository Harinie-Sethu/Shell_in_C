#include "headers.h"
#include "fore.h"

// Background process
void back_process(char **command, int *pids)
{
    int *status;
    int pid = fork();

    if (pid != 0)
    {
        *pids = pid;
    }

    if (pid == 0)
    {
        setpgid(0, 0);
        int stat = execvp(*command, command);

        if (stat == -1)
            printf("Error: No such command found.\n");

        return;
    }

    // Parent does not wait

    return;
}
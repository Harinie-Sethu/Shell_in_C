#include "headers.h"
#include "fore.h"

// typedef struct background back;

// struct background
// {
//     char *name;
//     int pid;
//     int job_num;
//     int run;
// };

// extern int fg_pid;
// extern int shell;
// extern back proc[100];
// extern int p;
// extern char fg_name[100];

// If the process is a foreground process
void fore_process(char **command, int *rtime)
{
    int *status;
    int pid = fork();

    // Start time of child execution
    time_t start = time(NULL);

    if (pid != 0)
    {
        fg_pid = pid;
    }

    if (pid == 0)
    {
        setpgid(0, 0);
        int stat = execvp(*command, command);

        if (stat == -1)
            printf("Error: No such command found.\n");

        return;
    }

    // Parent waits for child to finish
    else
        pid = wait(status);

    // End time of child execution
    time_t end = time(NULL);

    // Runtime
    *rtime = end - start;

    return;
}

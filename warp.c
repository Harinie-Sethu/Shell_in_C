#include "headers.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>
// Function to handle the cd command
// Changes directory according to given argument
void cdDotDot()
{
    chdir("..");
}

void cdDot()
{
    chdir(".");
}

void cdFile(char *path)
{
    chdir(path);
}

int cd(char **comm, char *save, char **prev, char **prev2)
{
    if (strcmp(*comm, ".") == 0)
    {
        getcwd(*prev, 100);
        cdDot;
    }
    else if (strcmp(*comm, "..") == 0)
    {
        getcwd(*prev, 100);
        cdDotDot();
    }
    else if (strcmp(*comm, "\0") == 0 || strcmp(*comm, "~") == 0)
    {
        getcwd(*prev, 100);
        cdFile(save);
    }
    else if (strcmp(*comm, "-") == 0)
    {
        getcwd(*prev2, 100);
        cdFile(*prev);
        strcpy(*prev, *prev2);
    }
    else
    {
        int dire = open(*comm, O_DIRECTORY, O_RDONLY);

        if (dire >= 0)
        {
            getcwd(*prev, 100);
            cdFile(*comm);
        }
        else
        {
            perror("Directory");
            return -1;
        }
    }

    return 0;
}
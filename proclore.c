#include "headers.h"

void convert(char HOME_DIRECTORY[], char complete_pathname[], char pathname[])
{
    int i;
    int flag = 0;
    for (i = 0; i < strlen(HOME_DIRECTORY); i++)
    {
        if (HOME_DIRECTORY[i] != complete_pathname[i])
        {
            flag = 1;
            break;
        }
    }
    if (flag == 0)
    {
        pathname[0] = '~';
        int k = 1;
        for (i = strlen(HOME_DIRECTORY); i < strlen(complete_pathname); i++)
        {
            pathname[k] = complete_pathname[i];
            k++;
        }
        pathname[k] = '\0';
    }
    else
        strcpy(pathname, complete_pathname);
}

void proclore(int pid, char HOME_DIRECTORY[], char **pt, int out)
{
    FILE *fp;
    int i;
    char path[PATH_MAX] = "/proc/";
    char copy[PATH_MAX];
    char memory[15];
    char status[5];
    char tmp1[100] = {'\0'};
    char tmp2[100] = {'\0'};
    char exePath[PATH_MAX] = {'\0'};
    char exepath[PATH_MAX] = {'\0'};
    char pgid[15] = {'\0'};

    char *pit = (char *)malloc(sizeof(char) * 50);
    strcpy(pit, *pt);

    char *print = (char *)malloc(sizeof(char) * 1000);
    strcpy(print, "");

    // Make sure this buffer is large enough
    char string_pid[12];
    snprintf(string_pid, sizeof(string_pid), "%d", pid);

    strcat(path, string_pid);
    strcpy(copy, path);
    strcat(path, "/stat");

    fp = fopen(path, "r");
    // error handling in case pid entered is invalid
    if (fp == NULL)
    {
        printf("There is no process with pid %d\n", pid);
        return;
    }
    // third word of stat file contains current status of process
    fscanf(fp, "%s %s %s", tmp1, tmp2, status);

    // Get the sixth field from /proc/<pid>/stat, which is the process group ID
    for (i = 0; i < 5; i++) // Skip the fields
        fscanf(fp, "%s", tmp1);
    fscanf(fp, "%s", pgid);

    strcpy(path, copy);
    strcat(path, "/statm");
    fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("There is no process with pid %d\n", pid);
        return;
    }
    // first word of statm file contains virtual memory used
    fscanf(fp, "%s", memory);

    strcpy(path, copy);
    strcat(path, "/exe");
    // readlink reads the symlink exe which is pointed to executable path of process
    readlink(path, exepath, sizeof(exepath));
    convert(HOME_DIRECTORY, exepath, exePath);

    // Append process information to the 'print' buffer
    sprintf(print + strlen(print), "pid: %d\n", pid);
    sprintf(print + strlen(print), "Process Status: %s\n", status);
    sprintf(print + strlen(print), "Process Group ID: %s\n", pgid);
    sprintf(print + strlen(print), "Virtual Memory: %s\n", memory);
    sprintf(print + strlen(print), "Executable Path: %s\n", exePath);
    if (out == 0)
    {
        printf("%s", print);
    }
    else if (out == 1)
    {
        int fp = open(pit, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (fp == -1)
        {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        int original = dup(STDOUT_FILENO);
        dup2(fp, STDOUT_FILENO);
        close(fp);

        printf("%s\n", print);

        dup2(original, STDOUT_FILENO);
    }
    else if (out == 2)
    {
        int fp = open(pit, O_CREAT | O_WRONLY | O_APPEND, 0644);
        if (fp == -1)
        {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        int original = dup(STDOUT_FILENO);
        dup2(fp, STDOUT_FILENO);
        close(fp);

        printf("%s\n", print);

        dup2(original, STDOUT_FILENO);
    }

    return;
}
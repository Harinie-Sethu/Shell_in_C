#include "headers.h"

void onlyls(char dir[], char **pt, int out)
{
    char tempdir[100000];
    char *path = (char *)malloc(sizeof(char) * 50);
    strcpy(path, *pt);

    char *print = (char *)malloc(sizeof(char) * 1000);
    strcpy(print, "");

    if (dir[0] == '~')
    {
        strcpy(tempdir, HOME_DIRECTORY);
        strcat(tempdir, &dir[1]);
        strcpy(dir, tempdir);
    }

    DIR *d;
    struct dirent **read;
    LL x = 0, count = scandir(dir, &read, NULL, alphasort);
    if (count < 0)
    {
        perror("Error: file or directory does not exist\n");
    }
    else
    {
        while (x < count)
        {
            if (read[x]->d_name[0] == '.')
            {
                x++;
                continue;
            }
            else
            {
                // Append the directory entry to the 'print' buffer
                strcat(print, read[x]->d_name);
                strcat(print, "\n");
                x++;
            }
        }
    }

    if (out == 0)
    {
        printf("%s", print);
    }
    else if (out == 1)
    {
        int fp = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
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
        int fp = open(path, O_CREAT | O_WRONLY | O_APPEND, 0644);
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
}

void lsa(char dir[], char **pt, int out)
{
    char tempdir[100000];
    char *path = (char *)malloc(sizeof(char) * 50);
    strcpy(path, *pt);

    char *print = (char *)malloc(sizeof(char) * 1000);
    strcpy(print, "");

    if (dir[0] == '~')
    {
        strcpy(tempdir, HOME_DIRECTORY);
        strcat(tempdir, &dir[1]);
        strcpy(dir, tempdir);
    }
    DIR *d;
    struct dirent **read;
    LL x = 0, no = scandir(dir, &read, NULL, alphasort);

    if (no < 0)
    {
        perror("Error: no such file or directory exists\n");
    }
    else
    {
        while (x < no)
        {
            // Append the directory entry to the 'print' buffer
            strcat(print, read[x]->d_name);
            strcat(print, "\n");
            x++;
        }
    }

    if (out == 0)
    {
        printf("%s", print);
    }
    else if (out == 1)
    {
        int fp = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
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
        int fp = open(path, O_CREAT | O_WRONLY | O_APPEND, 0644);
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
}

void lsl(char dir[], int flag, char **pt, int out)
{
    char tempdir[100000];
    char *path = (char *)malloc(sizeof(char) * 50);
    strcpy(path, *pt);

    char *print = (char *)malloc(sizeof(char) * 1000);
    strcpy(print, "");

    if (dir[0] == '~')
    {
        strcpy(tempdir, HOME_DIRECTORY);
        strcat(tempdir, &dir[1]);
        strcpy(dir, tempdir);
    }

    DIR *d;
    struct dirent **read;
    LL x = 0, no = scandir(dir, &read, NULL, alphasort);

    if (no < 0)
    {
        perror("Error: No such file or directory\n");
    }
    else
    {
        while (x < no)
        {
            char path[10000];
            struct stat s;
            strcpy(path, read[x++]->d_name);

            if (path[0] == '.' && flag == 0)
                continue;
            else
            {
                char per[20] = "", own[10], ug[10], date[30], time[6], path2[10000];
                LL hardlinks, size;
                strcpy(path2, "");
                strcat(path2, dir);
                strcat(path2, "/");
                strcat(path2, path);

                stat(path2, &s);

                hardlinks = s.st_nlink;

                struct passwd *pd;
                pd = getpwuid(s.st_uid);
                struct group *gr;
                gr = getgrgid(s.st_gid);

                size = s.st_size;

                if (S_ISDIR(s.st_mode) ? strcat(per, "d") : strcat(per, "-"))
                    ;
                if ((s.st_mode & S_IRUSR) ? strcat(per, "r") : strcat(per, "-"))
                    ;
                if ((s.st_mode & S_IWUSR) ? strcat(per, "w") : strcat(per, "-"))
                    ;
                if ((s.st_mode & S_IXUSR) ? strcat(per, "x") : strcat(per, "-"))
                    ;
                if ((s.st_mode & S_IRGRP) ? strcat(per, "r") : strcat(per, "-"))
                    ;
                if ((s.st_mode & S_IWGRP) ? strcat(per, "w") : strcat(per, "-"))
                    ;
                if ((s.st_mode & S_IXGRP) ? strcat(per, "x") : strcat(per, "-"))
                    ;
                if ((s.st_mode & S_IROTH) ? strcat(per, "r") : strcat(per, "-"))
                    ;
                if ((s.st_mode & S_IWOTH) ? strcat(per, "w") : strcat(per, "-"))
                    ;
                if ((s.st_mode & S_IXOTH) ? strcat(per, "x") : strcat(per, "-"))
                    ;

                strftime(date, 24, "%Y-%m-%d    %H:%M", localtime(&(s.st_ctime)));

                // Append the formatted output to the 'print' buffer
                strcat(print, per);
                char temp[50];
                sprintf(temp, "%lld", hardlinks);
                strcat(print, temp);
                strcat(print, pd->pw_name);
                strcat(print, gr->gr_name);
                sprintf(temp, "%lld", size);
                strcat(print, temp);
                strcat(print, date);
                strcat(print, path);
                strcat(print, "\n");
            }
        }
    }

    if (out == 0)
    {
        printf("%s", print);
    }
    else if (out == 1)
    {
        int fp = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
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
        int fp = open(path, O_CREAT | O_WRONLY | O_APPEND, 0644);
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
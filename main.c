#include "headers.h"
#include "prompt.c"
#include "warp.c"
#include "peak.c"
#include "proclore.c"
#include "history.c"
#include "ctrlZ.c"
#include "iman.c"
#include "neon.c"
// Declare a global variable to store the original terminal settings
// struct termios original_termios;



int tokenizeCommand(char *command, char *tokens[], LL *numTokens)
{
    char *saveptr;
    char *token = strtok_r(command, " ", &saveptr);
    *numTokens = 0;

    while (token != NULL)
    {
        tokens[*numTokens] = token;
        (*numTokens)++;
        token = strtok_r(NULL, " ", &saveptr);
    }
    tokens[*numTokens] = NULL;
    int noOo = *numTokens;
    return noOo;
}

int main()
{
    // Gets name of user
    char *user_name = (char *)malloc(sizeof(char) * 1000);
    getlogin_r(user_name, sizeof(user_name));

    // Gets name of host system
    char host[1000];
    char host_save[1000];
    gethostname(host, sizeof(host));
    strcpy(host_save, host);

    // get path
    char path[1000];
    getcwd(path, 1000);
    char curr_dir[1000];
    strcpy(curr_dir, path); // Set the main directory as the current directory

    // // After getting the current path
    getcwd(HOME_DIRECTORY, sizeof(HOME_DIRECTORY)); // getting the directory where shell.c is present

    // Saves the previous working directory
    char *prev1 = (char *)malloc(sizeof(char) * 100);
    getcwd(prev1, 100);

    // Saves the previous working directory
    char *prev2 = (char *)malloc(sizeof(char) * 100);
    getcwd(prev2, 100);

    // Saves the previous prompt
    char *save2 = (char *)malloc(sizeof(char) * 100);

    // Disable input buffering
    // disable_input_buffering();

    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = sigtstp_handler;
    sigaction(SIGTSTP, &sa, NULL);

    if (signal(SIGINT, sigint_handler) == SIG_ERR)
    {
        perror("Error setting up SIGINT handler");
        return 1;
    }
    if (signal(SIGTSTP, sigtstp_handler) == SIG_ERR)
    {
        perror("Error setting up SIGTSTP handler");
        return 1;
    }

    // Keep accepting commands
    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt(user_name, host);
        char input[4096];
        // Disable line buffering for stdin
        setbuf(stdin, NULL);

        // Set up signal handlers
        signal(SIGINT, sigint_handler);
        signal(SIGTSTP, sigtstp_handler);

        fgets(input, 4096, stdin);

        ssize_t bytes_read = read(STDIN_FILENO, input, sizeof(input));

        // Check for Ctrl+D (EOF)
        if (bytes_read == 0)
        {
            write(STDOUT_FILENO, "\nCtrl+D (EOF) signal received. Exiting the shell.\n", 52);
            break;
        }

        // 1) PROCESS INPUT
        //  newline prompt
        if (strcmp(input, "\n") == 0)
            continue;

        // tokenize input
        char *commands[1000];
        LL numCommands = 0;

        char *input_copy = strdup(input); // Create a copy of input
        input_copy[strlen(input_copy) - 1] = '\0';

        char *input_ptr = input_copy; // Pointer for strtok_r
        char *cmd = strtok_r(input_ptr, "&;", &input_ptr);
        while (cmd != NULL && numCommands < MAX_COMMANDS - 1)
        {
            commands[numCommands] = cmd;
            numCommands++;
            cmd = strtok_r(NULL, "&;", &input_ptr);
        }
        commands[numCommands] = NULL;

        // for (int i = 0; i < numCommands; i++)
        //     printf("%s \t", commands[i]);
        // printf("done\n");
        char *tok = (char *)malloc(sizeof(char) * 100);
        int numPar = 0;
        char *par[1000];
        LL tk = 0;

        for (LL i = 0; i < numCommands; i++)
        {
            char com_copy[100];
            strcpy(com_copy, commands[i]);

            char *cmdtok = commands[i];
            char *tokens[1000]; // Array to store tokens within a command
            LL k = 0;
            int noOo = tokenizeCommand(cmdtok, tokens, &k);

            // curr wcd for other commands
            char cwd[200000];
            getcwd(cwd, sizeof(cwd));

            if (strcmp(tokens[0], "pastevents") != 0) // WRITING INTO HISTORY
            {
                int q = his_check(tokens[0]);
            }

            if (strcmp(tokens[0], "warp") == 0)
            {
                if (tokens[1] == NULL || strcmp(tokens[1], "~") == 0)
                {
                    strcpy(curr_dir, host);
                    strcpy(host, host_save);
                    cdFile(HOME_DIRECTORY);
                }
                else
                {
                    LL f = cd(&tokens[1], HOME_DIRECTORY, &prev1, &prev2);

                    if (strcmp(tokens[1], ".") == 0)
                    {
                        strcpy(curr_dir, host);
                        // strcpy(host, host);
                    }

                    else
                    {
                        if (strcmp(tokens[1], "-") == 0)
                        {
                            strcpy(save2, host);
                            strcpy(host, curr_dir);
                            strcpy(curr_dir, save2);
                        }

                        else if (strcmp(tokens[1], "..") == 0)
                        {
                            char *wd = (char *)malloc(sizeof(char) * 100);
                            getcwd(wd, 100);

                            char *temp = (char *)malloc(sizeof(char) * 100);

                            strcpy(curr_dir, host);

                            if (strcmp(path, wd) > 0)
                            {
                                strcpy(temp, host_save);
                                strcat(temp, wd);
                                strcpy(host, temp);
                            }

                            else
                            {
                                temp = strrchr(host, '/');
                                *temp = '\0';
                            }
                        }

                        else
                        {
                            if (f >= 0)
                            {
                                strcpy(curr_dir, host);
                                strcat(host, "/");
                                strcat(host, tokens[1]);
                            }
                        }
                    }
                }
            }

            else if (strcmp(tokens[0], "peek") == 0)
            {
                int out = 0;
                char *nullness = (char *)malloc(sizeof(char) * 50);
                strcpy(nullness, "");

                if (tokens[1] == NULL)
                    // if (tokens[1][0] == '\0')
                    onlyls(cwd, &tokens[0], out);

                else if (tokens[2] == NULL)
                {
                    if (strcmp(tokens[1], "-a") == 0)
                        lsa(cwd, &tokens[0], out);
                    else if (strcmp(tokens[1], "-l") == 0)
                        lsl(cwd, 0, &tokens[0], out);
                    else if (strcmp(tokens[1], "-la") == 0)
                        lsl(cwd, 1, &tokens[0], out);
                    else if (strcmp(tokens[1], "-al") == 0)
                        lsl(cwd, 1, &tokens[0], out);
                    else
                        onlyls(tokens[1], &tokens[1], out);
                }

                else if (tokens[3] == NULL)
                {
                    if (strcmp(tokens[1], "-a") == 0 && strcmp(tokens[2], "-l") != 0)
                        lsa(tokens[2], &tokens[0], out);
                    else if (strcmp(tokens[1], "-l") == 0 && strcmp(tokens[2], "-a") != 0)
                        lsl(tokens[2], 0, &tokens[0], out);
                    else if (strcmp(tokens[1], "-a") == 0 && strcmp(tokens[2], "-l") == 0)
                        lsa(cwd, &tokens[0], out);
                    else if (strcmp(tokens[1], "-l") == 0 && strcmp(tokens[2], "-a") == 0)
                        lsl(cwd, 0, &tokens[0], out);
                    else if (strcmp(tokens[1], "-la") == 0)
                        lsl(tokens[2], 1, &tokens[0], out);
                    else if (strcmp(tokens[1], "-al") == 0)
                        lsl(tokens[2], 1, &tokens[0], out);
                    else if (strcmp(tokens[1], ">") == 0)
                    {
                        out = 1;
                        onlyls(cwd, &tokens[2], out);
                    }
                    else if (strcmp(tokens[1], ">>") == 0)
                    {
                        out = 2;
                        onlyls(cwd, &tokens[2], out);
                    }
                }

                else if (tokens[4] == NULL)
                {
                    if (strcmp(tokens[1], "-a") == 0 && strcmp(tokens[2], "-l") == 0)
                        lsa(tokens[3], &tokens[0], out);

                    else if (strcmp(tokens[1], "-l") == 0 && strcmp(tokens[2], "-a") == 0)
                        lsl(tokens[3], 0, &tokens[0], out);

                    else if (strcmp(tokens[2], ">") == 0)
                    {
                        out = 1;
                        if (strcmp(tokens[1], "-a") == 0)
                            lsa(cwd, &tokens[3], out);
                        else if (strcmp(tokens[1], "-l") == 0)
                            lsl(cwd, 0, &tokens[3], out);
                        else if (strcmp(tokens[1], "-la") == 0)
                            lsl(cwd, 1, &tokens[3], out);
                        else if (strcmp(tokens[1], "-al") == 0)
                            lsl(cwd, 1, &tokens[3], out);
                        else
                            onlyls(tokens[1], &tokens[3], out);
                    }
                    else if (strcmp(tokens[2], ">>") == 0)
                    {
                        out = 2;
                        if (strcmp(tokens[1], "-a") == 0)
                            lsa(cwd, &tokens[3], out);
                        else if (strcmp(tokens[1], "-l") == 0)
                            lsl(cwd, 0, &tokens[3], out);
                        else if (strcmp(tokens[1], "-la") == 0)
                            lsl(cwd, 1, &tokens[3], out);
                        else if (strcmp(tokens[1], "-al") == 0)
                            lsl(cwd, 1, &tokens[3], out);
                        else
                            onlyls(tokens[1], &tokens[3], out);
                    }
                }

                else if (tokens[5] == NULL)
                {
                    if (strcmp(tokens[3], ">") == 0)
                    {
                        out = 1;
                        if (strcmp(tokens[1], "-a") == 0 && strcmp(tokens[2], "-l") != 0)
                            lsa(tokens[2], &tokens[4], out);
                        else if (strcmp(tokens[1], "-l") == 0 && strcmp(tokens[2], "-a") != 0)
                            lsl(tokens[2], 0, &tokens[4], out);
                        else if (strcmp(tokens[1], "-a") == 0 && strcmp(tokens[2], "-l") == 0)
                            lsa(cwd, &tokens[4], out);
                        else if (strcmp(tokens[1], "-l") == 0 && strcmp(tokens[2], "-a") == 0)
                            lsl(cwd, 0, &tokens[4], out);
                        else if (strcmp(tokens[1], "-la") == 0)
                            lsl(tokens[2], 1, &tokens[4], out);
                        else if (strcmp(tokens[1], "-al") == 0)
                            lsl(tokens[2], 1, &tokens[4], out);
                    }
                    else if (strcmp(tokens[3], ">>") == 0)
                    {
                        out = 2;

                        if (strcmp(tokens[1], "-a") == 0 && strcmp(tokens[2], "-l") != 0)
                            lsa(tokens[2], &tokens[4], out);
                        else if (strcmp(tokens[1], "-l") == 0 && strcmp(tokens[2], "-a") != 0)
                            lsl(tokens[2], 0, &tokens[4], out);
                        else if (strcmp(tokens[1], "-a") == 0 && strcmp(tokens[2], "-l") == 0)
                            lsa(cwd, &tokens[4], out);
                        else if (strcmp(tokens[1], "-l") == 0 && strcmp(tokens[2], "-a") == 0)
                            lsl(cwd, 0, &tokens[4], out);
                        else if (strcmp(tokens[1], "-la") == 0)
                            lsl(tokens[2], 1, &tokens[4], out);
                        else if (strcmp(tokens[1], "-al") == 0)
                            lsl(tokens[2], 1, &tokens[4], out);
                    }
                }

                else if (tokens[6] == NULL)
                {
                    if (strcmp(tokens[4], ">") == 0)
                    {
                        out = 1;
                        if (strcmp(tokens[1], "-a") == 0 && strcmp(tokens[2], "-l") == 0)
                            lsa(tokens[3], &tokens[5], out);

                        else if (strcmp(tokens[1], "-l") == 0 && strcmp(tokens[2], "-a") == 0)
                            lsl(tokens[3], 0, &tokens[5], out);
                    }
                    else if (strcmp(tokens[4], ">>") == 0)
                    {
                        out = 2;
                        if (strcmp(tokens[1], "-a") == 0 && strcmp(tokens[2], "-l") == 0)
                            lsa(tokens[3], &tokens[5], out);

                        else if (strcmp(tokens[1], "-l") == 0 && strcmp(tokens[2], "-a") == 0)
                            lsl(tokens[3], 0, &tokens[5], out);
                    }
                }
            }

            else if (strcmp(tokens[0], "echo") == 0)
            {
                int out = 0;
                char *path = (char *)malloc(sizeof(char) * 50);
                char *print = (char *)malloc(sizeof(char) * 50);
                strcpy(print, "");
                strcpy(path, "");

                if (path == NULL || print == NULL)
                {
                    perror("Memory allocation failed");
                    exit(EXIT_FAILURE);
                }

                // for (int jk = 0; jk < noOo; jk++)
                // {
                //     if (strcmp(tokens[jk], ">") == 0)
                //         out = 1;
                //     else if (strcmp(tokens[jk], ">>") == 0)
                //         out = 2;

                //     if (out == 0)
                //     {
                //         strcat(print, tokens[1]);
                //         strcat(print, " ");
                //     }
                //     else
                //     {
                //         strcat(path, tokens[jk + 1]);
                //     }
                // }

                if (tokens[2] != NULL)
                {
                    if (strcmp(tokens[2], ">") == 0)
                        out = 1;

                    else if (strcmp(tokens[2], ">>") == 0)
                        out = 2;

                    strcpy(path, tokens[3]);
                }
                strcpy(print, tokens[1]);

                if (out == 1)
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
                else
                {
                    printf("%s\n", print);
                }

                free(path);
                free(print);
            }

            else if (strcmp(tokens[0], "proclore") == 0)
            {
                int out = 0;
                int pid = getpid();

                // if (tokens[1] != NULL)
                //     proclore(atoi(tokens[1]), HOME_DIRECTORY);
                // else
                //     proclore(pid, HOME_DIRECTORY);

                if (tokens[1] == NULL)
                {
                    proclore(pid, HOME_DIRECTORY, &tokens[0], out);
                }

                else if (tokens[2] == NULL)
                {
                    proclore(atoi(tokens[1]), HOME_DIRECTORY, &tokens[0], out);
                }

                else if (tokens[3] == NULL)
                {
                    if (strcmp(tokens[1], ">") == 0)
                    {
                        out = 1;
                        proclore(pid, HOME_DIRECTORY, &tokens[2], out);
                    }
                    else if (strcmp(tokens[1], ">>") == 0)
                    {
                        out = 2;
                        proclore(pid, HOME_DIRECTORY, &tokens[2], out);
                    }
                }

                else if (tokens[4] == NULL)
                {
                    if (strcmp(tokens[1], ">") == 0)
                    {
                        out = 1;
                        proclore(atoi(tokens[1]), HOME_DIRECTORY, &tokens[2], out);
                    }
                    else if (strcmp(tokens[1], ">>") == 0)
                    {
                        out = 2;
                        proclore(atoi(tokens[1]), HOME_DIRECTORY, &tokens[2], out);
                    }
                }
            }

            else if (strcmp(tokens[0], "seek") == 0)
            {
                // printf("seeking life\n");

                // char *dir2 = (char *)malloc(sizeof(char) * 100);
                // getcwd(dir2, 100);

                // char file[100] = "\0";
                // char print[100];
                // strcpy(print, ".");

                // int seek_d = 0, seek_f = 0, seek_e = 0, out = 0;

                // if (tokens[1] == NULL)
                // {
                //     printf(".\n");
                //     discover_noflags(path, print, file);
                // }
                // else
                // {
                //     // seek -d animal
                //     if (strcmp(tokens[1], "-d") == 0)
                //         seek_d = 1;

                //     //seek -f animal
                //     else if (strcmp(tokens[1], "-f") == 0)
                //         seek_f = 1;

                //     // seek -e animal
                //     else if (strcmp(tokens[1], "-e") == 0)
                //         seek_e = 1;

                //     else
                //     {
                //         // seek animal
                //         int length = strlen(tokens[1]);
                //         tokens[length - 1] = '\0';
                //         strcpy(file, &token[1]);
                //         discover_noflags(path, print, file);
                //     }

                //     if (tokens[2] != NULL)
                //     {
                //         if (strcmp(tokens[2], "-d") == 0)
                //             seek_d = 1;

                //         else if (strcmp(tokens[2], "-f") == 0)
                //             seek_f = 1;

                //         else if (strcmp(tokens[2], "-e") == 0)
                //             seek_e = 1;

                //         if (tokens[3] != NULL)
                //         {
                //         }
                //     }
                // }
            }

            else if (strcmp(tokens[0], "pastevents") == 0)
            {
                // printf("ghosting life\n");
                history_print();
                his_check(tokens[0]);
            }

            else if (strcmp(tokens[0], "iMan") == 0)
            {
                fetchManPage(tokens[1]);
            }

            else if(strcmp(tokens[0], "neonate") == 0)
            {
                neonate(atoi(tokens[2]));
            }

            else
            {
                system(com_copy);
                // printf("%s\n", com_copy);
            }
        }
    }

    return 0;
}

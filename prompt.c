#include "headers.h"
#include "prompt.h"

void prompt(char *user_name, char *host)
{

    // strcat(user_name, cwd_path);
    // printf("\033[1;32m%s ", user_name);

    // Get the relative path with respect to the main directory
    // char relative_path[1000];
    // if (strstr(path, curr_dir) == path)
    // {
    //     strcpy(relative_path, "~>");
    //     strcat(relative_path, path + strlen(curr_dir));
    // }
    // else
    // {
    //     strcpy(relative_path, path);
    // }

    // Print the prompt in the required format
    printf("\033[1;35m%s@%s:~>", user_name, host);
    printf("\033[1;37m"); // Reset color
}


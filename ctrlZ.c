#include "headers.h"

void sigint_handler(int signo)
{
    write(STDOUT_FILENO, "\nCtrl+C (SIGINT) signal received. Exiting the shell.\n", 53);
    exit(0);
}

// Function to handle Ctrl+Z (SIGTSTP) signal
void sigtstp_handler(int signo)
{
    write(STDOUT_FILENO, "\nCtrl+Z (SIGTSTP) signal received. Stopping the shell.\n", 56);
}
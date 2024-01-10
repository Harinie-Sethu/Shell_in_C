#ifndef HEADERS_H_
#define HEADERS_H_

#include "prompt.h"
#define PATH_MAX 1000
#define MAX_TOKENS 1000
#define MAX_COMMANDS 1000
#define MAX_BUFFER_SIZE 4096
typedef long long int LL;
#define PATH_MAX_LENGTH 512

// all the header files we will ever need
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <grp.h>
#include <stdbool.h>
#include <signal.h>
#include <termios.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <termios.h>
#include <ctype.h>

#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <pwd.h>
#include <fcntl.h> // Include the header for file control options


// After getting the current path
char HOME_DIRECTORY[PATH_MAX] = {'\0'};
extern int alphasort();
#include "fore.h"

char history[10][1000];

#endif
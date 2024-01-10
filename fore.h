#ifndef _FORE_H
#define _FORE_H

// Struct which stores the name and pid of the background processes
typedef struct background back;

struct background
{
    char* name;
    int pid;
    int job_num;
    int run;
};

extern int fg_pid;
extern int shell;
extern back proc[100];
extern int p;
extern char fg_name[100];


#endif
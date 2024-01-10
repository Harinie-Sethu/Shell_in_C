#include "headers.h"

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
 */
void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

int isKeyPressed()
{
    struct timeval timeout = {0L, 0L};
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(0, &readfds);
    return select(1, &readfds, NULL, NULL, &timeout);
}

void neto(int timearg)
{
    int exitFlag = 0;

    while (1)
    {
        char keyPressed;
        if (isKeyPressed())
        {
            keyPressed = getchar();
            if (keyPressed == 'x')
            {
                // printf("Exiting neonate\n");
                exitFlag = 1;
            }
        }

        if (timearg != 0)
        {
            long long int i;
            int flag = 0;
            for (i = 0; i < timearg * 1000; i++)
            {
                if (isKeyPressed())
                {
                    keyPressed = getchar();
                    if (keyPressed == 'x')
                    {
                        // printf("Exiting neonate\n");
                        flag = 1;
                        break;
                    }
                }
                usleep(1000);
            }
            if (flag == 1 || exitFlag == 1)
                break;
        }
        else if (exitFlag == 1)
        {
            break;
        }
    }
}

void neonate(LL time)
{

    enableRawMode();

    // temporarily block signals
    sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGTSTP);
    sigaddset(&mask, SIGINT);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);

    while (1)
    {

        unsigned long long maxtime = 0;
        int maxpid = -1;

        // Open the /proc directory
        DIR *proc_dir = opendir("/proc");
        if (!proc_dir)
        {
            perror("Error opening /proc directory");
            return;
        }

        // Read directory entries
        struct dirent *entry;
        while ((entry = readdir(proc_dir)) != NULL)
        {
            // Check if the entry name is a valid PID
            if (!isdigit(entry->d_name[0]))
            {
                continue;
            }

            // Construct the path to the stat file
            char stat_path[PATH_MAX];
            snprintf(stat_path, sizeof(stat_path), "/proc/%s/stat", entry->d_name);

            // Open the stat file for reading
            FILE *stat_file = fopen(stat_path, "r");
            if (!stat_file)
            {
                perror("Failed to open stat file");
                continue;
            }

            // Read the contents of the stat file
            char line[1024];
            if (!fgets(line, sizeof(line), stat_file))
            {
                perror("Failed to read stat file");
                fclose(stat_file);
                continue;
            }

            // Parse the "frtime" value from the stat file
            unsigned long long frtime;
            if (sscanf(line, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u %*u %*d %*d %*d %*d %*d %llu", &frtime) != 1)
            {
                perror("Failed to parse frtime value");
                fclose(stat_file);
                continue;
            }

            // Compare with the current maximum "frtime"
            if (frtime > maxtime)
            {
                maxtime = frtime;
                maxpid = atoi(entry->d_name);
            }

            fclose(stat_file);
        }

        // Close the /proc directory
        closedir(proc_dir);

        // // Output the PID of the most recent process created
        if (maxpid != -1)
        {
            printf("%d\n", maxpid);
            // printf("PID of the most recent process: %d\n", maxpid);
        }
        else
        {
            printf("No valid processes found.\n");
            break;
        }
        fflush(stdout);

        // neto(time);
        if (time == 0)
        {
            char keyPressed;
            if (isKeyPressed() == 1)
            {
                keyPressed = getchar();
                if (keyPressed == 'x')
                    break;
            }
        }
        else if (time != 0)
        {
            LL intim=0, exit = 0;
            while (intim < time * 1000)
            {
                char keyPressed;
                if (isKeyPressed() == 1)
                {
                    keyPressed = getchar();
                    if (keyPressed == 'x')
                    {
                        exit = 1;
                        break;
                    }
                }
                usleep(1000);
            }

            if (exit == 1)
                break;

            
            intim++;
        }
    }

    // unblock signals
    sigprocmask(SIG_SETMASK, &oldmask, NULL);

    disableRawMode();
}
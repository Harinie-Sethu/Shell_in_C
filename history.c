#include "headers.h"

int history_start = -1, history_end = -1, history_size = 0, max_entries = 10;

void his_load(LL start, LL end)
{
    history_start = start;
    history_end = end;
}

void del()
{
    if (history_start == -1)
        printf("\nError: history is empty");

    if (history_start == history_end)
    {
        history_start = -1;
        history_end = -1;
    }
    else if (history_start == max_entries - 1)
        history_start = 0;
    else
        history_start++;
}

void wr(char newEntry[])
{
    if ((history_start == 0 && history_end == max_entries - 1) || (history_end == (history_start - 1) % (max_entries - 1)))
        del();

    else if (history_start == -1)
    {
        history_start = history_end = 0;
        strcpy(history[history_end], newEntry);
    }

    else if (history_end == max_entries - 1 && history_start != 0)
    {
        history_end = 0;
        strcpy(history[history_end], newEntry);
    }

    else
    {
        history_end++;
        strcpy(history[history_end], newEntry);
    }
}

int his_check(char newEntry[])
{
    int found = 0;
    for (LL i = 0; i < max_entries; i++)
    {
        if (strcmp(newEntry, history[i]) == 0)
        {
            found = 1;
            break;
        }
    }
    if (found == 0)
    {
        if ((max_entries + history_end - history_start + 1) % max_entries != 0)
            wr(newEntry);

        else
        {
            del();
            wr(newEntry);
        }
    }
    return found;
}

void history_print()
{
    if (history_start == history_end && history_start == -1)
        printf("No Previous Events Available in Memory\n");
    else
    {
        if (history_start <= history_end)
            for (LL i = history_start; i <= history_end; i++)
                printf("> %s\n", history[i]);

        else
        {
            for (LL i = history_start; i < max_entries; i++)
                printf("> %s\n", history[i]);

            for (LL i = 0; i <= history_end; i++)
                printf("> %s\n", history[i]);
        }
    }
}
#include "main.h"

// External Variables
extern pid_t child_pid;        // Stores process ID of the child process
extern char prompt[25];        // Stores the shell prompt string
extern char original_input[100]; // Stores the original user input before processing
Slist *head = NULL;            // Head of the linked list for background jobs

void my_handler(int signum)
{
    if (signum == SIGINT) // Handling Ctrl+C
    {
        printf("\n");

        if (child_pid > 0) // If a child process is running, send SIGINT to terminate it
        {
            kill(child_pid, SIGINT);
        }
        else if (child_pid == 0) // If no child process, simply display the prompt
        {
            printf("%s ", prompt);
            fflush(stdout);
        }
    }
    else if (signum == SIGTSTP) // Handling Ctrl+Z
    {
        printf("\n");

        if (child_pid > 0) // If a child process is running, send SIGTSTP to stop it
        {
            kill(child_pid, SIGSTOP);
            
            // Print message indicating the job has been stopped
            printf(ANSI_COLOR_RED "[%d]+  Stopped                 %s\n", child_pid, original_input);
            
            // Insert the stopped job into the background jobs list
            insert_at_last(&head, child_pid, original_input);
        }
        else if (child_pid == 0) // If no child process, simply display the prompt
        {
            printf("%s ", prompt);
            fflush(stdout);
        }
    }
}

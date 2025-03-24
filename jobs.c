#include "main.h"

extern pid_t child_pid;
extern Slist *head;
int stopped = 1;  // 1 if the job is stopped, 0 if running
void display_list(Slist **head)
{
    if (*head == NULL)
    {
        printf(ANSI_COLOR_RED "bash: jobs: no current jobs\n");
        return;
    }

    Slist *temp = *head;
    Slist *prev = NULL;
    int status;
    pid_t result;

    while (temp)
    {
        // Check if the process has completed using waitpid with WNOHANG
        result = waitpid(temp->child_pid, &status, WNOHANG);
        if (result == temp->child_pid) // Process has exited
        {
            // Remove the completed job from the list
            if (prev == NULL)
            {
                *head = temp->link;
                free(temp);
                temp = *head;
            }
            else
            {
                prev->link = temp->link;
                free(temp);
                temp = prev->link;
            }
            continue;
        }

        // Print job status (Stopped or Running)
        if (stopped)
        {
            printf(ANSI_COLOR_YELLOW "[%d]+ Stopped                 %s\n", temp->child_pid, temp->command);
        }
        else
        {
            printf(ANSI_COLOR_YELLOW "[%d]+ Running                 %s \n", temp->child_pid, temp->command);
        }

        prev = temp;
        temp = temp->link;
    }
}

void insert_at_last(Slist **head, int child_pid, char *command)
{
    Slist *new_node = (Slist *)malloc(sizeof(Slist));
    if (!new_node)
    {
        perror(ANSI_COLOR_RED "Memory allocation failed");
        return;
    }
    
    // Initialize the new job node
    new_node->child_pid = child_pid;
    strcpy(new_node->command, command);
    new_node->link = NULL;

    // If list is empty, set new node as head
    if (*head == NULL)
    {
        *head = new_node;
    }
    else
    {
        Slist *temp = *head;
        while (temp->link)
        {
            temp = temp->link;
        }
        temp->link = new_node;
    }
}

/**
 * Function: remove_job_from_list
 * ------------------------------
 * Removes a job from the list using its PID.
 *
 * Input:
 *  - `head`: Pointer to the head of the job list.
 *  - `pid`: Process ID of the job to remove.
 *
 * Output:
 *  - Removes the job from the list if found.
 */
void remove_job_from_list(Slist **head, pid_t pid)
{
    Slist *temp = *head, *prev = NULL;
    while (temp)
    {
        if (temp->child_pid == pid)
        {
            if (prev == NULL)
            {
                *head = temp->link;
            }
            else
            {
                prev->link = temp->link;
            }
            free(temp);
            return;
        }
        prev = temp;
        temp = temp->link;
    }
}

/**
 * Function: fg
 * ------------
 * Moves the most recent background job to the foreground.
 *
 * Input:
 *  - `head`: Pointer to the head of the job list.
 *
 * Output:
 *  - Resumes the job in the foreground.
 *  - Removes the job from the list once completed.
 */
void fg(Slist **head)
{
    if (*head == NULL)
    {
        printf(ANSI_COLOR_RED "bash: fg: no current jobs\n");
        return;
    }

    Slist *temp = *head;
    int status;

    printf(ANSI_COLOR_YELLOW "Bringing [%d] %s to foreground\n", temp->child_pid, temp->command);
    
    // Send SIGCONT to resume process
    kill(temp->child_pid, SIGCONT);

    // Wait for the process to finish in the foreground
    waitpid(temp->child_pid, &status, WUNTRACED);
    stopped = 1; 

    // Remove job from list after completion
    remove_job_from_list(head, temp->child_pid);
}

/**
 * Function: bg
 * ------------
 * Resumes the most recent background job in the background.
 *
 * Input:
 *  - `head`: Pointer to the head of the job list.
 *
 * Output:
 *  - Resumes the job in the background.
 */
void bg(Slist **head)
{
    if (*head == NULL)
    {
        printf(ANSI_COLOR_RED "bash: bg: no current jobs\n");
        return;
    }

    Slist *temp = *head;

    printf(ANSI_COLOR_YELLOW "Resuming [%d] %s in background\n", temp->child_pid, temp->command);

    // Send SIGCONT to resume process in the background
    kill(temp->child_pid, SIGCONT);

    stopped = 0;
}

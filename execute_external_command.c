#include "main.h"
int status;
extern pid_t child_pid; // Child process ID (used globally)
extern Slist *head;     // Head of the job list

// Function to execute external commands
void execute_external_commands(char *input_string)
{
    char *argv[200]; // Array to store command arguments
    int argc = 0;    // Argument count
    char *token;
    int pipe_index = 0; // Flag to check if a pipeline exists

    // Tokenize input string based on space delimiter
    token = strtok(input_string, " ");
    while (token != NULL)
    {
        argv[argc++] = token;
        if (strcmp(token, "|") == 0) // Check for pipe symbol "|"
        {
            pipe_index = 1; // Set flag if pipeline exists
        }
        token = strtok(NULL, " ");
    }
    argv[argc] = NULL; // Null-terminate the argument list

    // Create a new child process
    pid_t pid = fork();
    if (pid == 0) // Child process
    {
        if (pipe_index) // If the command involves a pipeline
        {
            printf(ANSI_COLOR_CYAN  "Executing piped command...\n" ANSI_COLOR_RESET);
            n_pipe(argv, argc); // Call function to handle piped commands
            exit(1);
        }
        else // If it is a simple external command
        {
            printf(ANSI_COLOR_MAGENTA "Executing external command: %s\n" ANSI_COLOR_RESET, argv[0]);
            execvp(argv[0], argv); // Execute command
            perror(ANSI_COLOR_RED "Command execution failed" ANSI_COLOR_RESET);
            exit(0);
        }
    }
    else if (pid > 0) // Parent process
    {
        child_pid = pid;                          // Store child PID globally
        insert_at_last(&head, pid, input_string); // Add process to job list

        // Wait for the child process unless it moves to the background
        waitpid(child_pid, &status, WUNTRACED);

        // Remove process from job list after completion
        remove_job_from_list(&head, child_pid);
        child_pid = 0;
    }
    else // Fork failed
    {
        perror(ANSI_COLOR_RED "fork failed" ANSI_COLOR_RESET);
    }
}

// Function to execute piped commands
void n_pipe(char *argv[], int argc)
{
    printf(ANSI_COLOR_CYAN  "Processing pipeline commands...\n" ANSI_COLOR_RESET);

    // Array to store positions of commands within argv[]
    int cmd_pos[argc];
    cmd_pos[0] = 0;    // First command starts at index 0
    int cmd_count = 1; // Number of commands in the pipeline

    // Identify command positions by locating pipe symbols "|"
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "|") == 0) // If pipe symbol found
        {
            argv[i] = NULL;               // Replace "|" with NULL to terminate the previous command
            cmd_pos[cmd_count++] = i + 1; // Next command starts after "|"
        }
    }

    // Pipe and process variables
    int fd[2]; // File descriptors for pipe
    pid_t pid;
    int status;

    // Iterate over each command in the pipeline
    for (int i = 0; i < cmd_count; i++)
    {
        if (i != cmd_count - 1) // Create pipe if not the last command
        {
            pipe(fd);
        }
        pid = fork(); // Fork a new process for each command
        if (pid > 0)  // Parent process
        {
            printf(ANSI_COLOR_BLUE "Created process [%d] for command: %s\n" ANSI_COLOR_RESET, pid, argv[cmd_pos[i]]);
            if (i != cmd_count - 1) // If not the last command, set up input redirection
            {
                close(fd[1]);   // Close write end
                dup2(fd[0], 0); // Redirect input to pipe read end
                close(fd[0]);
            }
        }
        else if (pid == 0) // Child process
        {
            printf(ANSI_COLOR_MAGENTA "Executing command: %s\n" ANSI_COLOR_RESET, argv[cmd_pos[i]]);
            if (i != cmd_count - 1) // If not the last command, set up output redirection
            {
                close(fd[0]);   // Close read end
                dup2(fd[1], 1); // Redirect output to pipe write end
                close(fd[1]);
            }
            execvp(argv[cmd_pos[i]], argv + cmd_pos[i]); // Execute command
            perror(ANSI_COLOR_RED "Execution failed" ANSI_COLOR_RESET);
            exit(0);
        }
        else // Fork failed
        {
            perror(ANSI_COLOR_RED "fork failed" ANSI_COLOR_RESET);
        }
    }

    // Wait for all child processes to complete
    for (int i = 0; i < cmd_count; i++)
    {
        wait(&status);
    }
    waitpid(pid, &status, WUNTRACED);
}

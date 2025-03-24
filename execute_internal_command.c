#include "main.h"

extern int status;  // Global variable to store the exit status of the last command
void execute_internal_commands(char *input_string)
{
    char input_copy[256];  // Create a copy of the input string to use with `strtok`
    strncpy(input_copy, input_string, sizeof(input_copy) - 1);
    input_copy[sizeof(input_copy) - 1] = '\0'; // Ensure null termination

    // Handle `echo $$` command (Prints the PID of the shell)
    if (strncmp(input_string, "echo $$", strlen("echo $$")) == 0)
    {
        printf(ANSI_COLOR_CYAN  "Shell PID: %d\n" ANSI_COLOR_RESET, getpid());
        return;
    }
    // Handle `echo $?` command (Prints the exit status of the last executed command)
    else if (strncmp(input_string, "echo $?", strlen("echo $?")) == 0)
    {
        printf(ANSI_COLOR_BLUE "Last Command Exit Status: %d\n" ANSI_COLOR_RESET, status);
        return;
    }
    // Handle `echo $SHELL` command (Prints the shell path from the environment variable)
    else if (strncmp(input_string, "echo $SHELL", strlen("echo $SHELL")) == 0)
    {
        char *shell_path = getenv("SHELL");
        if (shell_path)
        {
            printf(ANSI_COLOR_CYAN "Shell Path: %s\n" ANSI_COLOR_RESET, shell_path);
        }
        else
        {
            printf(ANSI_COLOR_CYAN "Shell Path: Not Set\n" ANSI_COLOR_RESET);
        }
        return;
    }
    
    // Handle `exit` command (Terminates the shell)
    if (strcmp(input_string, "exit") == 0)
    {
        printf(ANSI_COLOR_MAGENTA "Exiting Minishell...\n" ANSI_COLOR_RESET);
        exit(0);
    }
    
    // Handle `pwd` command (Prints the current working directory)
    if (strcmp(input_string, "pwd") == 0)
    {
        char buff[256]; // Buffer to store the directory path
        if (getcwd(buff, sizeof(buff)) != NULL)
        {
            printf(ANSI_COLOR_MAGENTA "Current Directory: %s\n" ANSI_COLOR_RESET, buff);
        }
        else
        {
            perror(ANSI_COLOR_RED "pwd error" ANSI_COLOR_RESET);
        }
        return;
    }

    // Handle `cd` command (Changes the current directory)
    char *token = strtok(input_copy, " "); // Tokenize input string to extract the command
    if (token != NULL && strcmp(token, "cd") == 0)
    {
        token = strtok(NULL, " "); // Get the next argument (directory path)
        if (token == NULL)
        {
            fprintf(stderr, ANSI_COLOR_RED "cd: missing argument\n" ANSI_COLOR_RESET);
        }
        else
        {
            if (chdir(token) != 0) // Attempt to change directory
            {
                perror(ANSI_COLOR_RED "cd" ANSI_COLOR_RESET);
            }
            else
            {
                printf(ANSI_COLOR_MAGENTA "Changed Directory to: %s\n" ANSI_COLOR_RESET, token);
            }
        }
    }
}

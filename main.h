#ifndef MAIN_H
#define MAIN_H

// Standard Libraries
#include <stdio.h>       // Standard I/O functions
#include <string.h>      // String manipulation functions
#include <stdlib.h>      // Memory allocation, process control, conversions
#include <ctype.h>       // Character handling functions
#include <stdio_ext.h>   // Extended stdio functions (__fpurge, etc.)
#include <sys/types.h>   // System data types
#include <sys/stat.h>    // File information
#include <fcntl.h>       // File control operations
#include <unistd.h>      // POSIX API (fork, exec, etc.)
#include <signal.h>      // Signal handling
#include <sys/wait.h>    // Process waiting

// Command Type Definitions
#define BUILTIN 1       // Indicates a built-in command
#define EXTERNAL 2      // Indicates an external command
#define NO_COMMAND 3    // Indicates an unrecognized command

// ANSI Color Codes for Output Formatting
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

// Global Variables
extern char *external_command[200]; // Array to store external commands
extern pid_t child_pid;             // Stores child process ID
extern char input_string[25];        // Stores user input command string
extern char prompt[25];              // Stores command prompt text
extern char original_input[100];     // Stores original input before processing
extern pid_t delete;                 // Stores PID to be deleted from job list
extern int flag;                      // Generic flag variable for status tracking

// Linked List Structure for Job Management
typedef struct node
{
    int child_pid;          // Process ID of the child
    char command[50];       // Stores the executed command
    struct node *link;      // Pointer to the next node in the list
} Slist;

// Function Prototypes

// Input Handling
void scan_input(char *prompt, char *input_string);  // Reads user input
char *get_command(char *input_string);             // Extracts command from input
void copy_change(char *prompt, char *input_string); // Modifies prompt/input as needed

// Command Type Checking
int check_command_type(char *command);             // Determines if a command is built-in or external

// Internal Command Execution
void echo(char *input_string, int status);        // Handles 'echo' command
void execute_internal_commands(char *input_string); // Executes built-in commands

// External Command Execution
void execute_external_commands(char *input_string); // Executes external commands
void extract_external_commands(char **external_commands); // Extracts external commands from input

// Pipelining
void n_pipe(char *argv[], int argc);              // Handles execution of piped commands

// Signal Handling
void signal_handler(int sig_num);                 // Handles signals (e.g., SIGINT, SIGTSTP)
void my_handler(int signum);                      // Custom signal handler function

// Job Control Functions
void insert_at_last(Slist **head, int child_pid, char *command); // Adds process to job list
void display_list(Slist **head);                 // Displays running background jobs
void bg(Slist **head);                           // Moves a process to the background
void fg(Slist **head);                           // Brings a background process to the foreground
void remove_job_from_list(Slist **head, pid_t pid); // Removes a completed process from the job list

#endif

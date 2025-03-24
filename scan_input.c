#include "main.h"

// Global Variables
pid_t child_pid;                  // Stores the process ID of the child process
extern Slist *head;                // Pointer to the linked list of background jobs
char original_input[100];          // Stores the original user input before processing
void scan_input(char *prompt, char *input_string)
{
    // Load external commands from the file "external_commands.txt"
    extract_external_commands(external_command);

    // Handle CTRL+C (SIGINT) and CTRL+Z (SIGTSTP) signals
    signal(SIGINT, my_handler);
    signal(SIGTSTP, my_handler);

    while (1) // Infinite loop for taking user input
    {
        memset(input_string, 0, sizeof(input_string)); // Clear input buffer
        
        // Print shell prompt in green color
        printf(ANSI_COLOR_GREEN "%s ", prompt);
        fflush(stdout); // Flush output to ensure proper display

        // Change input text color to yellow
        printf(ANSI_COLOR_CYAN);
        scanf(" %[^\n]", input_string);  // Read user input (entire line)
        printf(ANSI_COLOR_RESET); // Reset terminal color after input
        
        getchar(); // Consume newline character left in buffer

        // Store original input for later reference
        strcpy(original_input, input_string);

        // Handle prompt change (PS1= command)
        if (strncmp(input_string, "PS1=", 4) == 0)
        {
            if (input_string[4] != ' ' && input_string[4] != '\0')
            {
                strcpy(prompt, input_string + 4); // Update shell prompt
            }
            else
            {
                printf(ANSI_COLOR_RED "PS1: prompt string must not be empty or contain space after '='\n");
            }
        }
        else
        {
            // Extract the first word (command) from user input
            char *command = get_command(input_string);
            int command_type = check_command_type(command);

            if (command_type == BUILTIN)
            {
                execute_internal_commands(input_string); // Execute built-in commands
            }
            else if (command_type == EXTERNAL)
            {
                execute_external_commands(input_string); // Execute external commands
            }
            else if (strncmp(input_string, "jobs", 5) == 0)
            {
                display_list(&head); // Display background jobs
            }
            else if (strcmp(input_string, "fg") == 0)
            {
                fg(&head); // Bring a background job to the foreground
            }
            else if (strcmp(input_string, "bg") == 0)
            {
                bg(&head); // Resume a stopped background job
            }
            else
            {
                printf(ANSI_COLOR_RED "Not a command\n"); // Invalid command message
            }
        }
    }
}

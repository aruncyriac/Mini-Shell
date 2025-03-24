#include "main.h"
void extract_external_commands(char **external_commands)
{
    char ch;
    char buffer[25];  // Temporary buffer to store each command
    int i = 0, j = 0;

    // Open the file containing external commands in read-only mode
    int fd = open("external_commands.txt", O_RDONLY);
    if (fd == -1)  // Check if file opening failed
    {
        printf("File can't open\n");
        return;
    }

    // Read the file character by character
    while (read(fd, &ch, 1) > 0)
    {
        if (ch != '\n')  // If the character is not a newline, store it in the buffer
        {
            buffer[i] = ch;
            i++;
        }
        else  // If newline is encountered, process the buffer
        {
            buffer[i] = '\0';  // Null-terminate the string

            // Remove any trailing newline or carriage return characters
            for (int k = 0; buffer[k] != '\0'; k++)
            {
                if (buffer[k] == '\n' || buffer[k] == '\r')
                {
                    buffer[k] = '\0';
                    break;
                }
            }

            // Allocate memory for the command and store it in the array
            external_commands[j] = malloc(strlen(buffer) + 1);
            strcpy(external_commands[j], buffer);
            j++;  // Move to the next position in the external_commands array

            i = 0;  // Reset buffer index for the next command
        }
    }

    // Ensure the last command is added if file does not end with a newline
    if (i > 0)
    {
        buffer[i] = '\0';  // Null-terminate the last command

        // Remove trailing newline or carriage return
        for (int k = 0; buffer[k] != '\0'; k++)
        {
            if (buffer[k] == '\n' || buffer[k] == '\r')
            {
                buffer[k] = '\0';
                break;
            }
        }

        // Allocate memory and store the last command
        external_commands[j] = malloc(strlen(buffer) + 1);
        strcpy(external_commands[j], buffer);
        j++;
    }

    external_commands[j] = NULL; // Null-terminate the list of commands
    close(fd); // Close the file descriptor
}


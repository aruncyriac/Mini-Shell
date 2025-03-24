#include "main.h"

char *get_command(char *input_string)
{
    static char command[20]; // Static array to store the extracted command

    // Fill the array with zeroes to clear previous data
    memset(command, 0, sizeof(command));

    int i = 0; // Index for input_string
    int j = 0; // Index for command

    // Extract characters until a space or null terminator is encountered
    while (input_string[i] != ' ' && input_string[i] != '\0')
    {
        command[j] = input_string[i]; // Copy character to command array
        i++;
        j++;
    }

    command[j] = '\0'; // Null-terminate the extracted command

    return command; // Return the extracted command string
}

#include "main.h"
int check_command_type(char *command)
{
    // List of built-in shell commands
    char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs",
                        "let", "eval", "set", "unset", "export", "declare", "typeset",
                        "readonly", "getopts", "source", "exit", "exec", "shopt", "caller",
                        "true", "type", "hash", "bind", "help", NULL};

    int i = 0;

    // Check if the command matches any built-in command
    while (builtins[i])
    {
        if (strcmp(builtins[i], command) == 0) // Compare input command with built-in commands
        {
            return BUILTIN; // Return BUILTIN if a match is found
        }
        i++;
    }

    i = 0;

    // Check if the command matches any external command (assuming external_command[] is defined elsewhere)
    while (external_command[i])
    {
        if (strcmp(external_command[i], command) == 0) // Compare input command with external commands
        {
            return EXTERNAL; // Return EXTERNAL if a match is found
        }
        i++;
    }

    return NO_COMMAND; // Return NO_COMMAND if no match is found in either list
}

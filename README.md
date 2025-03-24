# Mini Shell (msh)

A custom shell (msh) implemented in C, supporting command execution, environment variable handling, job control, and signal handling.

## Features

### Customizable Prompt (PS1)
Users can modify the prompt dynamically using `PS1=NEW_PROMPT` while maintaining syntax validation.

### Process Management
Supports executing external commands by creating child processes, ensuring proper synchronization.

### Shell Variables
Implements special variables like:
- `echo $?` → Displays exit status of the last command
- `echo $$` → Prints the shell’s PID
- `echo $SHELL` → Displays the shell’s executable path

### Job Control
- `fg` → Bring a background job to the foreground
- `bg` → Resume a suspended job in the background
- `jobs` → List background and stopped jobs with their PIDs

### Signal Handling
- `Ctrl+C (SIGINT)` → Terminates the foreground process but keeps the shell running
- `Ctrl+Z (SIGTSTP)` → Suspends the foreground process and displays its PID

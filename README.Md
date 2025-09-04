# POSIX Shell Implementation using C++

Welcome to a user-defined interactive shell program using cpp.It mimics standard Linux Shell by supporting **built-in commands, system commands,I/O redirection,pipelines,background jobs, and command history**.

# Features:

* **System Call Integration**: Uses Linux system calls (`fork`, `execvp`, `pipe`, `dup2`, `waitpid`, etc.) for efficient process management and command execution.
* **Interactive Command Line**: Provides an interactive prompt similar to Bash/Zsh, supporting built-ins, system commands, pipelines, and redirection.
* **Process Management**: Handles both foreground and background jobs with signal handling (`SIGINT`, `SIGTSTP`, `SIGCHLD`).
* **File Operations**: Implements redirection (`<`, `>`, `>>`) and pipelines (`|`) for both system and built-in commands.
* **Command History**: Saves and loads past commands across sessions, with the ability to replay them.
* **Autocomplete**: Tab-completion support for built-in commands and files.
* **Custom Prompt**: Displays prompt in the format:

# Getting Started:
*Clone the Repository:\
`git clone https://github.com/your-username/POSIX-shell.git`\
`cd POSIX-shell`\
*Compile and Run: \
`make` \
`./shell.out`

# Description

- **header.h** contains all the header files being used in this codebase along with global variables/arrays which are being used using extern to use it in different files.
- **main.cpp** is the main cpp file for this shell from where all the different functions get called accordingly.
- **display.cpp** handles the display of the shell in the following format:
```
<username>@<system_name>:<current_directory>~>
```
- **builtin.cpp** implements **cd** , **pwd** , **echo** shell built-ins.
- **ls.cpp** lists files in the directory, with flags:
```
-l : long listing format
-a : include hidden files
-al/-la : combined

```
- **syscmd.cpp** executes system commands via **execvp**.
- **pinfo.cpp** displays process info(PID,status,memory,executable path).Defaults to shell PID if no argument is given.
- **search.cpp** search for a given file or folder under the current directory recursively and output **True** or **False**.
- **redirect.cpp** handles input/output redirection (<,>,>>).
- **pipeline.cpp** handles execution of commands connected by pipes ( | ).
- **signals.cpp** defines handlers for:
```
SIGINT - (Ctrl+C) - kill foreground process.
SIGTSTP - (Ctrl+Z) - stop and move process to background.
SIGCHLD - (Ctrl+D) - log out of the shell without having any effect on the actual terminal. 
```
- **autocomplete.cpp** implements tab completion for built-in commands and filesystem paths.
- **history.cpp** maintains command history, storing past 20 commands run on shell and also execute them.**history.txt** stores those commands.
- **Makefile** compiles all .cpp files into **mysh** with warning enabled, and includes **readline** support for autocomplete.

# Assumptions 
-The shell does not support advanced redirection operators like 2>&1, &>, 2>.

-The symbols  <, >, >>, &, |, ;, -  always retain their special meanings (not treated as arguments to echo, etc.).

-Command input may contain multiple spaces/tabs, which are handled gracefully.

-Background functionality is only implemented for system commands, not built-ins.

-Maximum path length is limited to **4096** characters (PATH_MAX).

-History file exists and persists across runs (created automatically if missing).

-Segmentation faults and invalid system calls are handled gracefully with error messages.

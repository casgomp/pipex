# pipex

### Description:
Recreate the behavior of the shell pipe operator,  connecting the output of one command to the input of another.

### Requirements:
Your program should be executed as follows:
`./pipex file1 cmd1 cmd2 file2`

It must take 4 arguments:
- `file1` and `file2` are file names.
- `cmd1` and `cmd2` are shell commands with their parameters.

It must behave exactly like the following shell command:
`$> < file1 cmd1 | cmd2 > file2`.

Examples:
`$> ./pipex infile "ls -l" "wc -l" outfile`

Its behavior should be equivalent to: `< infile ls -l | wc -l > outfile`

`$> ./pipex infile "grep a1" "wc -w" outfile`.

Its behavior should be equivalent to: `< infile grep a1 | wc -w > outfile`

### Learning process:
Key things learned from this project:
- Unix processes.
- Forking processes and executing commands with execve.
- Using pipes to communicate between processes.
- Environment variables. Most importantly PATH, that the system searches for paths of commands to be executed.
- Shell behavior.

### Useful sources:
These video tutorials about [Unix processes in C](https://www.youtube.com/playlist?list=PLfqABt5AS4FkW5mOn2Tn9ZZLLDwA3kZUY) was very helpful.

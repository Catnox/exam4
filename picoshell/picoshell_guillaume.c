#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int picoshell(char **cmds[])
{
    int i;
    int pipefd[2];
    int prev_fd;
    int exit_code;
    pid_t pid;
    int status;

	prev_fd = -1;
	exit_code = 0;
	i = 0;
    while (cmds[i])
    {
        if (cmds[i + 1] && pipe(pipefd))
                return (1);
        pid = fork();
        if (pid == -1)
        {
            if (cmds[i + 1])
			{
				close(pipefd[0]);
				close(pipefd[1]);
			}
			return (1);
        }
        if (pid == 0)
        {
            if (prev_fd != -1)
            {
                if (dup2(prev_fd, STDIN_FILENO) == -1)
                    exit(1);
                close(prev_fd);
            }
            if (cmds[i + 1])
            {
				close(pipefd[0]);
                if (dup2(pipefd[1], STDOUT_FILENO) == -1)
                    exit(1);
                close(pipefd[1]);
            }
            execvp(cmds[i][0], cmds[i]);
            exit(1);
        }
        if (prev_fd != -1)
            close(prev_fd);
        if (cmds[i + 1])
		{
            close(pipefd[1]);
            prev_fd = pipefd[0];
		}
        i++;
    }

    while (wait(&status) != -1)
    {
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            exit_code = 1;
    }

    return exit_code;
}
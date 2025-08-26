#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int ft_popen(const char *file, char const *argv[], char type)
{
    int fd[2];
    pid_t pid;

    if (!file || !argv || (type != 'r' && type != 'w'))
        return (-1);
    if (pipe(fd) < 0)
        return (-1);
    if ((pid = fork()) < 0)
        return (close(fd[0]), close(fd[1]), -1);
    if (pid == 0)
    {
        close(fd[type == 'r' ? 0 : 1]);
        if (dup2(fd[type == 'r' ? 1 : 0], type == 'r' ? 1 : 0) < 0)
            exit(-1);
        close(fd[type == 'r' ? 1 : 0]);
        //close(fd[0]);
        //close(fd[1]);
        execvp(file, (char *const *)argv);
        exit(-1);
    }
    close(fd[type == 'r' ? 1 : 0]);
    return (fd[type == 'r' ? 0 : 1]);
}

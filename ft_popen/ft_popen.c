#include <unistd.h>
#include <stdlib.h>

int ft_popen(const char *file, char *const argv[], char type)
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
        execvp(file, argv);
        exit(-1);
    }
    close(fd[type == 'r' ? 1 : 0]);
    return (fd[type == 'r' ? 0 : 1]);
}


/* Ceci est la version que j'ai passe en examen et qui est donc valide.
Pour la version au dessus elle doit aussi etre bonne mais j'avais oublie
 les parentheses pour assignation et la comparaison pour le fork.
J'ai choisi de close les 2 fd apres dup2 pour simplifier l'ecriture. */
int ft_popen(const char *file, char *const argv[], char type)
{
    int fd[2];
    pid_t pid;

    if (!file || !argv || (type != 'r' && type != 'w'))
        return (-1);
    if (pipe(fd) < 0)
        return (-1);
    pid = fork();
    if (pid < 0)
        return (close(fd[0]), close(fd[1]), -1);
    if (pid == 0)
    {
        if (dup2(fd[type == 'r' ? 1 : 0], type == 'r' ? 1 : 0) < 0)
            exit(-1);
        close(fd[0]);
        close(fd[1]);
        execvp(file, argv);
        exit(-1);
    }
    close(fd[type == 'r' ? 1 : 0]);
    return (fd[type == 'r' ? 0 : 1]);
}
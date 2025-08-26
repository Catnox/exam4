#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int picoshell(char **cmds[])
{
    int i = 0, in_fd = 0, fd[2], status, ret = 0;
    pid_t pid;

    while (cmds[i])
    {
        // Créer pipe seulement si pas dernière commande
        if (cmds[i + 1] && pipe(fd) == -1)
            return (close(in_fd), 1);

        if ((pid = fork()) < 0)
            return (close(in_fd), cmds[i + 1] ? (close(fd[0]), close(fd[1]), 1) : 1);

        if (pid == 0) // Processus enfant
        {
            // Rediriger stdin si nécessaire
            if (in_fd != 0 && (dup2(in_fd, 0) == -1 || close(in_fd) == -1))
                exit(1);
            // Rediriger stdout si pas dernière commande
            if (cmds[i + 1] && (dup2(fd[1], 1) == -1 || close(fd[1]) == -1 || close(fd[0]) == -1))
                exit(1);
            
            execvp(cmds[i][0], cmds[i]);
            exit(1);
        }
        
        // Processus parent
        if (in_fd != 0) close(in_fd);
        if (cmds[i + 1]) close(fd[1]);
        in_fd = cmds[i + 1] ? fd[0] : 0;
        i++;
    }

    // Attendre tous les processus enfants
    while (wait(&status) > 0)
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
            ret = 1;

    return ret;
}

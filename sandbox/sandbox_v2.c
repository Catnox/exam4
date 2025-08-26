#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

static void timeout_handler(int sig) { (void)sig; }

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    pid_t pid = fork();
    if (pid < 0) return -1;

    if (pid == 0)
    {
        f();
        _exit(0);
    }

    // Configuration du handler pour SIGALRM dans le parent
    struct sigaction sa = {0};
    sa.sa_handler = timeout_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGALRM, &sa, NULL) < 0)
        return -1;

    alarm(timeout);
    
    int status;
    pid_t r = waitpid(pid, &status, 0);
    
    if (r < 0 && errno == EINTR)
    {
        // Timeout - kill l'enfant et le récolter
        kill(pid, SIGKILL);
        waitpid(pid, NULL, 0);
        if (verbose)
            printf("Bad function: timed out after %u seconds\n", timeout);
        return 0;
    }
    
    if (r < 0) return -1;
    
    alarm(0); // Annuler l'alarme restante

    if (WIFEXITED(status))
    {
        int code = WEXITSTATUS(status);
        if (code == 0)
        {
            if (verbose) printf("Nice function!\n");
            return 1;
        }
        if (verbose) printf("Bad function: exited with code %d\n", code);
        return 0;
    }

    if (WIFSIGNALED(status))
    {
        if (verbose) printf("Bad function: %s\n", strsignal(WTERMSIG(status)));
        return 0;
    }

    return -1;
}

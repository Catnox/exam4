#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

static void timeout_handler(int sig) { (void)sig; }

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    pid_t pid = fork();
    if (pid < 0) return (-1);
    if (pid == 0)
    {
        f();
        _exit(0);
    }
    struct sigaction sa = {0};
    sa.sa_handler = timeout_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGALRM, &sa, NULL) < 0) return (-1);
    alarm(timeout);
    int status;
    pid_t r = waitpid(pid, &status, 0);
    alarm(0);
    if (r < 0 && errno == EINTR)
    {
        kill(pid, SIGKILL);
        waitpid(pid, NULL, 0);
        if (verbose) printf("Bad function: timed out after %u seconds\n", timeout);
        return (0);
    }
    if (r < 0) return (-1);
    if (WIFEXITED(status))
    {
        int code = WEXITSTATUS(status);
        if (code == 0)
        {
            if (verbose) printf("Nice function!\n");
            return (1);
        }
        if (verbose) printf("Bad function: exited with code %d\n", code);
        return (0);
    }
    if (WIFSIGNALED(status))
    {
        int sig = WTERMSIG(status);
        if (sig == SIGALRM)
        {
            if (verbose) printf("Bad function: timed out after %u seconds\n", timeout);
        }
        else
        {
            if (verbose) printf("Bad function: %s\n", strsignal(sig));
        }
        return (0);
    }
    return (-1);
}
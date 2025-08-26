#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int ft_popen(const char *file, char const *argv[], char type);

int main() {
    int fd, status;
    printf("Test simple avec debug\n");
    
    const char *args[] = {"echo", "test", NULL};
    fd = ft_popen("echo", args, 'r');
    printf("ft_popen returned fd: %d\n", fd);
    
    if (fd != -1) {
        char buffer[100];
        ssize_t bytes = read(fd, buffer, sizeof(buffer)-1);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            printf("Read: %s", buffer);
        }
        
        int close_result = close(fd);
        printf("close() result: %d", close_result);
        if (close_result == -1) {
            printf(" (errno: %s)", strerror(errno));
        }
        printf("\n");
        
        wait(&status);
        printf("Child exit status: %d\n", WEXITSTATUS(status));
    }
    
    return 0;
}

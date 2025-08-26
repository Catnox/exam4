#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int ft_popen(const char *file, char const *argv[], char type);

int main()
{
    int fd;
    char buffer[1024];
    ssize_t bytes_read;
    int status;

    // Test 1: Lecture avec 'r' - lire la sortie de "echo hello"
    printf("Test 1: Lecture avec 'r'\n");
    const char *args_r[] = {"echo", "hello world", NULL};
    fd = ft_popen("echo", args_r, 'r');
    if (fd != -1) {
        bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Lu: %s", buffer);
        }
        close(fd);
        wait(&status);
    } else {
        printf("Erreur ft_popen\n");
    }

    // Test 2: Écriture avec 'w' - envoyer du texte à "cat"
    printf("\nTest 2: Écriture avec 'w'\n");
    const char *args_w[] = {"cat", NULL};
    fd = ft_popen("cat", args_w, 'w');
    if (fd != -1) {
        write(fd, "Hello from parent!\n", 19);
        close(fd);
        wait(&status);
    } else {
        printf("Erreur ft_popen\n");
    }

    printf("\nTests terminés\n");
    return 0;
}

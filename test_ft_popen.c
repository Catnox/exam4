#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

// Fonction ft_popen à tester
int ft_popen(const char *file, char const *argv[], char type);

void test_read_mode()
{
    printf("=== TEST MODE 'r' (lecture de la sortie de ls) ===\n");
    
    int fd = ft_popen("ls", (char const *[]){"ls", "-l", NULL}, 'r');
    if (fd == -1)
    {
        printf("ERREUR: ft_popen a retourné -1\n");
        return;
    }
    
    char buffer[1024];
    ssize_t bytes_read;
    printf("Sortie de 'ls -l':\n");
    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytes_read] = '\0';
        printf("%s", buffer);
    }
    
    close(fd);
    wait(NULL); // Attendre que le processus enfant se termine
    printf("TEST MODE 'r': OK\n\n");
}

void test_write_mode()
{
    printf("=== TEST MODE 'w' (écriture vers cat) ===\n");
    
    int fd = ft_popen("cat", (char const *[]){"cat", NULL}, 'w');
    if (fd == -1)
    {
        printf("ERREUR: ft_popen a retourné -1\n");
        return;
    }
    
    const char *message = "Hello from ft_popen write mode!\n";
    write(fd, message, strlen(message));
    close(fd);
    wait(NULL); // Attendre que le processus enfant se termine
    printf("TEST MODE 'w': OK\n\n");
}

void test_invalid_type()
{
    printf("=== TEST TYPE INVALIDE ('x') ===\n");
    
    int fd = ft_popen("ls", (char const *[]){"ls", NULL}, 'x');
    if (fd == -1)
        printf("TEST TYPE INVALIDE: OK (retourne bien -1)\n");
    else
    {
        printf("TEST TYPE INVALIDE: ECHEC (devrait retourner -1)\n");
        close(fd);
    }
    printf("\n");
}

void test_null_file()
{
    printf("=== TEST FILE NULL ===\n");
    
    int fd = ft_popen(NULL, (char const *[]){"ls", NULL}, 'r');
    if (fd == -1)
        printf("TEST FILE NULL: OK (retourne bien -1)\n");
    else
    {
        printf("TEST FILE NULL: ECHEC (devrait retourner -1)\n");
        close(fd);
    }
    printf("\n");
}

void test_null_argv()
{
    printf("=== TEST ARGV NULL ===\n");
    
    int fd = ft_popen("ls", NULL, 'r');
    if (fd == -1)
        printf("TEST ARGV NULL: OK (retourne bien -1)\n");
    else
    {
        printf("TEST ARGV NULL: ECHEC (devrait retourner -1)\n");
        close(fd);
    }
    printf("\n");
}

void test_invalid_command()
{
    printf("=== TEST COMMANDE INEXISTANTE ===\n");
    
    int fd = ft_popen("commande_qui_nexiste_pas", 
                      (char const *[]){"commande_qui_nexiste_pas", NULL}, 'r');
    if (fd == -1)
    {
        printf("TEST COMMANDE INEXISTANTE: OK (retourne -1)\n");
    }
    else
    {
        printf("Fd retourné: %d\n", fd);
        char buffer[100];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read <= 0)
            printf("TEST COMMANDE INEXISTANTE: OK (aucune sortie)\n");
        else
            printf("TEST COMMANDE INEXISTANTE: Données lues malgré l'erreur\n");
        close(fd);
        wait(NULL);
    }
    printf("\n");
}

int main()
{
    printf("=== TESTS DE LA FONCTION ft_popen ===\n\n");
    
    test_read_mode();
    test_write_mode();
    test_invalid_type();
    test_null_file();
    test_null_argv();
    test_invalid_command();
    
    printf("=== FIN DES TESTS ===\n");
    printf("Utilisez 'leaks' ou 'valgrind' pour vérifier les fuites mémoire\n");
    
    return 0;
}

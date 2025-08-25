#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int ft_popen(const char *file, char const *argv[], char type);

int main()
{
    printf("Test de fuites de fd...\n");
    
    // Test avec paramètres invalides (ne devrait pas créer de fd)
    for (int i = 0; i < 100; i++)
    {
        int fd1 = ft_popen(NULL, (char const *[]){"ls", NULL}, 'r');
        int fd2 = ft_popen("ls", NULL, 'r');  
        int fd3 = ft_popen("ls", (char const *[]){"ls", NULL}, 'x');
        
        if (fd1 != -1 || fd2 != -1 || fd3 != -1)
        {
            printf("ERREUR: Un fd a été créé avec des paramètres invalides!\n");
            return 1;
        }
    }
    
    printf("Test terminé - aucune fuite détectée avec paramètres invalides\n");
    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>

int ft_popen(const char *file, char const *argv[], char type);

int count_open_fds() {
    DIR *d = opendir("/proc/self/fd");
    int count = 0;
    struct dirent *entry;
    
    if (!d) return -1;
    while ((entry = readdir(d)) != NULL) {
        if (entry->d_name[0] != '.') {
            count++;
        }
    }
    closedir(d);
    return count - 1; // -1 pour enlever le fd du opendir
}

int main() {
    int fd, status;
    int fds_before, fds_after;
    
    printf("Test de fuite de descripteurs de fichiers\n");
    
    fds_before = count_open_fds();
    printf("FDs avant ft_popen: %d\n", fds_before);
    
    // Test multiple fois pour vérifier
    for (int i = 0; i < 5; i++) {
        const char *args[] = {"echo", "test", NULL};
        fd = ft_popen("echo", args, 'r');
        if (fd != -1) {
            close(fd);
            wait(&status);
        }
    }
    
    fds_after = count_open_fds();
    printf("FDs après 5 appels: %d\n", fds_after);
    
    if (fds_before == fds_after) {
        printf("✅ Aucune fuite de descripteurs détectée!\n");
    } else {
        printf("❌ Fuite détectée: %d descripteurs en plus\n", fds_after - fds_before);
    }
    
    return 0;
}

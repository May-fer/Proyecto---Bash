#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

void print_msg(const char *msg) {
    write(STDOUT_FILENO, msg, strlen(msg));
}

void print_error(const char *msg) {
    write(STDERR_FILENO, msg, strlen(msg));
    write(STDERR_FILENO, "\n", 1);
}

void builtin_cd(char **args) {
    if (args[1] == NULL) {
        print_error("Uso esperado: cd <directorio>");
    } else {
        if (chdir(args[1]) != 0) {
            perror("Error en cd");
        }
    }
}

void builtin_pwd() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        print_msg(cwd);
        print_msg("\n");
    } else {
        perror("Error en pwd");
    }
}

void builtin_mkdir(char **args) {
    if (args[1] == NULL) {
        print_error("Uso: mkdir <nombre>");
        return;
    }
    if (mkdir(args[1], 0755) != 0) {
        perror("Error al crear directorio");
    }
}

void builtin_rm(char **args) {
    if (args[1] == NULL) {
        print_error("Uso: rm <archivo>");
        return;
    }
    if (unlink(args[1]) != 0) {
        if (rmdir(args[1]) != 0) {
            perror("Error al eliminar");
        }
    }
}

void builtin_mv(char **args) {
    if (args[1] == NULL || args[2] == NULL) {
        print_error("Uso: mv <origen> <destino>");
        return;
    }
    if (rename(args[1], args[2]) != 0) {
        perror("Error en mv");
    }
}
void builtin_cat(char **args) {
    if (args[1] == NULL) {
        print_error("Uso: cat <archivo>");
        return;
    }
    int fd = open(args[1], O_RDONLY);
    if (fd < 0) {
        perror("Error al abrir archivo");
        return;
    }
    
    char buf[1024];
    int n;
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        write(STDOUT_FILENO, buf, n);
    }
    close(fd);
    print_msg("\n");
}

void builtin_cp(char **args) {
    if (args[1] == NULL || args[2] == NULL) {
        print_error("Uso: cp <origen> <destino>");
        return;
    }
    
    int src_fd = open(args[1], O_RDONLY);
    if (src_fd < 0) {
        perror("Error abriendo origen");
        return;
    }

    
    int dest_fd = open(args[2], O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (dest_fd < 0) {
        perror("Error abriendo destino");
        close(src_fd);
        return;
    }

    char buf[4096]; 
    int n;
    while ((n = read(src_fd, buf, sizeof(buf))) > 0) {
        if (write(dest_fd, buf, n) != n) {
            perror("Error de escritura");
            break;
        }
    }
    close(src_fd);
    close(dest_fd);
}

void builtin_ls() {
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                print_msg(dir->d_name);
                print_msg("  ");
            }
        }
        print_msg("\n");
        closedir(d);
    } else {
        perror("Error en ls");
    }
}

void ejecutar_comando(char **args, int background, char *input_file, char *output_file) {
    pid_t pid = fork(); 

    if (pid == 0) {
        
        if (input_file != NULL) {
            int fd0 = open(input_file, O_RDONLY);
            if (fd0 < 0) {
                perror("Error abriendo archivo de entrada");
                exit(1);
            }
            dup2(fd0, STDIN_FILENO); 
            close(fd0);
        }

        if (output_file != NULL) {
            int fd1 = open(output_file, O_CREAT | O_WRONLY | O_TRUNC, 0644); 
            if (fd1 < 0) {
                perror("Error abriendo archivo de salida");
                exit(1);
            }
            dup2(fd1, STDOUT_FILENO); 
            close(fd1);
        }

        if (execvp(args[0], args) == -1) {
            perror("Comando no encontrado");
        }
        exit(EXIT_FAILURE); 

    } else if (pid < 0) {
        perror("Error al hacer fork");
    } else {
        
        if (!background) {
            waitpid(pid, NULL, 0); 
        } else {
            char msg[50];
            print_msg("[Proceso en background iniciado]\n");
        }
    }
}


int main() {
    char input[MAX_CMD_LEN];
    char *args[MAX_ARGS];
    
    while (1) {
        print_msg("mi_shell> "); 
        int bytes_read = read(STDIN_FILENO, input, MAX_CMD_LEN);
        if (bytes_read <= 0) break; // Ctrl+D o error
        
        input[bytes_read - 1] = '\0'; // Reemplazar \n con null terminator

        if (strlen(input) == 0) continue; // Enter vacío

        int i = 0;
        int background = 0;
        char *input_file = NULL;
        char *output_file = NULL;
        
        char *token = strtok(input, " ");
        while (token != NULL) {
            if (strcmp(token, "&") == 0) {
                background = 1;
            } else if (strcmp(token, ">") == 0) {
                token = strtok(NULL, " "); // Siguiente token es el archivo
                output_file = token;
            } else if (strcmp(token, "<") == 0) {
                token = strtok(NULL, " "); // Siguiente token es el archivo
                input_file = token;
            } else {
                args[i++] = token;
            }
            token = strtok(NULL, " ");
        }
        args[i] = NULL; 

        if (args[0] == NULL) continue;

        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        if (strcmp(args[0], "cd") == 0) builtin_cd(args);
        else if (strcmp(args[0], "pwd") == 0) builtin_pwd();
        else if (strcmp(args[0], "mkdir") == 0) builtin_mkdir(args);
        else if (strcmp(args[0], "rm") == 0) builtin_rm(args);
        else if (strcmp(args[0], "mv") == 0) builtin_mv(args);
        else if (strcmp(args[0], "cp") == 0) builtin_cp(args);
        else if (strcmp(args[0], "cat") == 0) builtin_cat(args);
        else if (strcmp(args[0], "ls") == 0) builtin_ls();
        else {
            ejecutar_comando(args, background, input_file, output_file);
        }
    }

    return 0;
}

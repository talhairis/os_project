// Kabuk (Shell) Programı
#include "Shell.h"

pid_t background_pids[MAX_ARGS];
int background_count = 0;
int keep_running;

// sigaction, WHANGUP

// Arka planda çalışan işlemleri izleyen fonksiyon
void* monitor_background_processes(void* arg) {
    while (1) {
        sleep(1); // Sürekli kontrol için bekleme (1 saniye)
        for (int i = 0; i < background_count; i++) {
            int status;
            pid_t pid = waitpid(background_pids[i], &status, WNOHANG);
            if (pid > 0) {  // İşlem tamamlandı
                if (WIFEXITED(status)) {
                    printf("\nProcess [%d] finished with exit status %d\n", pid, WEXITSTATUS(status));
                } else if (WIFSIGNALED(status)) {
                    printf("\nProcess [%d] terminated by signal %d\n", pid, WTERMSIG(status));
                }
                // İşlem tamamlandığında listeden kaldır
                for (int j = i; j < background_count - 1; j++) {
                    background_pids[j] = background_pids[j + 1];
                }
                background_count--;

                // Prompt yeniden yazdır
                if (keep_running == 1) {
                    printf("> ");
                    fflush(stdout);
                }
            }
        }

        if (background_count == 0 && keep_running == 0) {
            break;
        }
    }
}

void shell() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    keep_running = 1;

    while(1) {
        printf("> ");
        fflush(stdout);

        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            perror("Input error");
            continue;
        }

        // Girişin sonundaki yeni satır karakterini kaldır
        input[strcspn(input, "\n")] = 0;

        // ";" ile ayrılmış komutları işler
        char *commands[MAX_ARGS];
        int command_count = 0;

        char *command = strtok(input, ";");
        while (command != NULL) {
            commands[command_count++] = command;
            command = strtok(NULL, ";");
        }

        // Her komutu sırayla işle
        for (int i = 0; i < command_count; i++) {
            // Komutu parçala
            char *token = strtok(commands[i], " \t\r\n");
            int arg_index = 0;
            while (token != NULL) {
                args[arg_index++] = token;
                token = strtok(NULL, " \t\r\n");
            }
            args[arg_index] = NULL;

            // Boş komutları atla
            if (args[0] == NULL) {
                continue;
            }

            // quit komutunu kontrol et
            if (strcmp(args[0], "quit") == 0) {
                handle_quit();
                return;
            }

            // Arka planda çalıştırmayı kontrol et
            int background = 0;
            if (arg_index > 0 && strcmp(args[arg_index - 1], "&") == 0) {
                background = 1;
                args[arg_index - 1] = NULL;
            }

            // Pipe'ı kontrol et ve işle
            if (handle_pipe(args)) {
                continue;
            }

            // Komutu çalıştır
            execute_command(args, background);
        }
    }
}

void execute_command(char **args, int background) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return;
    }

    if (pid == 0) {
        // Çocuk süreç
        redirect_input_output(args);
        if (execvp(args[0], args) < 0) {
            perror("Exec failed");
            exit(EXIT_FAILURE);
        }
    } else {
        // Ana süreç
        if (background) {
            background_pids[background_count++] = pid;
            printf("[%d] Running in background\n", pid);
        } else {
            int status;
            waitpid(pid, &status, 0);
        }
    }
}

void redirect_input_output(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "<") == 0) {
            int fd = open(args[i + 1], O_RDONLY);
            if (fd < 0) {
                perror("Input file open failed");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            args[i] = NULL;
            args[i + 1] = NULL; // Pipe'lara karışmaması için temizle
        } else if (strcmp(args[i], ">") == 0) {
            int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("Output file open failed");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL;
            args[i + 1] = NULL; // Pipe'lara karışmaması için temizle
        }
    }
}

int handle_pipe(char **args) {
    int pipe_positions[MAX_ARGS] = {0};
    int pipe_count = 0;

    // Pipe pozisyonlarını tespit et
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            pipe_positions[pipe_count++] = i;
            args[i] = NULL; // Pipe sembolünü kaldır
        }
    }

    if (pipe_count == 0) {
        return 0; // Pipe yok
    }

    int prev_pipe_fd = -1;
    for (int i = 0; i <= pipe_count; i++) {
        int pipe_fd[2];
        if (i < pipe_count && pipe(pipe_fd) < 0) {
            perror("Pipe failed");
            return -1;
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            return -1;
        }

        if (pid == 0) {
            // Çocuk süreç
            if (prev_pipe_fd != -1) {
                dup2(prev_pipe_fd, STDIN_FILENO);
                close(prev_pipe_fd);
            }
            if (i < pipe_count) {
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }

            if (pipe_fd[0] != -1) close(pipe_fd[0]);

            // Komutun yönlendirmesini ayarla
            redirect_input_output(args + (i == 0 ? 0 : pipe_positions[i - 1] + 1));

            execvp(args[i == 0 ? 0 : pipe_positions[i - 1] + 1], args + (i == 0 ? 0 : pipe_positions[i - 1] + 1));
            perror("Exec failed");
            exit(EXIT_FAILURE);
        } else {
            // Ana süreç
            if (prev_pipe_fd != -1) close(prev_pipe_fd);
            if (i < pipe_count) close(pipe_fd[1]);

            prev_pipe_fd = pipe_fd[0];
        }
    }

    for (int i = 0; i <= pipe_count; i++) wait(NULL);

    return 1;
}

void handle_quit() {
    printf("Exiting shell...\n");
    keep_running = 0;  // Shell döngüsünü sonlandır
}
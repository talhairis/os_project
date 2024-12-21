// Kabuk (Shell) Programı
#include "Shell.h"


void shell() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    while(1) {
        printf("> ");
        fflush(stdout);

        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            perror("Input error");
            continue;
        }

        // Yeni satır karakterini kaldır
        input[strcspn(input, "\n")] = 0;

        // Komutları ayrıştır
        int i = 0;
        char *token = strtok(input, " ");
        while (token != NULL && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (args[0] == NULL) {
            continue;
        }

        // quit komutunu kontrol et
        if (strcmp(args[0], "quit") == 0) {
            handle_quit();
            return;
        }

        // Komutu çalıştır
        execute_command(args);
    }
}

void execute_command(char **args) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return;
    }

    if (pid == 0) {
        if (execvp(args[0], args) < 0) {
            perror("Exec failed");
            exit(EXIT_FAILURE);
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

void handle_quit() {
    printf("Exiting shell...\n");
}
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

            // Komutu çalıştır
            execute_command(args);
        }
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
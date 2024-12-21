#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

void shell();
void execute_command(char**);
void handle_quit();

#endif
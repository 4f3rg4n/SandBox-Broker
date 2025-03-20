#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>

#define MAX_PATH_SIZE 100

char* files_hook_hanlder(char* path, char* func, int flags);
int open_hook(const char *path, int flags);

extern unsigned long int hook_id;
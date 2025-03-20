#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>

#define PATH_MAX 4096

typedef struct {
    int fd;
    char* path;
    struct fd_obj* next;
} fd_obj;

char* files_hook_hanlder(const char* path, const char* func, int flags);
char* get_path_from_fd(int fd);
void add_fd_object(int fd, char* path);
ssize_t open_hook(const char *path, int flags, ...);


extern unsigned long int hook_id;
extern fd_obj* fd_list;
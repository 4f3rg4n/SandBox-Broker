#ifndef FILES_HOOK_H
#define FILES_HOOK_H
#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>

#define PATH_MAX 4096

/* Used for global fd's list */
typedef struct {
    int fd;
    char* path;
    struct fd_obj* next;
} fd_obj;

/* helper funcs */
char* files_hook_hanlder(const char* path, const char* func, int flags, const void* buf, const char* mode);

//fd's helpers
char* get_path_from_fd(int fd);
void add_fd_object(int fd, char* path);


/* syscalls hook funcs */
ssize_t open_hook(const char *path, int flags, ...);
ssize_t read_hook(int fd, void *buf, size_t count);
ssize_t write_hook(int fd, const void *buf, size_t count);

/* wrappers hook funcs */
FILE* fopen_hook(const char *path, const char *mode);
size_t fread_hook(void *ptr, size_t size, size_t count, FILE *stream);
size_t fwrite_hook(const void *ptr, size_t size, size_t count, FILE *stream);

/* Globals */
extern unsigned long int hook_id;
extern fd_obj* fd_list; 

#endif // FILES_HOOK_H
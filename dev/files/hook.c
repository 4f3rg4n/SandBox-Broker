#define _GNU_SOURCE
#include "hook.h"

static int (*real_open)(const char*, int, ...) = NULL;
static ssize_t (*real_read)(int, void*, size_t) = NULL;
fd_obj* fd_list = NULL;

unsigned long int hook_id = 0;

char* files_hook_hanlder(const char* path, const char* func, int flags) {
    char op;
    char* new_path = (char*)calloc(sizeof(char), PATH_MAX);

    printf("- [FILE HOOK {ID = %ld}] -\n", hook_id++);
    printf("++ hook data ++\n");
    printf("path: %s\n", path);
    printf("func: %s\n", func);
    printf("flags: %d\n", flags);

    printf("modify path? (y/n): ");
    op = getchar();
    getchar();
    if (op == 'y') {
        printf("path: ");
        fgets(new_path, PATH_MAX, stdin);
        new_path[strlen(new_path) - 1] = '\0';
        return new_path;
    }
    else {
        memcpy(new_path, path, strlen(path));
        printf("continue function process? (y/n): ");
        op = getchar();
        getchar();
        if (op == 'y')
            return new_path;
        return NULL;
    }
}

char* get_path_from_fd(int fd) {
    fd_obj* obj = fd_list;
    while(obj) {
        if(obj->fd == fd)
            return obj->path;
        obj = obj->next;
    }

    return NULL; //error
}

void add_fd_object(int fd, char* path) {
    fd_obj* obj = (fd_obj*)malloc(sizeof(fd_obj));
    obj->fd = fd;
    obj->path = path;
    obj->next = fd_list;
    fd_list = obj;
}

ssize_t open_hook(const char *path, int flags, ...) {
    int fd;
    mode_t mode = 0;

    // Check if the real open function is already saved
    if (!real_open)
        real_open = dlsym(RTLD_NEXT, "open"); // Get open symbol

    // Handle optional third argument (mode) using va_list
    if (flags & O_CREAT) {
        va_list args;
        va_start(args, flags);
        mode = va_arg(args, mode_t); // Get mode value if O_CREAT is used
        va_end(args);
    }

    char* new_path = files_hook_hanlder(path, "open", flags);
    fd = real_open(new_path, flags, mode);  // Pass mode if present
    add_fd_object(fd, new_path);

    return fd;
}

ssize_t read_hook(int fd, void *buf, size_t count) {
    ssize_t res;

    // Check if the real read function is already saved
    if (!real_read)
        real_read = dlsym(RTLD_NEXT, "read"); // Get read symbol

    char* path = get_path_from_fd(fd);
    char* new_path = files_hook_hanlder(path, "read", O_RDONLY);
    free(path);

    res = real_read(fd, buf, count);  // Call the real read function
    free(new_path);
    return res;
}

// We no longer define open() directly, we use alias to hook it
__attribute__((weak, alias("open_hook"))) int open(const char *path, int flags, ...);
__attribute__((weak, alias("read_hook"))) ssize_t read(int fd, void *buf, size_t count);

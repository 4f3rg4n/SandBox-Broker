#define _GNU_SOURCE
#include "hook.h"

static int (*real_open)(const char*, int, ...) = NULL;
static ssize_t (*real_read)(int, void*, size_t) = NULL;
fd_obj* fd_list = NULL;

unsigned long int hook_id = 0;

/*
This function handle files functions hooks.
Input:
path - file path
func - function name
flags - function flags
buf - buffer that need to be pass to the real function
Output: function new path.
*/
char* files_hook_hanlder(const char* path, const char* func, int flags, const void* buf) {
    char op;
    char* new_path = (char*)calloc(sizeof(char), PATH_MAX);

    /* Hook data */
    printf("- [FILE HOOK {ID = %ld}] -\n", hook_id++);
    printf("++ hook data ++\n");
    printf("buf: %s\n", buf);
    printf("path: %s\n", path);
    printf("func: %s\n", func);
    printf("flags: %d\n", flags);

    /* Handle function path modification */
    printf("modify path? (y/n): ");
    op = getchar();
    getchar();

    //new path
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
        if (op == 'y') //continue with old path
            return new_path;
        return NULL; //terminate function
    }
}

/* 
This function return file path by its id using global files linked list.
Input:
fd - file fd number
Output: file path.
*/
char* get_path_from_fd(int fd) {
    fd_obj* obj = fd_list;

    while(obj) { //loop until get the file path
        if(obj->fd == fd)
            return obj->path;
        obj = obj->next;
    }

    return NULL; //error: path not found!
}

/*
This function add fd number with file path to the global fd list.
Input:
fd- file fd number
path - file path
Output: NULL.
*/
void add_fd_object(int fd, char* path) {
    fd_obj* obj = (fd_obj*)malloc(sizeof(fd_obj));
    obj->fd = fd;
    obj->path = path;
    obj->next = fd_list;
    fd_list = obj;
}

/*
This function is the hook function of the open syscall func.
Input:
path - file path
flags - open flags
Output: file fd.
*/
ssize_t open_hook(const char *path, int flags, ...) {
    int fd;

    //check if the real open function is already saved
    if (!real_open)
        real_open = dlsym(RTLD_NEXT, "open"); //get open runtime addr

    char* new_path = files_hook_hanlder(path, "open", flags, 0);
    fd = real_open(new_path, flags); //run real open func
    add_fd_object(fd, new_path); //add fd & path to global fd's list

    return fd;
}

/*
This function is the hook function of the read syscall func.
Input:
fd - file fd number
buf - target buffer pointer to save data
count - number of bytes to read
Output: number of readed bytes.
*/
ssize_t read_hook(int fd, void *buf, size_t count) {
    ssize_t res;

    //check if the real read function is already saved
    if (!real_read)
        real_read = dlsym(RTLD_NEXT, "read"); //get read runtime addr

    char* path = get_path_from_fd(fd);
    char* new_path = files_hook_hanlder(path, "read", O_RDONLY, buf);

    res = real_read(fd, buf, count);  //call the real read function
    free(new_path);
    return res;
}

/*
This function is the hook function of the write syscall func.
Input:
fd - file fd number
buf - target buffer pointer to write data from
count - number of bytes to write
Output: number of written bytes.
*/
ssize_t write_hook(int fd, const void *buf, size_t count) {
    ssize_t res;

    //check if the real write function is already saved
    if (!real_read)
        real_read = dlsym(RTLD_NEXT, "write"); //get runtime write addr

    char* path = get_path_from_fd(fd);
    char* new_path = files_hook_hanlder(path, "write", O_RDONLY, buf);

    res = real_read(fd, buf, count);  //call the real write function
    free(new_path);
    return res;
}

/* Let the real open & read & write function become weak 
and the hook functions strong so they will replace them */
__attribute__((weak, alias("open_hook"))) int open(const char *path, int flags, ...);
__attribute__((weak, alias("read_hook"))) ssize_t read(int fd, void *buf, size_t count);
__attribute__((weak, alias("write_hook"))) ssize_t write(int fd, const void *buf, size_t count);

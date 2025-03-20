#include "hook.h"

static int (*real_open)(const char*, int, ...) = NULL;

unsigned long int hook_id = 0;

char* hook_hanlder(char* path, char* func, int flags) {
    char op;
    char* new_path = calloc(sizeof(char), MAX_PATH_SIZE);

    printf("- [HOOK {ID = %d}] -\n", hook_id++);
    printf("++ hook data ++\n");
    printf("path: %s\n", path);
    printf("func: %s\n", func);
    printf("flags: %d\n", flags);

    printf("modify path? (y/n): ");
    op = getchar();
    getchar();
    if(op == 'y'){
        printf("path: ");
        fgets(new_path, MAX_PATH_SIZE, stdin);
        new_path[strlen(new_path) -1] = '\0';
        return new_path;
    }
    else {
        memcpy(new_path, path, strlen(path));
        printf("continue function process ? (y/n): ");
        op = getchar();
        getchar();
        if(op == 'y')
            return new_path;
        return NULL;
    }

}

int open_hook(const char *path, int flags) {
    int op, res;

    //check if the real open function is already saved
    if (!real_open)
        real_open = dlsym(RTLD_NEXT, "open"); //get open symbol
    char* new_path = hook_hanlder(path, "open", flags);
    res = real_open(new_path, flags);
    free(new_path);
    return res;
}

int open(const char *path, int flags, ...) __attribute__((weak, alias("open_hook")));

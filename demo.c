#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char buffer[100];
    int fd;

    FILE *file = fopen("testfile.txt", "w+");
    fwrite("Hello, fwrite!\n", sizeof(char), 16, file);

    file = fopen("testfile.txt", "r");
    fread(buffer, sizeof(char), 100, file);
    printf("fread: %s", buffer);

    fd = open("testfile.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    write(fd, "Hello, write!\n", 14);

    fd = open("testfile.txt", O_RDONLY);
    read(fd, buffer, 100);
    printf("read: %s", buffer);

    return 0;
}

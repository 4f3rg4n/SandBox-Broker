#ifndef NETWORK_HOOK_H
#define NETWORK_HOOK_H
#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define IP_MAX 128

/* Used for global socket list */
typedef struct {
    int sockfd;
    char* ip_port;
    struct sock_obj* next;
} sock_obj;

/* Helper funcs */
char* network_hook_handler(int sockfd, const char* func, const void* buf);
void add_sock_object(int sockfd, char* ip_port);

/* Network hook funcs */
int socket_hook(int domain, int type, int protocol);
int connect_hook(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
ssize_t send_hook(int sockfd, const void* buf, size_t len, int flags);
ssize_t recv_hook(int sockfd, void* buf, size_t len, int flags);

/* Globals */
extern unsigned long int hook_id;
extern sock_obj* sock_list;

#endif // NETWORK_HOOK_H

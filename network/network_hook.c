#include "network_hook.h"

/* save real functions */
static int (*real_socket)(int, int, int) = NULL;
static int (*real_connect)(int, const struct sockaddr*, socklen_t) = NULL;
static ssize_t (*real_send)(int, const void*, size_t, int) = NULL;
static ssize_t (*real_recv)(int, void*, size_t, int) = NULL;

/* Globals */
unsigned long int hook_id = 0;
sock_obj* sock_list = NULL;

/* 
This function handles network function hooks. 
Input: 
sockfd - socket descriptor
func - function name (e.g., "send", "recv", "connect")
buf - socket data to send or receive (optional for some functions)
Output: new IP address or port if modified, NULL to continue with the original address.
*/
char* network_hook_handler(int sockfd, const char* func, const void* buf) {
    char op;
    char* new_ip = (char*)calloc(sizeof(char), IP_MAX);

    /* Hook data */
    printf("- [NETWORK HOOK {ID = %ld}] -\n", hook_id++);
    printf("++ hook data ++\n");
    printf("sockfd: %d\n", sockfd);
    printf("func: %s\n", func);
    printf("buf: %s\n", buf);

    /* Handle function data modification */
    printf("modify IP/Port? (y/n): ");
    op = getchar();
    getchar();

    if (op == 'y') {
        printf("Enter new IP/Port: ");
        fgets(new_ip, IP_MAX, stdin);
        new_ip[strlen(new_ip) - 1] = '\0';
        return new_ip;
    }

    return NULL; // return NULL to continue with the original socket address
}

/* 
This function adds socket details (sockfd, IP/Port) to the global socket list.
Input: 
sockfd - socket descriptor
ip_port - IP address and port of the socket
Output: NULL.
*/
void add_sock_object(int sockfd, char* ip_port) {
    sock_obj* obj = (sock_obj*)malloc(sizeof(sock_obj));
    obj->sockfd = sockfd;
    obj->ip_port = ip_port;
    obj->next = sock_list;
    sock_list = obj;
}

/* 
This function is the hook for the socket syscall function.
Input:
domain - the address family (e.g., AF_INET for IPv4)
type - the socket type (e.g., SOCK_STREAM for TCP)
protocol - the protocol (e.g., IPPROTO_TCP for TCP)
Output: socket descriptor.
*/
int socket_hook(int domain, int type, int protocol) {
    int sockfd;

    /* Check if the real socket function is already saved */
    if (!real_socket)
        real_socket = dlsym(RTLD_NEXT, "socket"); //get the socket function address

    sockfd = real_socket(domain, type, protocol); //call the real socket function

    //add socket details to the global list (stubbed with IP/Port)
    char* ip_port = (char*)malloc(IP_MAX); //placeholder for IP and Port
    snprintf(ip_port, IP_MAX, "Socket-%d", sockfd);
    add_sock_object(sockfd, ip_port);

    return sockfd;
}

/* 
This function is the hook for the connect syscall function.
Input:
sockfd - socket descriptor
addr - server address (struct sockaddr)
addrlen - length of the address
Output: 0 if success / -1 if error.
*/
int connect_hook(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
    int res;

    /* Check if the real connect function is already saved */
    if (!real_connect)
        real_connect = dlsym(RTLD_NEXT, "connect"); //get connect runtime addr

    //modify IP/Port if needed
    char* new_ip = network_hook_handler(sockfd, "connect", addr);
    if (new_ip) {
        //here you can modify the sockaddr to reflect the new IP/Port
        printf("Modified connect to IP/Port: %s\n", new_ip);
    }

    res = real_connect(sockfd, addr, addrlen); //call the real connect function

    free(new_ip);
    return res;
}

/* 
This function is the hook for the send syscall function.
Input:
sockfd - socket descriptor
buf - data to send
len - length of data
flags - flags for send (e.g., MSG_NOSIGNAL)
Output: number of bytes sent, or -1 on error.
*/
ssize_t send_hook(int sockfd, const void* buf, size_t len, int flags) {
    ssize_t res;

    /* Check if the real send function is already saved */
    if (!real_send)
        real_send = dlsym(RTLD_NEXT, "send"); //get send runtime addr

    //modify send data if needed
    char* new_ip = network_hook_handler(sockfd, "send", buf);
    if (new_ip) {
        printf("Modified send to IP/Port: %s\n", new_ip);
    }

    res = real_send(sockfd, buf, len, flags); //call the real send function
    free(new_ip);
    return res;
}

/* 
This function is the hook for the recv syscall function.
Input:
sockfd - socket descriptor
buf - buffer to store received data
len - maximum number of bytes to receive
flags - flags for recv (e.g., MSG_WAITALL)
Output: number of bytes received, or -1 on error.
*/
ssize_t recv_hook(int sockfd, void* buf, size_t len, int flags) {
    ssize_t res;

    /* Check if the real recv function is already saved */
    if (!real_recv)
        real_recv = dlsym(RTLD_NEXT, "recv"); //get recv runtime addr

    //modify recv data if needed
    char* new_ip = network_hook_handler(sockfd, "recv", buf);
    if (new_ip) {
        printf("Modified recv from IP/Port: %s\n", new_ip);
    }

    res = real_recv(sockfd, buf, len, flags); //call the real recv function
    free(new_ip);
    return res;
}

/* Let the real function become weak and the hook functions strong so they will replace them */
__attribute__((weak, alias("socket_hook"))) int socket(int domain, int type, int protocol);
__attribute__((weak, alias("connect_hook"))) int connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
__attribute__((weak, alias("send_hook"))) ssize_t send(int sockfd, const void* buf, size_t len, int flags);
__attribute__((weak, alias("recv_hook"))) ssize_t recv(int sockfd, void* buf, size_t len, int flags);

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <syslog.h>
#include <arpa/inet.h>

#define PORT "9000"
#define BACKLOG 10

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    //Syslog
    openlog(NULL,0,LOG_USER);

    // Setup address structures

    struct addrinfo hints, *res;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;

    int status;
    status = getaddrinfo(NULL, PORT, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error");
        return -1;
    }

    // Make and bind socket

    int sockfd;
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        fprintf(stderr, "socket error");
        return -1;
    }

    status = bind(sockfd, res->ai_addr, res->ai_addrlen);
    if (status == -1) {
        fprintf(stderr, "bind error");
        return -1; 
    }

    // Free memory used for struc addrinfo

    freeaddrinfo(res);

    // Listen for incoming connections

    status = listen(sockfd, BACKLOG);
    if (status == -1) {
        fprintf(stderr, "listen error");
        return -1;
    }

    // Accept incoming connections

    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    int new_fd;
    char s[INET6_ADDRSTRLEN];

    addr_size = sizeof(their_addr);
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
    if (new_fd == -1) {
        fprintf(stderr, "accept error");
        return -1;
    }

    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof(s));
    syslog(LOG_INFO,"Accepted connection from %s", s); 


    // Receive packet

    // Close connection
    close(new_fd);
    syslog(LOG_INFO, "Closed connection from %s", s);
    closelog();
}

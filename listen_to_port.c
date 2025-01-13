#include <stdio.h>

#include <string.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netdb.h>

#include <stdlib.h>

#include <unistd.h>



#define MYPORT "3490"  // Port to bind to

#define BACKLOG 10     // Number of pending connections queue



int main(void) {

    struct sockaddr_storage their_addr; // Client's address information

    socklen_t addr_size;

    struct addrinfo hints, *res;

    int sockfd, new_fd;



    memset(&hints, 0, sizeof hints);



    hints.ai_family = AF_UNSPEC;       // IPv4 or IPv6

    hints.ai_socktype = SOCK_STREAM;  // TCP stream sockets

    hints.ai_flags = AI_PASSIVE;      // Use my IP



    if (getaddrinfo(NULL, MYPORT, &hints, &res) != 0) {

        perror("getaddrinfo");

        exit(1);

    }



    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (sockfd == -1) {

        perror("socket");

        exit(1);

    }



    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {

        perror("bind");

        close(sockfd);

        exit(1);

    }



    freeaddrinfo(res); // Done with `res`



    if (listen(sockfd, BACKLOG) == -1) {

        perror("listen");

        exit(1);

    }



    printf("Waiting for a connection...\n");



    addr_size = sizeof their_addr;

    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

    if (new_fd == -1) {

        perror("accept");

        exit(1);

    }



    printf("Connection established!\n");



    char *msg = "hello im testing this";

    int len = strlen(msg);

    int bytes_sent = send(new_fd, msg, len, 0);

    if (bytes_sent == -1) {

        perror("send");

    } else {

        printf("Sent: %s\n", msg);

    }



    close(new_fd);

    close(sockfd);



    return 0;

}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include "lb_back_end.h"

int bind_and_listen(int *sockfd) {
    int port;
    struct sockaddr_in server_addr;

    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd < 0) {
        perror("socket");
        return -1;
    }

    srand(time(NULL));
    for (int i = 0; i < 10; i++) {
        port = rand() % (64000 - 1024 + 1) + 1024;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr.sin_port = htons(port);

        if (bind(*sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0) {
            printf("Listening on port %d...\n", port);
            if (listen(*sockfd, 10) == 0) {
                return port;
            } else {
                perror("listen");
            }
        } else {
            perror("bind");
        }
    }

    return -1;
}

int wait_for_connection(int sockfd) {
    int client_sockfd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    printf("Waiting for incoming connections...\n");

    if ((client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
        perror("accept");
        return -1;
    }

    printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    return client_sockfd;
}

int send_message(int sockfd, const char* message) {
    ssize_t send_size = write(sockfd, message, strlen(message));
    if (send_size < 0) {
        perror("write");
        return -1;
    }
    printf("Sent message: %s\n", message);
    return 0;
}

int write_port_to_file(int number, char* file_name) {
    FILE *fp;
    fp = fopen(file_name, "a"); // Open file in append mode
    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }
    fprintf(fp, "%d\n", number); // Write number to file
    fclose(fp); // Close file
    return 0;
}

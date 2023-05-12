#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define BUFFER_SIZE 1024

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


int main() {
    int sockfd_server, sockfd_client;
    int server_port, client_port;

    server_port = bind_and_listen(&sockfd_server);
    if (server_port < 0) {
        printf("Error binding and listening to server port.\n");
        return -1;
    }

    client_port = bind_and_listen(&sockfd_client);
    if (client_port < 0) {
        printf("Error binding and listening to client port.\n");
        return -1;
    }

    // Save the ports to files
    if (write_port_to_file(server_port, "server_port.txt") < 0) {
        printf("Error writing server port number to file.\n");
        return -1;
    }

    if (write_port_to_file(client_port, "http_port.txt") < 0) {
        printf("Error writing client port number to file.\n");
        return -1;
    }

    int server_sockfd = wait_for_connection(sockfd_server);
    if (server_sockfd < 0) {
        printf("Error waiting for server connection.\n");
        return -1;
    }
    printf("Server connected on port %d...\n", server_port);  
    

    int client_sockfd = wait_for_connection(sockfd_client);
    if (client_sockfd < 0) {
        printf("Error waiting for client connection.\n");
        return -1;
    }
    printf("Client connected on port %d...\n", client_port);

    char client_buffer[BUFFER_SIZE];
    ssize_t recv_size;

    // Receive a message from the client
    if ((recv_size = read(client_sockfd, client_buffer, BUFFER_SIZE)) > 0) {
        client_buffer[recv_size] = '\0';
        printf("Received message from client: %s\n", client_buffer);

        // Send the message to the server
        write(server_sockfd, client_buffer, strlen(client_buffer));

        char server_buffer[BUFFER_SIZE];
        ssize_t recv_size_server;

        // Receive a response from the server
        if ((recv_size_server = read(server_sockfd, server_buffer, BUFFER_SIZE)) > 0) {
            server_buffer[recv_size_server] = '\0';
            printf("Received response from server: %s\n", server_buffer);

            // Send the response back to the client
            write(client_sockfd, server_buffer, strlen(server_buffer));
        } else {
            printf("Error receiving response from server.\n");
        }
    } else {
        printf("Error receiving message from client.\n");
    }

    close(client_sockfd);
    close(server_sockfd);
    close(sockfd_client);
    close(sockfd_server);

    return 0;
}

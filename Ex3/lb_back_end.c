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
        port = rand() % (PORT_MAX - PORT_MIN + 1) + PORT_MIN;
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



int write_port_to_file(int number, char* file_name) {
    FILE *fp;
    fp = fopen(file_name, "w"); // Open file in write mode (overwrite existing contents)
    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }
    fprintf(fp, "%d\n", number); // Write number to file
    fclose(fp); // Close file
    return 0;
}


int send_message(int sockfd, const char* message, int server_num, int* server_counters) {
    // Create a new message buffer to include the server activation count
    char new_message[BUFFER_SIZE];
    snprintf(new_message, BUFFER_SIZE, "%s Server %d Activation Count: %d", message, server_num + 1, server_counters[server_num]);

    ssize_t send_size = write(sockfd, new_message, strlen(new_message));
    if (send_size < 0) {
        perror("write");
        return -1;
    }
    printf("Sent message: %s\n", new_message);
    return 0;
}


void handle_client(int client_sockfd, int server_sockfd, int server_num, int* server_counters) {
    char client_buffer[BUFFER_SIZE];
    ssize_t recv_size;

    // Receive a message from the client
    if ((recv_size = read(client_sockfd, client_buffer, BUFFER_SIZE)) > 0) {
        client_buffer[recv_size] = '\0';
        printf("Received message from client: %s\n", client_buffer);

        int server_responded = 0; // Keeps track of whether a server has responded yet

        printf("Sending message to server %d...\n", server_num + 1);
        send_message(server_sockfd, client_buffer, server_num, server_counters);

        char server_buffer[BUFFER_SIZE];
        ssize_t recv_size_server;

        // Receive a response from the current server
        if ((recv_size_server = read(server_sockfd, server_buffer, BUFFER_SIZE)) > 0) {
            server_buffer[recv_size_server] = '\0';
            printf("Received response from server %d: %s\n", server_num + 1, server_buffer);

            // Send the response back to the client
            send_message(client_sockfd, server_buffer, server_num, server_counters);

            server_responded = 1; // Mark that a server has responded
        } else {
            printf("Error receiving message from server %d.\n", server_num + 1);
        }

        printf("Loop executed 1 time.\n");

        if (!server_responded) {
            printf("None of the servers responded.\n");
        }

        // Increment the counter for the current server
        server_counters[server_num]++;

        close(client_sockfd);
    }
}

void run_load_balancer(int* sockfd_servers, int sockfd_client, int client_port) {
    int num_clients = 0; // Counter for number of clients served
    int server_num = 0; // Keeps track of the current server

    int server_counters[NUM_SERVERS];
    memset(server_counters, 0, sizeof(server_counters)); // Initialize counters to zero

    while (1) {
        int client_sockfd = wait_for_connection(sockfd_client);
        printf("Client connected on port %d...\n", client_port);
        handle_client(client_sockfd, sockfd_servers[server_num], server_num, server_counters);
        server_num = (server_num + 1) % NUM_SERVERS; // Move on to the next server
        num_clients++;

        if (num_clients >= MAX_NUM_CLIENTS) {
            printf("Maximum number of clients served. Exiting...\n");
            break;
        }
    }

    // Print the server counters
    int i;
    for (i = 0; i < NUM_SERVERS; i++) {
        printf("Server %d was activated %d times.\n", i + 1, server_counters[i]);
    }

    // Send a message to the servers to close the connection

    close(sockfd_client);
}

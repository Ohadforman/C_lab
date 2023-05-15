#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include "lb_back_end.h"

#define MAX_NUM_CLIENTS 9


void handle_client(int client_sockfd, int server_sockfd, int server_num) {
    char client_buffer[BUFFER_SIZE];
    ssize_t recv_size;

    // Receive a message from the client
    if ((recv_size = read(client_sockfd, client_buffer, BUFFER_SIZE)) > 0) {
        client_buffer[recv_size] = '\0';
        printf("Received message from client: %s\n", client_buffer);

        int server_responded = 0; // Keeps track of whether a server has responded yet

        printf("Sending message to server %d...\n", server_num + 1);
        send_message(server_sockfd, client_buffer);

        char server_buffer[BUFFER_SIZE];
        ssize_t recv_size_server;

        // Receive a response from the current server
        if ((recv_size_server = read(server_sockfd, server_buffer, BUFFER_SIZE)) > 0) {
            server_buffer[recv_size_server] = '\0';
            printf("Received response from server %d: %s\n", server_num + 1, server_buffer);

            // Send the response back to the client
            send_message(client_sockfd, server_buffer);
            server_responded = 1; // Mark that a server has responded
        } else {
            printf("Error receiving message from server %d.\n", server_num + 1);
        }

        printf("Loop executed 1 time.\n");

        if (!server_responded) {
            printf("None of the servers responded.\n");
        }

        close(client_sockfd);
    }
}

void run_load_balancer(int* sockfd_servers, int sockfd_client, int server_port, int client_port) {
    int num_clients = 0; // Counter for number of clients served
    int server_num = 0; // Keeps track of the current server

    while (1) {
        int client_sockfd = wait_for_connection(sockfd_client);
        printf("Client connected on port %d...\n", client_port);
        handle_client(client_sockfd, sockfd_servers[server_num], server_num);
        server_num = (server_num + 1) % 3; // Move on to the next server
        num_clients++;

        if (num_clients >= MAX_NUM_CLIENTS) {
            printf("Maximum number of clients served. Exiting...\n");
            break;
        }
    }

    // Send a message to the servers to close the connection
    char close_msg[] = "Closing connection";
    int i;
    for (i = 0; i < 3; i++) {
        send_message(sockfd_servers[i], close_msg);
        close(sockfd_servers[i]);
    }
    close(sockfd_client);
}





int main() {
    int sockfd_server, sockfd_client;
    int server_port, client_port;
    server_port = bind_and_listen(&sockfd_server);
    client_port = bind_and_listen(&sockfd_client);
    
    // Save the ports to files
    write_port_to_file(server_port, "server_port");
    write_port_to_file(client_port, "http_port");

    int sockfd_servers[3];
    sockfd_servers[0] = wait_for_connection(sockfd_server);
    sockfd_servers[1] = wait_for_connection(sockfd_server);
    sockfd_servers[2] = wait_for_connection(sockfd_server);
    printf("3 Servers connected on port %d...\n", server_port);

    run_load_balancer(sockfd_servers, sockfd_client, server_port, client_port);
    
    return 0;
}

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

void run_load_balancer(int sockfd_server, int sockfd_client, int server_port, int client_port) {
    int num_clients = 0; // Counter for number of clients served

    while (1) {
        int client_sockfd = wait_for_connection(sockfd_client);
        printf("Client connected on port %d...\n", client_port);

        char client_buffer[BUFFER_SIZE];
        ssize_t recv_size;

        // Receive a message from the client
        if ((recv_size = read(client_sockfd, client_buffer, BUFFER_SIZE)) > 0) {
            client_buffer[recv_size] = '\0';
            printf("Received message from client: %s\n", client_buffer);

            // Send the message to the server
            send_message(sockfd_server, client_buffer);

            char server_buffer[BUFFER_SIZE];
            ssize_t recv_size_server;

            // Receive a response from the server
            if ((recv_size_server = read(sockfd_server, server_buffer, BUFFER_SIZE)) > 0) {
                server_buffer[recv_size_server] = '\0';
                printf("Received response from server: %s\n", server_buffer);

                // Send the response back to the client
                send_message(client_sockfd, server_buffer);
            } else {
                printf("Error receiving message from client.\n");
                return;
            }

            close(client_sockfd);
            num_clients++;

            if (num_clients >= MAX_NUM_CLIENTS) {
                printf("Maximum number of clients served. Exiting...\n");
                break;
            }
        }
    }

    // Send a message to the server to close the connection
    char close_msg[] = "Closing connection";
    send_message(sockfd_server, close_msg);
    close(sockfd_server);
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

    int server_sockfd = wait_for_connection(sockfd_server);
    printf("Server connected on port %d...\n", server_port);

    run_load_balancer(server_sockfd, sockfd_client, server_port, client_port);

    return 0;
}

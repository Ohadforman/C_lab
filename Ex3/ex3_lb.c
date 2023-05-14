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

#define MAX_CLIENTS 9

int main() {
    int sockfd_server, sockfd_client;
    int server_port, client_port;
    int num_clients = 0; // Counter for number of clients served

    while (num_clients < MAX_CLIENTS) {
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
            if (send_message(server_sockfd, client_buffer) < 0) {
                printf("Error sending message to server.\n");
                return -1;
            }

            char server_buffer[BUFFER_SIZE];
            ssize_t recv_size_server;

            // Receive a response from the server
            if ((recv_size_server = read(server_sockfd, server_buffer, BUFFER_SIZE)) > 0) {
                server_buffer[recv_size_server] = '\0';
                printf("Received response from server: %s\n", server_buffer);

                // Send the response back to the client
                if (send_message(client_sockfd, server_buffer) < 0) {
                    printf("Error sending message to client.\n");
                    return -1;
                }
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

        num_clients++; // Increment the counter
    }

    return 0;
}

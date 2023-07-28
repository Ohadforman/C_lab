#include "lb_back_end.h"
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#define LOCAL_HOST "127.0.0.1"
#define PORT_MAX 64000
#define PORT_MIN 1024
#define MAX_BUFFER_SIZE 10240
#define DOUBLE_CRLF_LEN 4

int bind_and_listen(int* sockfd)
{
  int port;
  struct sockaddr_in server_addr;

  // Create socket
  *sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (*sockfd < 0) {
    perror("socket");
    return -1;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(LOCAL_HOST);

  // Bind to random port
  while (true) {
    port = rand() % (PORT_MAX - PORT_MIN + 1) + PORT_MIN;
    server_addr.sin_port = htons(port);
    setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    if (bind(*sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0) {
      if (listen(*sockfd, NUM_SERVERS) != -1) {
        return port;
      } else {
        perror("listen");
      }
    }
  }

  return -1;
}

int write_port_to_file(int number, char* file_name)
{
  FILE* fp;
  fp = fopen(file_name, "w");
  if (fp == NULL) {
    return -1;
  }
  fprintf(fp, "%d", number);
  fclose(fp);

  return 0;
}

int wait_for_connection(int sockfd)
{
  int client_sockfd;
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  if ((client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_len)) < 0) {
    perror("accept");
    return -1;
  }

  return client_sockfd;
}

void forward_request_to_server(int server_sockfd, char* client_buffer, ssize_t request_length)
{
  ssize_t send_size = write(server_sockfd, client_buffer, request_length);
  if (send_size < 0) {
    perror("write");
    return;
  }
}

void get_and_forward_server_response(int server_sockfd, int client_sockfd)
{
  char recv_buffer[MAX_BUFFER_SIZE];
  char send_buffer[MAX_BUFFER_SIZE];
  ssize_t bytes_received;
  ssize_t bytes_sent;
  int response_length;
  char* first_occurrence;
  char* second_occurrence;

  // Receive the response from the server
  bytes_received = read(server_sockfd, recv_buffer, MAX_BUFFER_SIZE);
  if (bytes_received < 0) {
    perror("read");
    return;
  }

  // Null-terminate the received data to treat it as a string
  recv_buffer[bytes_received] = '\0';

  // Handle response
  first_occurrence = strstr(recv_buffer, "\r\n\r\n");
  if (first_occurrence != NULL) {
    second_occurrence = strstr(first_occurrence + DOUBLE_CRLF_LEN, "\r\n\r\n");
    if (second_occurrence != NULL) {
      // The response is complete, extract the HTTP response part
      response_length = second_occurrence - recv_buffer + DOUBLE_CRLF_LEN;  // Include the final \r\n\r\n
      strncpy(send_buffer, recv_buffer, response_length);
      send_buffer[response_length] = '\0';

      // Forward the response to the client
      bytes_sent = write(client_sockfd, send_buffer, response_length);
      if (bytes_sent < 0) {
        perror("write");
        return;
      }
    }
  }
}

void handle_client(int client_sockfd, int server_sockfd)
{
  char recv_buffer[MAX_BUFFER_SIZE];
  char send_buffer[MAX_BUFFER_SIZE];
  int bytes_received;
  char* end_of_http_request;
  int request_length;

  while (true) {
    // Receive data from the client
    bytes_received = recv(client_sockfd, recv_buffer, sizeof(recv_buffer) - 1, 0);
    if (bytes_received <= 0) {
      perror("Client disconnected or error occurred");
      break;
    }

    // Null-terminate the received data to treat it as a string
    recv_buffer[bytes_received] = '\0';

    // Find the end of the HTTP request (the double CRLF: \r\n\r\n)
    end_of_http_request = strstr(recv_buffer, "\r\n\r\n");
    if (end_of_http_request != NULL) {
      // The request is complete, extract the HTTP request part
      request_length = end_of_http_request - recv_buffer + DOUBLE_CRLF_LEN;  // Include the final \r\n\r\n
      strncpy(send_buffer, recv_buffer, request_length);
      send_buffer[request_length] = '\0';

      forward_request_to_server(server_sockfd, send_buffer, request_length);
      get_and_forward_server_response(server_sockfd, client_sockfd);
    }
  }
}

void run_load_balancer(int* sockfd_servers, int sockfd_client)
{
  int server_num = 0;  // Keeps track of the current server

  while (true) {
    int client_sockfd = wait_for_connection(sockfd_client);

    handle_client(client_sockfd, sockfd_servers[server_num]);
    server_num = (server_num + 1) % NUM_SERVERS;  // Move on to the next server
  }
}

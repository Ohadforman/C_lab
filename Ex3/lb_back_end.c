#include "lb_back_end.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 10240
#define PORT_MAX 64000
#define PORT_MIN 1024
#define MAX_NUM_CLIENTS 9
#define TRY_CONNECT 10

int bind_and_listen(int* sockfd);
int wait_for_connection(int sockfd);
int write_port_to_file(int number, char* file_name);
void send_response_chunked(int client_sockfd, const char* response, int response_length, int chunk_size);
int send_message(int sockfd, const char* message, int server_num, int* server_counters);
void handle_client(int client_sockfd, int server_sockfd, int server_num, int* server_counters);
void run_load_balancer(int* sockfd_servers, int sockfd_client);

int bind_and_listen(int* sockfd)
{
  int port;
  struct sockaddr_in server_addr;

  *sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (*sockfd < 0) {
    perror("socket");
    return -1;
  }

  srand(time(NULL));
  for (int i = 0; i < TRY_CONNECT; i++) {
    port = rand() % (PORT_MAX - PORT_MIN + 1) + PORT_MIN;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    if (bind(*sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0) {
      if (listen(*sockfd, TRY_CONNECT) == 0) {
        return port;
      } else {
        perror("listen");
      }
    }
  }

  return -1;
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

void send_response_chunked(int client_sockfd, const char* response, int response_length, int chunk_size)
{
  int num_chunks = response_length / chunk_size;
  int remaining_bytes = response_length % chunk_size;
  int total_sent_bytes = 0;

  // Send complete chunks
  for (int i = 0; i < num_chunks; i++) {
    ssize_t send_size = write(client_sockfd, response + total_sent_bytes, chunk_size);
    if (send_size < 0) {
      perror("write");
      return;
    }
    total_sent_bytes += send_size;
    sleep(1);  // Sleep for 1 second between chunks
  }

  // Send remaining bytes
  if (remaining_bytes > 0) {
    ssize_t send_size = write(client_sockfd, response + total_sent_bytes, remaining_bytes);
    if (send_size < 0) {
      perror("write");
      return;
    }
  }
}

int send_message(int sockfd, const char* message, int server_num, int* server_counters)
{
  // Create a new message buffer to include the server activation count
  char new_message[BUFFER_SIZE];
  snprintf(new_message, BUFFER_SIZE, "%s Server %d Activation Count: %d", message, server_num + 1,
           server_counters[server_num]);

  ssize_t send_size = write(sockfd, new_message, strlen(new_message));
  if (send_size < 0) {
    perror("write");
    return -1;
  }
  return 0;
}

void process_received_chunk(char* client_buffer, ssize_t recv_size, ssize_t* total_recv_size, int* num_chunks_received)
{
  *total_recv_size += recv_size;

  // Example: Check if the end of the request has been reached
  if (strstr(client_buffer, "\r\n\r\n") != NULL) {
    // Check if we have received two instances of "\r\n\r\n"
    char* second_occurrence = strstr(client_buffer + *total_recv_size - recv_size, "\r\n\r\n");
    if (second_occurrence != NULL) {
      *num_chunks_received = 2;
    }
  }
}

void forward_request_to_server(int client_sockfd, int server_sockfd, char* client_buffer, ssize_t total_recv_size,
                               int server_num, int* server_counters)
{
  ssize_t send_size = write(server_sockfd, client_buffer, total_recv_size);
  if (send_size < 0) {
    perror("write");
    return;
  }

  // Receive the response from the server
  char server_buffer[BUFFER_SIZE];
  ssize_t recv_size_server = read(server_sockfd, server_buffer, BUFFER_SIZE);
  if (recv_size_server < 0) {
    perror("read");
    return;
  }

  // Forward the response to the client
  ssize_t send_size_client = write(client_sockfd, server_buffer, recv_size_server);
  if (send_size_client < 0) {
    perror("write");
    return;
  }

  // Increment the counter for the current server
  server_counters[server_num]++;
}

void handle_client(int client_sockfd, int server_sockfd, int server_num, int* server_counters)
{
  char client_buffer[BUFFER_SIZE];
  ssize_t recv_size;
  ssize_t total_recv_size = 0;  // Tracks the total size of the received request
  int num_chunks_received = 0;  // Tracks the number of chunks received

  while (num_chunks_received < 2) {
    sleep(1);
    recv_size = read(client_sockfd, client_buffer + total_recv_size, BUFFER_SIZE - total_recv_size);
    if (recv_size < 0) {
      perror("read");
      return;
    } else if (recv_size == 0) {
      // No more data to read, end of data
      return;
    }

    process_received_chunk(client_buffer, recv_size, &total_recv_size, &num_chunks_received);

    sleep(1);
  }

  forward_request_to_server(client_sockfd, server_sockfd, client_buffer, total_recv_size, server_num, server_counters);

  shutdown(client_sockfd, SHUT_RDWR);
}

void send_closing_message(int* sockfd_servers)
{
  const char* closing_message = "Closing connection due to max number of clients reached.";

  // Send the closing message to all servers
  for (int i = 0; i < NUM_SERVERS; i++) {
    ssize_t send_size = write(sockfd_servers[i], closing_message, strlen(closing_message));
    if (send_size < 0) {
      perror("write");
      continue;
    }
    close(sockfd_servers[i]);  // Close the server connection
  }
}

void run_load_balancer(int* sockfd_servers, int sockfd_client)
{
  int num_clients = 0;  // Counter for number of clients served
  int server_num = 0;   // Keeps track of the current server

  int server_counters[NUM_SERVERS];
  memset(server_counters, 0, sizeof(server_counters));  // Initialize counters to zero

  while (1) {
    int client_sockfd = wait_for_connection(sockfd_client);

    handle_client(client_sockfd, sockfd_servers[server_num], server_num, server_counters);
    server_num = (server_num + 1) % NUM_SERVERS;  // Move on to the next server
    num_clients++;

    // If we reached the maximum number of clients send closing msg
    if (num_clients >= MAX_NUM_CLIENTS) {
      send_closing_message(sockfd_servers);
      break;
    }
  }

  close(sockfd_client);
}

#!/usr/bin/python2.7 -tt
#ln -s ex3_server27.py ex3_server
import socket
import os
import sys
import time

count = 0

if len(sys.argv) != 2:
    print "Usage: python server.py 'cat server_port'"
    sys.exit(1)

# Get the port number from the command "cat server_port"
port_command = sys.argv[1]
port_output = os.popen(port_command).read().strip()
print "port_output: %s" % port_output
LB_PORT = int(port_output)

# Replace this with the IP address of the LB you want to connect to
LB_ADDRESS = 'localhost'

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the LB's address and port
print "Connecting to %s:%d..." % (LB_ADDRESS, LB_PORT)
sock.connect((LB_ADDRESS, LB_PORT))
print "Connected!"

# Set a timeout value for the socket
sock.settimeout(1)  # Set timeout to 1 second

# Define the chunk size and delay between chunks
CHUNK_SIZE = 1024
DELAY = 1  # seconds

while True:
    try:
        # Receive a message from the LB
        message = sock.recv(CHUNK_SIZE)

        if not message:
            # No data was received, continue to wait for new messages
            continue

        # Process the received message
        try:
            message_str = message.decode('utf-8', errors='ignore')
        except UnicodeDecodeError:
            continue  # Skip this message if decoding fails
        print "Received message: %s" % message_str

        if "Closing connection" in message_str:
            # Close the socket
            print "Closing connection due to max number of clients reached."
            sock.close()
            print "Disconnected."
            break

        if "GET /counter" in message_str:
            # Update the count
            count += 1

            # Send an HTTP response with the current count
            response_str = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n%d\r\n\r\n" % (len(str(count)), count)
        else:
            # Send a 404 response
            response_str = "HTTP/1.1 404 Not Found\r\nContent-type: text/html\r\nContent-length: 113\r\n\r\n"
            response_str += "<html><head><title>Not Found</title></head><body>\r\n"
            response_str += "Sorry, the object you requested was not found.\r\n"
            response_str += "</body></html>\r\n\r\n"

        # Send the response back to the LB in chunks
        start_index = 0
        while start_index < len(response_str):
            end_index = start_index + CHUNK_SIZE
            chunk = response_str[start_index:end_index]

            # Send the chunk to the LB
            sock.sendall(chunk)
            print "Sent chunk: %s" % chunk

            # Wait for the specified delay between chunks
            time.sleep(DELAY)

            start_index = end_index

    except socket.timeout:
        pass  # no data was received, continue to wait for new messages
    except socket.error as e:
        if e.errno == socket.errno.ECONNRESET:
            continue  # the connection was reset, continue to wait for new messages
        else:
            print "Socket error:", e
            break

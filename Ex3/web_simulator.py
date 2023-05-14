import socket
import sys
import subprocess
import time


# Check if the port is provided as a command-line argument
if len(sys.argv) != 2:
    print("Usage: python3 client.py <port>")
    sys.exit(1)

# Get the port number from the command-line argument
port_command = sys.argv[1]
LB_PORT = int(subprocess.check_output(port_command, shell=True).decode().strip())

# Replace this with the IP address of the LB you want to connect to
LB_ADDRESS = 'localhost'

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Keep trying to connect until successful or max retries reached
max_retries = 10
retry_count = 0
while retry_count < max_retries:
    try:
        # Connect the socket to the LB's address and port
        print(f"Connecting to {LB_ADDRESS}:{LB_PORT}... (Attempt {retry_count+1}/{max_retries})")
        sock.connect((LB_ADDRESS, LB_PORT))
        print("Connected!")
        break
    except (ConnectionRefusedError, OSError) as e:
        print(f"Failed to connect: {e}")
        if retry_count < max_retries-1:
            retry_count += 1
            time.sleep(1)
        else:
            print("Max retries reached. Exiting.")
            sys.exit(1)

# Send a message to the LB
message = "GET /count HTTP/1.1"
print(f"Sending message: {message}")
sock.sendall(message.encode())

# Receive a response from the LB
response = sock.recv(1024)
print(f"Received response: {response.decode()}")

# Close the socket after receiving the first response
sock.close()
print("Disconnected.")

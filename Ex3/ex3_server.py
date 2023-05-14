import socket
import subprocess
import sys
import time

def get_count():
    try:
        with open('count_server1.txt', 'r') as f:
            lines = f.readlines()
            if lines:
                last_line = lines[-1]
                count = int(last_line.strip())
                return count
    except (FileNotFoundError, ValueError):
        pass

    return 0


def save_count(count):
    with open('count_server1.txt', 'w') as f:
        f.write(str(count) + '\n')


def increment_count():
    # Get the current count from the file
    count = get_count()

    # Increment the count
    count += 1

    # Save the new count to the file
    save_count(count)

    # Return the updated count
    return count


if len(sys.argv) != 2:
    print("Usage: python3 server.py 'cat server_port'")
    sys.exit(1)

# Get the port number from the command "cat server_port"
port_command = sys.argv[1]
LB_PORT = int(subprocess.check_output(port_command, shell=True).decode().strip())

# Replace this with the IP address of the LB you want to connect to
LB_ADDRESS = 'localhost'

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the LB's address and port
print(f"Connecting to {LB_ADDRESS}:{LB_PORT}...")
sock.connect((LB_ADDRESS, LB_PORT))
print("Connected!")


while True:
    try:
        # Receive a message from the LB
        message = sock.recv(1024)
        
        if not message:
            # No data was received, continue to wait for new messages
            time.sleep(1)
            continue
        
        # Process the received message
        message_str = message.decode()
        print(f"Received message: {message_str}")
        first_line = message_str.split('\r\n')[0]

        if "Closing connection" in message_str:
            # Close the socket
            print("Closing connection do to max number of clients reached.")
            sock.close()
            print("Disconnected.")
            break

        if "count" in first_line:
            # Increment the counter in count_server.txt
            count = increment_count()-1
            print(f"Incremented count to {count} in count_server1.txt")
            # Send an HTTP response with the current count
            response_str = f"HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nContent-Length: {len(str(count))}\r\n\r\n{count}\r\n\r\n"
        else:
            # Send a 404 Not Found response
            response_str = "HTTP/1.1 404 Not Found\r\nContent-type: text/html\r\nContent-length: 113\r\n\r\n<html><head><title>Not Found</title></head><body>Sorry, the object you requested was not found.</body></html>"

        # Send the response back to the LB
        sock.sendall(response_str.encode())
        print(f"Sent response: {response_str}")
    except socket.timeout:
        pass  # no data was received, continue to wait for new messages
    except ConnectionResetError:
        continue  # the connection was reset, continue to wait for new messages

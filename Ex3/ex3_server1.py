import socket


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
    with open('count_server1.txt', 'a') as f:
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


# Read the last line from the server_port.txt file
with open('server_port.txt', 'r') as f:
    last_line = f.readlines()[-1].strip()
    LB_PORT = int(last_line)

# Replace this with the IP address of the LB you want to connect to
LB_ADDRESS = 'localhost'

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the LB's address and port
print(f"Connecting to {LB_ADDRESS}:{LB_PORT}...")
sock.connect((LB_ADDRESS, LB_PORT))
print("Connected!")


# Receive a response from the LB
response = sock.recv(1024)
print(f"Received response: {response.decode()}")

# Check if the response contains the string "count" in the first line
first_line = response.decode().split('\r\n')[0]

#TODO need to change to after GET only??
if "count" in first_line: 
    # Increment the counter in count_server.txt
    count = increment_count()-1
    print(f"Incremented count to {count} in count_server1.txt")
    # Send an HTTP response with the current count
    response_str = f"HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nContent-Length: {len(str(count))}\r\n\r\n{count}\r\n\r\n"
else:
    # Send a 404 Not Found response
    response_str = "HTTP/1.1 404 Not Found\r\nContent-type: text/html\r\nContent-length: 113\r\n\r\n<html><head><title>Not Found</title></head><body>Sorry, the object you requested was not found.</body></html>"
    
sock.sendall(response_str.encode())

# Close the socket
sock.close()
print("Disconnected.")

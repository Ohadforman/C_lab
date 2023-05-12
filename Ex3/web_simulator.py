import socket

# Read the last line from the http_port.txt file
with open('http_port.txt', 'r') as f:
    last_line = f.readlines()[-1].strip()
    HTTP_PORT = int(last_line)

# Replace this with the IP address of the LB you want to connect to
LB_ADDRESS = 'localhost'

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the LB's address and port
print(f"Connecting to {LB_ADDRESS}:{HTTP_PORT}...")
sock.connect((LB_ADDRESS, HTTP_PORT))
print("Connected!")

# Send a message to the LB
message = "GET \count HTTP/1.1"
print(f"Sending message: {message}")
sock.sendall(message.encode())

# Receive a response from the LB
response = sock.recv(1024)
print(f"Received response: {response.decode()}")

# Close the socket
sock.close()
print("Disconnected.")

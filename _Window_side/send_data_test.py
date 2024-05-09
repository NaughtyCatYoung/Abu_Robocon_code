import socket

# Raspberry Pi's IP address and port
HOST = '192.168.0.101'  # Replace with your Raspberry Pi's IP address
PORT = 12345            # Replace with the port number you chose

client_socket =socket.socket(socket.AF_INET,socket.SOCK_STREAM)
client_socket.connect((HOST, PORT))
print('Connected to Raspberry Pi')

# Send commands to the Raspberry Pi
while True:
    command = input('Enter command (or "exit" to quit): ')
    if command.lower() == 'exit':
        break
    client_socket.sendall(command.encode())

# Close the connection
client_socket.close()

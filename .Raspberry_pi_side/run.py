import socket
import serial
ser = serial.Serial(
    port = '/dev/ttyUSB0',
    baudrate = 115200,
    timeout = 10,
    xonxoff = False
)



while True:
    try:
        # Define host and port
        HOST = '0.0.0.0'  # Raspberry Pi's IP address
        PORT = 12345      # Choose a port number

        # Create a socket object
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        # Bind the socket to the host and port
        server_socket.bind((HOST, PORT))

        # Listen for incoming connections
        server_socket.listen(1)

        print('Waiting for connection...')

        # Accept a connection
        client_socket, client_address = server_socket.accept()
        print('Connected to', client_address)

        while True:
            # Receive data from the client
            data = client_socket.recv(1024)
            if not data:
                break

            # Process the received data (e.g., control the robot)
            # Here you can interpret the received data and perform corresponding actions
            print(data.decode())
            command=data.decode()
            ser.write(command.encode(encoding = 'ascii', errors = 'strict'))       

        # Close the connection
        client_socket.close()
        server_socket.close()
    except Exception as ex:
        print(ex)

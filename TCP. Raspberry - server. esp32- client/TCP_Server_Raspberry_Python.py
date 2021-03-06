import socket
from time import sleep

SERVER_ADDRESS = '192.168.2.1'
SERVER_PORT = 48984

# Create the server socket
server_socket = socket.socket()
server_socket.bind((SERVER_ADDRESS, SERVER_PORT))
server_socket.listen(10)

print("Server " + str(SERVER_ADDRESS) + " is running")

connection, address = server_socket.accept()
print("New connection from {address}".format(address=address))

# Send data to server
connection.send("Start_Transmission_Data")

while True:

    # Receive response from server
    data = connection.recv(1024)
    if not data:
        print("Server abended. Exiting")
        break
    
    IMU_data = data.split("A")

    for i in range(len(IMU_data)):
	x = IMU_data[i].split("F")
	print(x)                                                                                                              
	i=+1

    sleep(0.5)

connection.close()

#Socket Code modified from https://docs.python.org/release/2.6.5/library/socketserver.html
#Bryce Egley
import csv
import socket
import sys

HOST = raw_input('Enter HOST name: ')
PORTTemp = raw_input('Enter PORT: ')
PORT = int(PORTTemp)
#HOST,PORT = "localhost", 9999
#data = " ".join(sys.argv[1:])

# Create a socket (SOCK_STREAM means a TCP socket)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
handler = raw_input('Enter your handle: ')
print('Type two messages to begin chat')
# Connect to server and send data
data = "start"
sock.connect((HOST, PORT))
while (data != "\quit"):
    data = raw_input(handler + "> ")
    sock.send(handler + "> " + data + "\n")
    # Receive data from the server
    received = sock.recv(1024)
    print received[:len(received)/2]
print("Ending connection...")

#Close connection
sock.close()

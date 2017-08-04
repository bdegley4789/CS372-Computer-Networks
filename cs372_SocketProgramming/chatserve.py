#Socket Code from https://docs.python.org/release/2.6.5/library/socketserver.html
#Bryce Egley
import SocketServer
import csv
import sys

handler = "Server"
i = 0

class MyTCPHandler(SocketServer.BaseRequestHandler):
    """
        The RequestHandler class for our server.
        
        It is instantiated once per connection to the server, and must
        override the handle() method to implement communication to the
        client.
        """
    def handle(self):
        # self.request is the TCP socket connected to the client
        self.data = self.request.recv(1024).strip()
        if i == 0:
            connect = "Connection created"
            self.request.send(connect)
            print("Connection created")
            global i
            i+=1
        else:
            #print "%s wrote:" % self.client_address[0]
            print self.data
            # Your message to send back
            reply = raw_input(handler + "> ")
            if reply == "\quit":
                print("Ending connection...")
                quit()
            global handler
            self.request.send(handler + "> " + reply)

if __name__ == "__main__":
    HOST = raw_input('Enter HOST name: ')
    PORTtemp = raw_input('Enter PORT name: ')
    global handler
    handler = raw_input('Type your handle: ')
    PORT = int(PORTtemp)
    #HOST, PORT = "localhost", 9999
    
    # Create the server, binding to localhost on port 9999
    server = SocketServer.TCPServer((HOST, PORT), MyTCPHandler)
    print("Waiting on client to connect...")
    
    # Activate the server; this will keep running until you
    # interrupt the program with Ctrl-C
    server.serve_forever()

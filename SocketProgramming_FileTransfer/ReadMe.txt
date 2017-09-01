By Bryce Egley

To compile fileServer.c
Type gcc fileServer.c -o fileServer
on sftp://access.engr.oregonstate.edu server
ignore any warnings

Run  fileServer.c
with ./fileServer 9999
Or choose another port
For Handle type "Server"

Now run fileClient.py 
with python fileClient.py
Enter "localhost" for hostname
Enter 9999 for port or the same port number
For handle type "Client"

Type two messages on client to start a conversation

Type -l to display file contents
Type -g "filename" to get a file

Notes
On the -g filename there is an error with the C string so I just set the filename
to "test.txt"  
displaying the filename next to each individual character in the file.
Also for commands to work you make need to type one more message between
the server and client to initiate the servers response to a command

If you type any other -command it will show error message

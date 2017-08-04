//Socket code from http://www.linuxhowtos.org/data/6/client.c
//Bryce Egley
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <netdb.h> 
#include <string.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int main(int argc, char *argv[])
{
    char host[100];
    char port[100];
    char server_reply[2000];
    //Get host name
    printf("Enter hostname:");
    fflush(stdin);
    scanf("%s",host);
    fflush(stdin);
    //Get port name
    printf("Enter Port:");
    fflush(stdin);
    scanf("%s",port);
    fflush(stdin);
    char buffer[700];
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    portno = atoi(port);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    //Create Client Handle, 10 chars
    char handle[600];
    //Size 510 for handle(10 chars) and message(500 chars)
    char handleMess[510];
    printf("Please enter your handle: ");
    scanf("%s",handle);
    //500 character message
    char message[500];
    char temp[500];
    strcpy(temp,"quit");
    printf("Please enter a message to start connection: ");
    bzero(buffer,256);
    fflush(stdin);
    //Clear stdin buffer
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    //scanf("%s",message);
    //Reads line of 500 characters
    fgets(message,500,stdin);
    //strcpy(handleMess,strcat(handle,message));
    //strcpy(buffer,handleMess);
    strcpy(buffer,message);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0)
        error("ERROR reading from socket");
    //printf("%s\n",buffer);
    //multiple messages over socket from https://stackoverflow.com/questions/4172538/sending-multiple-messages-via-send-recv-socket-programming-c
    while(1) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        printf("%s> ",handle);
        char mess[500];
        char tempbuff[700];
        char end[500];
        strcpy(end,"\quit");
        fgets(mess,500,stdin);
        //This doesn't work
        if (strcmp(mess,end) == 0) {
            printf("Ending connection");
            exit(0);
        }
        //strcpy(tempbuff,strcat(handle,mess));
        stpcpy(stpcpy(stpcpy(tempbuff,handle),"> "),mess);
        connect(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr));
        send(sockfd, tempbuff, strlen(tempbuff), 0);
        
        //Recieve a reply
        //Tutorial http://www.binarytides.com/server-client-example-c-sockets-linux/
        if( recv(sockfd, server_reply, 2000, 0) < 0) {
            error("ERROR recieve failed");
        }
        printf("%s\n",server_reply);
        memset(&server_reply[0], 0, sizeof(server_reply));
    }
    close(sockfd);
    return 0;
}

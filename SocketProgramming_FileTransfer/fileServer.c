//Socket code modified from http://www.linuxhowtos.org/data/6/server.c
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
#include <dirent.h>
#include <stdint.h>
#include <netdb.h>
#include <pthread.h>
/* A simple server in the internet domain using TCP
 The port number is passed as an argument */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    int m;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    error("ERROR opening socket");
    char handle[600];
    printf("Please enter your handle: ");
    scanf("%s",handle);
    fflush(stdin);
    printf("Waiting on client to connect...\n");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
    error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                       (struct sockaddr *) &cli_addr,
                       &clilen);
    if (newsockfd < 0)
    error("ERROR on accept");
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    fflush(stdin);
    printf("Connected\n");
    fflush(stdin);
    printf("%s",buffer);
    fflush(stdin);
    n = write(newsockfd,"Connection initiatedConnection initiate",41);
    if (n < 0) error("ERROR writing to socket");
    fflush(stdin);
    int first = 0;
    while(1) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        int special = 0;
        fflush(stdin);
        if (first == 1) {
            printf("%s> ", handle);
        }
        char mess[500];
        char tempbuff[700];
        char listtemp[1000000];
        fgets(mess,500,stdin);
        //fflush();
        if (first == 1) {
            strtok(mess, "\n");
            stpcpy(stpcpy(stpcpy(tempbuff,handle),"> "),mess);
        } else {
            strtok(tempbuff, "\n");
            strcpy(tempbuff,"Starting");
        }
        first = 1;
        strtok(tempbuff, "\n");
        n = write(newsockfd,tempbuff,800);
        //memset(&n[0], 0, sizeof(n));
        m = read(newsockfd,buffer,700);
        printf("%s",buffer);
        if (strstr(buffer,"-l") != NULL) {
            special = 1;
            n = 0;
            printf("List\n");
            //Open directory code modified from tutorial http://www.sanfoundry.com/c-program-list-files-directory/
            DIR * d;
            struct dirent * dir;
            d = opendir(".");
            if (d)
            {
                while ((dir = readdir(d)) != NULL)
                {
                    printf("%s\n", dir->d_name);
                    strcat(listtemp, dir->d_name);
                    strcat(listtemp, "\n");
                }
                closedir(d);
            }
            n = write(newsockfd,listtemp,2000000);
            memset(&listtemp[0], 0, sizeof(listtemp));
            
        }
        if (strstr(buffer,"-g") != NULL) {
            special = 1;
            printf("File(Get)\n");
            //Send file code modified from tutorial http://www.geeksforgeeks.org/c-program-print-contents-file/
            FILE *fptr;
            char filename[100], c;
            memcpy( filename, &buffer[11], 9 );
            filename[11] = '\0';
            //filename = buffer.substr(buffer.find(" ") + 1);
            printf("File: %s",filename);
            fptr = fopen(filename, "r");
            if (fptr == NULL)
            {
                printf("Cannot open file \n");
                exit(0);
            }
            // Read contents from file
            c = fgetc(fptr);
            while (c != EOF)
            {
                char temp9[10];
                printf ("%c", c);
                strcat(listtemp, &c);
                strtok(tempbuff, "test.txt");
                c = fgetc(fptr);
            }
            fclose(fptr);
            n = write(newsockfd,listtemp,2000000);
            memset(&listtemp[0], 0, sizeof(listtemp));
        }
        if (strstr(buffer,"-") != NULL && strstr(buffer,"-g") == NULL && strstr(buffer,"-l") == NULL) {
            special = 1;
            n = 0;
            printf("Error: Invalid Command(-l for list -g for File get)\n");
            n = write(newsockfd,"Error: Invalid Command(-l for list -g for File get)\n",200);
        }
        memset(&buffer[0], 0, sizeof(buffer));
        memset(&mess[0], 0, sizeof(mess));
        memset(&tempbuff[0], 0, sizeof(tempbuff));
        if (special == 0) {
            if (n < 0) error("ERROR writing to socket");
        }
        special = 0;
        n = 0;
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}

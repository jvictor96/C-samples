#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit, atoi, malloc, free */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netdb.h> /* struct hostent, gethostbyname */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */

void error(const char *msg) { perror(msg); exit(0); }

int main(int argc,char *argv[])
{

    int i;
    struct hostent *server;
    struct sockaddr_in serv_addr;
    int bytes, sent, received, total, message_size;
    char *message, response[4096];
    int portno = atoi(argv[2])>0?atoi(argv[2]):80;
    char *host = strlen(argv[1])>0?argv[1]:"localhost";
    char *path = strlen(argv[4])>0?argv[4]:"/";
    if (argc < 5) { puts("Parameters: <host> <port> <method> <path> [<data> [<headers>]]"); exit(0); }
    /* How big is the message? */
    message_size=0;
    if(!strcmp(argv[3],"GET"))
    {
                printf("Process 1\n");
        message_size+=strlen("%s %s%s%s HTTP/1.0\r\nHost: %s\r\n");        /* method         */
        message_size+=strlen(argv[3]);                         /* path           */
        message_size+=strlen(path);                         /* headers        */
        if(argc>5)
            message_size+=strlen(argv[5]);                     /* query string   */
        for(i=6;i<argc;i++)                                    /* headers        */
            message_size+=strlen(argv[i])+strlen("\r\n");
        message_size+=strlen("\r\n");                          /* blank line     */
    }
    else
    {
                printf("Process 2\n");
        message_size+=strlen("%s %s HTTP/1.0\r\nHost: %s\r\n");
        message_size+=strlen(argv[3]);                         /* method         */
        message_size+=strlen(path);                            /* path           */
        for(i=6;i<argc;i++)                                    /* headers        */
            message_size+=strlen(argv[i])+strlen("\r\n");
        if(argc>5)
            message_size+=strlen("Content-Length: %d\r\n")+10; /* content length */
        message_size+=strlen("\r\n");                          /* blank line     */
        if(argc>5)
            message_size+=strlen(argv[5]);                     /* body           */
    }
            printf("Allocating...\n");
    /* allocate space for the message */
    message=malloc(message_size);

    /* fill in the parameters */
    if(!strcmp(argv[3],"GET"))
    {
        if(argc>5)
            sprintf(message,"%s %s%s%s HTTP/1.0\r\nHost: %s\r\n",
                strlen(argv[3])>0?argv[3]:"GET",               /* method         */
                path,                                          /* path           */
                strlen(argv[5])>0?"?":"",                      /* ?              */
                strlen(argv[5])>0?argv[5]:"",host);            /* query string   */
        else
            sprintf(message,"%s %s HTTP/1.0\r\nHost: %s\r\n",
                strlen(argv[3])>0?argv[3]:"GET",               /* method         */
                path,host);                                    /* path           */
        for(i=6;i<argc;i++)                                    /* headers        */
            {strcat(message,argv[i]);strcat(message,"\r\n");}
        strcat(message,"\r\n");                                /* blank line     */
    }
    else
    {
        sprintf(message,"%s %s HTTP/1.0\r\nHost: %s\r\n",
            strlen(argv[3])>0?argv[3]:"POST",                  /* method         */
            path,host);                                        /* path           */
        for(i=6;i<argc;i++)                                    /* headers        */
            {strcat(message,argv[i]);strcat(message,"\r\n");}
        if(argc>5)
            sprintf(message+strlen(message),"Content-Length: %d\r\n",(int)strlen(argv[5]));
        strcat(message,"\r\n");                                /* blank line     */
        if(argc>5)
            strcat(message,argv[5]);                           /* body           */
    }
    printf("Processed\n");
    /* What are we going to send? */
    printf("Request:\n%s\n",message);
        /* lookup the ip address */

    total = strlen(message);
    /* create the socket */
    #ifdef __linux__ 
    int sockfd;
    server = gethostbyname(host);
    if (server == NULL) error("ERROR, no such host");
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) error("ERROR opening socket");
        /* fill in the structure */
        memset(&serv_addr,0,sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(portno);
        memcpy(&serv_addr.sin_addr.s_addr,server->h_addr_list[0],server->h_length);
                /* connect the socket */
        if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
            error("ERROR connecting");
                /* send the request */

    sent = 0;
    do {
        bytes = write(sockfd,message+sent,total-sent);
        if (bytes < 0)
            error("ERROR writing message to socket");
        if (bytes == 0)
            break;
        sent+=bytes;
    } while (sent < total);
    /* receive the response */
    memset(response, 0, sizeof(response));
    total = sizeof(response)-1;
    received = 0;
    printf("Response: \n");
    do {
       printf("%s", response);
       memset(response, 0, sizeof(response));
       bytes = recv(sockfd, response, 1024, 0);
        if (bytes < 0)
           printf("ERROR reading response from socket");
       if (bytes == 0)
           break;
       received+=bytes;
    } while (1);

    if (received == total)
        error("ERROR storing complete response from socket");

    /* close the socket */
    close(sockfd);
    #endif


    free(message);

    return 0;
}
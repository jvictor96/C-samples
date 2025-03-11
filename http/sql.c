#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit, atoi, malloc, free */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netdb.h> /* struct hostent, gethostbyname */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */

void error(const char *msg) { perror(msg); exit(0); }

void write_int32(char *buffer, int32_t value, int p0) {
    buffer[p0+0] = (value >> 24) & 0xFF;
    buffer[p0+1] = (value >> 16) & 0xFF;
    buffer[p0+2] = (value >> 8)  & 0xFF;
    buffer[p0+3] = (value)       & 0xFF;
}

void build_startup_message(char *buffer, char* user, char* database, int size) {
    int offset = 8;

    strcpy(buffer + offset, "user");
    offset += strlen("user") + 1;

    strcpy(buffer + offset, user);
    offset += strlen(user) + 1;

    strcpy(buffer + offset, "database");
    offset += strlen("database") + 1;

    strcpy(buffer + offset, database);
    offset += strlen(database) + 1;

    strcpy(buffer + offset, "replication");
    offset += strlen("replication") + 1;

    strcpy(buffer + offset, "false");
    offset += strlen("false") + 1;

    buffer[offset] = '\0'; 
    write_int32(buffer, size, 0);                                
    write_int32(buffer, 196608, 4);
}

void write_to_fd(char* message, int fd, int total) {
    int sent = 0;
    int bytes = 0;
    do {
        bytes = write(fd,message+sent,total-sent);
        if (bytes < 0)
            error("ERROR writing message to socket");
        if (bytes == 0)
            break;
        printf("%d\n", bytes);
        printf("bytes written to %d:", fd);
        for (int i = 0; i < bytes; i++) { // Print first 10 bytes for debugging
            printf("%02X", (unsigned char)message[i]);
        }
        printf("\n");
        sent+=bytes;
    } while (sent < total);
}

void read_from_fd(int fd, int total, char* response) {
    int bytes = 0,
        received = 0;
    printf("%d\n", bytes);
    memset(response, 0, sizeof(response));
    printf("%d\n", bytes);
    do {
        bytes = recv(fd, response, 4096, 0);
        if (bytes < 0)
           printf("ERROR reading response from socket");
        if (bytes == 0)
           break;
        printf("%d\n", bytes);
        received+=bytes;
    } while (1);

    if (received == total)
        error("ERROR storing complete response from socket");
}

int my_connect(struct hostent *server, char* host, int portno, struct sockaddr_in serv_addr) {
    server = gethostbyname(host);
    if (server == NULL) error("ERROR, no such host");
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
    /* fill in the structure */
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr_list[0],server->h_length);
            /* connect the socket */
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    return sockfd;
}

int main(int argc,char *argv[])
{

    int i;
    struct hostent *server;
    struct sockaddr_in serv_addr;
    int received;
    char *message, response[4096];
    int portno = 5432;
    char *host = "localhost";
    char *user = argv[1];
    char *database = argv[2];
    int sockfd = my_connect(server, host, portno, serv_addr);
    
    int startup_size = 8 + strlen("user") + strlen(user) + 2;
    startup_size += strlen("database") + strlen(database) + 2;
    startup_size += strlen("replication") + strlen("false") + 3;
    message = malloc(startup_size);

    build_startup_message(message, user, database, startup_size);
    printf("Connecting...\n");
    write_to_fd(message, sockfd, startup_size);
    printf("Connection OK: \n");

    /* receive the response */
    printf("Response: \n");
    read_from_fd(sockfd, sizeof(response), response);
    printf("read end\n");
    write_to_fd(response, STDOUT_FILENO, sizeof(response));

    char input[100];
    printf("Type your query: \n");
    while ( fgets(input, sizeof(input), stdin) && strcmp(input, "exit")) {
        free(message);
        printf("input len %d\n", strlen(input));
        message = malloc(strlen(input) + 5);
        sprintf(message, "Q    %s\0", input);
        write_int32(message, strlen(input) + 5, 1);
        printf("sprinted %s\n", message);
        write_to_fd(message, sockfd, strlen(input) + 6);
        printf("Query OK: \n");

        
        read_from_fd(sockfd, sizeof(response), response);
        write_to_fd(response, STDOUT_FILENO, sizeof(response));
        printf("Type your query: \n");
    }

    printf("\n");

    /* close the socket */
    close(sockfd);


    free(message);

    return 0;
}
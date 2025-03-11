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

void build_query_message(char *buffer, char* query) {
    sprintf(buffer,"Q    %s", query);          
    write_int32(buffer, strlen(query) + 5, 1);                             
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
    do {
       printf("%s", response);
       memset(response, 0, sizeof(response));
       bytes = recv(fd, response, 1024, 0);
        if (bytes < 0)
           printf("ERROR reading response from socket");
       if (bytes == 0)
           break;
       received+=bytes;
    } while (1);

    if (received == total)
        error("ERROR storing complete response from socket");
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
    char *path = "/";
    char *user = argv[1];
    char *database = argv[2];

    /* create the socket */
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

    int startup_size = 8 + strlen("user") + strlen(user) + 2;
    startup_size += strlen("database") + strlen(database) + 2;
    startup_size += strlen("replication") + strlen("false") + 3;
    message = malloc(startup_size);

    build_startup_message(message, user, database, startup_size);
    write_to_fd(message, STDOUT_FILENO, startup_size);
    printf("\n");

    printf("connecting...\n");
    printf("socketfd %d\n", sockfd);
    write_to_fd(message, sockfd, startup_size);
    printf("Connection OK: \n");

    /* receive the response */
    printf("Response: \n");
    received = recv(sockfd, response, sizeof(response) - 1, 0);
    if (received < 0) {
        perror("recv failed");
    } else {
        response[received] = '\0';  // Null-terminate for safety
        printf("%s \n", response);  // Print the raw bytes received
    }

    printf("\n");
    free(message);
    message = malloc(strlen("CREATE TABLE PRODUCTS(id int, name varchar(255))") + 5);
    printf("malloqued\n");
    build_query_message(message, "Q    CREATE TABLE PRODUCTS(id int, name varchar(255))");
    printf("built\n");

    write_to_fd(message, sockfd, strlen(message));
    printf("Query OK: \n");

    /* close the socket */
    close(sockfd);


    free(message);

    return 0;
}
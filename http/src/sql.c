#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit, atoi, malloc, free */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netdb.h> /* struct hostent, gethostbyname */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */

#include <json.h>
#include <sql.h>

struct message {
    int size;
    char* content;
};

void error(const char *msg) { perror(msg); exit(0); }

void write_int32(char *buffer, int32_t value, int p0) {
    buffer[p0+0] = (value >> 24) & 0xFF;
    buffer[p0+1] = (value >> 16) & 0xFF;
    buffer[p0+2] = (value >> 8)  & 0xFF;
    buffer[p0+3] = (value)       & 0xFF;
}

void to_json(struct row *head, char** header, struct element **result) {
    struct element *new_element = malloc(sizeof(struct element));
    new_element->type = LIST;
    *result = new_element;
    while(head) {
        new_element->list = malloc(sizeof(struct element));
        new_element = new_element->list;
        new_element->type = DICT;
        new_element->list = malloc(sizeof(struct element));
        new_element = new_element->list;
        new_element->value = header[0];
        new_element->type = STRING;
        new_element->next = malloc(sizeof(struct element));
        new_element = new_element->next;
        new_element->value = head->values[0];
        new_element->type = STRING;
        for(int i = 1; i < head->column_amount; i++) {
            new_element->next = malloc(sizeof(struct element));
            new_element = new_element->next;
            new_element->value = header[i];
            new_element->type = STRING;
            new_element->next = malloc(sizeof(struct element));
            new_element = new_element->next;
            new_element->value = head->values[i];
            new_element->type = STRING;
        }
        head = head->next_row;
    }
}

struct message build_startup_message(char* user, char* database) {
    int offset = 8, size = 15;
    char* fields[6] = {
        "user", user,
        "database", database,
        "replication", "false"
    };
    for(int i = 0; i < 6; i ++) {
        size += strlen(fields[i]);
    }
    char* buffer = malloc(128);
    for(int i = 0; i < 6; i ++) {
        strcpy(buffer + offset, fields[i]);
        offset += strlen(fields[i]) + 1;
    }

    buffer[offset] = '\0'; 
    write_int32(buffer, size, 0);                                
    write_int32(buffer, 196608, 4);

    struct message m;
    m.size = size;
    m.content = malloc(128);
    m.content = buffer;
    return m;
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
        sent+=bytes;
    } while (sent < total);
}

void handle_login(char *response) {
    if (response[8] == 0x00) {
        printf("Login SUCCEDED\n");
    }
    if (response[8] == 0x03) {
        printf("Clear text PASSWORD required\n");
    }
    if (response[8] == 0x05) {
        printf("MD5 PASSWORD required\n");
        printf("Salt: %c", response[9]);
    }
}

int handle_select(char* response, struct row **rows, int size) {
    int column_amount = (response[5] << 8) | response[6],
        offset;
    int dataSizes[column_amount], dataTypes[column_amount];
    char columnNames[column_amount][24];
    offset = 7;

    for(int i = 0; i < column_amount; i++) {
        int nameSize = 0;
        while (response[offset] != 0x00) {
            columnNames[i][nameSize] = (char)response[offset];
            nameSize++;
            offset++;
        }
        columnNames[i][nameSize] = 0x00;
        offset+=7;
        dataTypes[i] = (response[offset] << 24) | (response[offset + 1] << 16) | (response[offset + 2] << 8) | response[offset + 3];
        dataSizes[i] = (response[offset + 4] << 8) | response[offset + 5];
        offset+=12;
    }
    
    if(response[offset] == 'C') return 1;

    struct row* head = NULL;
    struct row *tail = NULL;
    while (response[offset] == 'D') {
        struct row *one_row = malloc(sizeof(struct row));
        one_row->column_amount = column_amount;
        one_row->values = malloc(column_amount * sizeof(char*));
        one_row->next_row = NULL;
        
        offset++;
        int rowSize = (response[offset ] << 24) | (response[offset + 1] << 16) | (response[offset + 2] << 8) | response[offset + 3];
        offset += 6;
        for(int i = 0; i < column_amount; i++) {
            int columnSize = (response[offset] << 24) | (response[offset + 1] << 16) | (response[offset + 2] << 8) | response[offset + 3];
            one_row->values[i] = malloc(columnSize + 1);
            offset+=4;
            strncpy(one_row->values[i], &response[offset], columnSize);
            offset+=columnSize;
            one_row->values[i][columnSize] = '\0';
        }
        
        if(!head) {
            tail = one_row;
            head = one_row;
        } else {
            tail->next_row = one_row;
            tail = one_row;
        }
    }
    *rows = head;
    return 0;
}

int read_from_fd(int fd, struct row** rows) {
    int bytes = 0,
        received = 0,
        offset = 0;
    char response[1024];
    memset(response, 0, sizeof(response));
    do {
        bytes = recv(fd, response, 4096, 0);
        if (bytes > 0) {
            if(response[0] == 'R') {
                handle_login(response);
            }
            if(response[0] == 'T') {
                return handle_select(response, rows, bytes);
            } else {
                int size = (response[1] << 24) | (response[2] << 16) | (response[3] << 8) | response[4];
                for (int i = 5; i < size; i++) {
                    //printf("%02X", (char)response[i]);
                    //printf("%c", (char)response[i]);
                }
                //printf("\n");
            }
            //printf("\n");
            break;
        }
        //printf("%d\n", bytes);
        received+=bytes;
    } while (1);
        
    return 1;
}

int my_connect(char* host, int portno, char* user, char* database) {
    struct hostent *server;
    struct sockaddr_in serv_addr;
    struct message message;
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
    
    int startup_size = 8 + strlen("user") + strlen(user) + 2;
    startup_size += strlen("database") + strlen(database) + 2;
    startup_size += strlen("replication") + strlen("false") + 3;
    message = build_startup_message(user, database);
    printf("Connecting...\n");
    write_to_fd(message.content, sockfd, message.size);
    read_from_fd(sockfd, NULL);
    printf("Connected\n");

    free(message.content);
    return sockfd;
}

void execute_query(char* input, int sockfd, struct row **result) {
    struct message message;
    message.size = strlen(input) + 5;
    message.content = malloc(message.size);
    sprintf(message.content, "Q    %s\0", input);

    write_int32(message.content, message.size, 1);
    write_to_fd(message.content, sockfd, message.size + 1);
    if(read_from_fd(sockfd, result) == 0) {
        struct row *head = *result;
        struct row *node_ptr = *result;
        while(node_ptr) {
            node_ptr = node_ptr->next_row;
        }
        *result = head;
    }
    free(message.content);
}

void free_result(struct row **result) {
    while (*result) {
        struct row *row;
        row = *result; // Save next node
        struct row *next; // Save next node
        if(row) {
            next = row->next_row;
            for (int i = 0; i < row->column_amount; i++) {
                free(row->values[i]); // Free each column value
            }
            free(row->values); // Free the array of values
            free(row);         // Free the row struct itsel
        }
        *result = next;        // Move to next node
    }
    *result = malloc(sizeof(struct row));
}
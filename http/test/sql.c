#include <sql.h>
#include <stdio.h> /* printf, sprintf */

int main(int argc,char *argv[])
{
    int portno = 5432;
    char *host = "localhost";
    char *user = argv[1];
    char *database = argv[2];

    int sockfd = my_connect(host, portno, user, database);

    char input[100];
    printf("Type your query: \n");
    while ( fgets(input, sizeof(input), stdin) && strcmp(input, "exit")) {
        struct row *result;
        execute_query(input, sockfd, result);
        free_result(result);
        printf("Type your query: \n");
    }

    printf("\n");

    close(sockfd);

    return 0;
}
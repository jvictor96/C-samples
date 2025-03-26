#include <repository.h>
#include <sql.h>
#include <stdio.h> /* printf, sprintf */
#include <unistd.h> /* read, write, close */
#include <stdlib.h> /* exit, atoi, malloc, free */

int main(int argc,char *argv[])
{
    int portno = 5432;
    char *host = "localhost";
    char *user = argv[1];
    char *database = argv[2];

    int sockfd = my_connect(host, portno, user, database);

    ddl(sockfd);
    post_product("cocacola", "4", sockfd);
    post_product("itaipava", "3", sockfd);
    post_product("heineken", "7.5", sockfd);

    struct element *result;
    get_all(&result, sockfd);

    char *get_all_result = malloc(512);
    printf("serialize\n");
    serialize(result, ' ', &get_all_result);
    printf("serialize\n");
    get_by_name("cocacola", &result, sockfd);
    
    close(sockfd);

    return 0;
}
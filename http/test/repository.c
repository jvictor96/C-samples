#include <repository.h>
#include <sql.h>
#include <unistd.h> /* read, write, close */

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
    struct row *result;
    get_all(&result, sockfd);
    free_result(&result);
    get_by_name("cocacola", &result, sockfd);
    free_result(&result);
    close(sockfd);

    return 0;
}
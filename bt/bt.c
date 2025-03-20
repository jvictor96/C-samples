#include <stdio.h>
#include <string.h>
#include <stdio.h> /* printf, sprintf */
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>

int main(int argc, char **argv)
{
    struct sockaddr_l2 loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);
    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
    if(s == -1) {
        printf("failute on socket");
    }
    // bind socket to port 0x1001 of the first available 
    // bluetooth adapter
    loc_addr.l2_family = AF_BLUETOOTH;
    loc_addr.l2_bdaddr = *BDADDR_ANY;
    loc_addr.l2_psm = htobs(0x1001);
    if(bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) == -1) {
        printf("failute on bind");
    }
    // put socket into listening mode
    if(listen(s, 1) == -1) {
        printf("failute on listen");
    }
    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);
    if(client == -1) {
        printf("failute on accept");
    }
    printf("client: %d\n", client);
    ba2str( &rem_addr.l2_bdaddr, buf );
    printf("accepted connection from %s\n", buf);

    memset(buf, 0, sizeof(buf));

    // read data from the client
    while(1) {
        bytes_read = recv(client, buf, sizeof(buf), 0);
        if( bytes_read > 0 ) {
            printf("received [%s]\n", buf);
        }
    }

    // close connection
    close(client);
    close(s);
}
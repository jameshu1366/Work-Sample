//
//  Behnam Dezfouli
//  CSEN, Santa Clara University
//

// This program implements a server that accepts connection from a client and copies the received bytes to a file
//
// The input arguments are as follows:
// argv[1]: Sever's port number



#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>


int main (int argc, char *argv[])
{
    char    message[10] = "received!";  // message to be sent to the client when the destination file name is received
    int     net_bytes_read;             // numer of bytes received over socket
    int     socket_fd = 0;              // socket descriptor
    int     connection_fd = 0;          // new connection descriptor
    struct  sockaddr_in serv_addr;      // Address format structure
    char    net_buff[1024];             // buffer to hold characters read from socket
    FILE    *dest_file;                 // pointer to the file that will include the received bytes over socket
    
    
    if (argc < 2) // Note: the name of the program is counted as an argument
    {
        printf ("Port number not specified!\n");
        return 1;
    }

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("cannot create socket");
        return 0;
    }

    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_port        = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);

    int bindresult;
    bindresult = bind(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (bindresult < 0) {
        perror("bind failed");
        return 1;
    }
    
    if(listen(socket_fd,5) < 0){
        perror("listen failed");
        return 1;
    }

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int new_socket;
    
    while(1){
        new_socket= accept(socket_fd, (struct sockaddr *)&client_addr, &client_len);
        if (new_socket == -1) {
            perror("Accept failed");
            continue;
        }
        printf("Client connected\n");
        break;
    }

    // Read dest file name from client and open file
    ssize_t r;
    r = read(new_socket, net_buff, 10);
    if(r <= 0){
        perror("Read fail 1.");
        return 1;
    }
    FILE *file = fopen(net_buff, "wb");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
    
    // Send received to server
    ssize_t w;
    w = write(new_socket, "received!", strlen("received!"));
    if (w < 0){
        error("ERROR writing to socket");
        return 0;
    }

    // Write data into file
    while((r = read(new_socket, net_buff, sizeof(net_buff))) > 0){
        if(fwrite(net_buff, 1, r, file) <= 0){
            perror("fwrite fail.");
            return 1;
        }
        
        //printf("A");
    }
    
    
    fclose(file);
    close(socket_fd);
}


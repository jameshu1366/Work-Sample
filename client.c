//
//  Behnam Dezfouli
//  CSEN, Santa Clara University
//

// This program implements a client that connects to a server and transfers the bytes read from a file over the established connection
//
// The input arguments are as follows:
// argv[1]: Sever's IP address
// argv[2]: Server's port number
// argv[3]: Source file
// argv[4]: Destination file at the server side which includes a copy of the source file



#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define    SIZE 10

int main (int argc, char *argv[])
{
    int        sockfd = 0;              // socket descriptor
    char       net_buff[SIZE];          // to hold the bytes read from socket
    char       file_buff[SIZE];         // to hold the bytes read from source file
    struct     sockaddr_in serv_addr;   // server address structure
    int        net_bytes_read;          // numer of bytes received over socket
    FILE       *source_file;            // pointer to the source file
    
    if (argc < 5)
    {
        printf ("Usage: ./%s <server ip address> <server port number> <source file>  <destination file>\n", argv[0]);
        return 1;
    }
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("cannot create socket");
        return 0;
    }

    // Make sure intial values are zero
    memset (net_buff, '0', sizeof (net_buff));
    memset (file_buff, '0', sizeof (file_buff));
    memset (&serv_addr, '0', sizeof (serv_addr));
    
    // Setting up serv_addr
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_port        = htons(atoi(argv[2]));
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    // Connect
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0){
        perror("Connection failed");
        return 1;
    }

    // Send dest file name to server
    ssize_t w;
    w = write(sockfd, argv[4], SIZE);
    if (w < 0){
        error("ERROR writing to socket");
        return 0;
    }

    bzero(net_buff, SIZE);

    // Read received from server
    ssize_t r;
    r = read(sockfd, net_buff, SIZE);
    if(r <= 0){
        perror("Read fail 1.");
        return 1;
    }else{
        net_buff[r] = '\n';
        printf("%s",net_buff);
    }

    // Open input file
    FILE *file = fopen(argv[3], "rb");
    if(file == NULL){
        perror("File Open failed");
        return 0;
    }

    // Send data to server
    while((r = fread(file_buff, 1, 10, file))>0){
        w = write(sockfd, file_buff, r);
        if (w < 0){
            error("ERROR writing to socket");
            return 0;
        }
        memset(file_buff, 0, SIZE);
        
    };
    
    

    
    
    
    
    
    
    
    //fclose(s_file);
    //fclose(d_file);
    close(sockfd);
    
    return 0;
}


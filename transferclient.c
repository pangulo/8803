#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
 
#define BUFSIZE 256 //4096 
 
#define USAGE                                                                 \
"usage:\n"                                                                    \
"  transferclient [options]\n"                                                \
"options:\n"                                                                  \
"  -s                  Server (Default: localhost)\n"                         \
"  -p                  Port (Default: 8888)\n"                                \
"  -o                  Output file (Default foo.txt)\n"                       \
"  -h                  Show this help message\n"                             
 
/* Main ========================================================= */
int main(int argc, char **argv) {
    int option_char = 0;
    char *hostname = "localhost";
    unsigned short portno = 8888;
    char *filename = "foo.txt";
    struct sockaddr_in server_addr;
    struct hostent *server;
    int sockC;
    char recbuf[BUFSIZE];
    int numRec;
    int enable = 1;
    size_t numWrite, x;
 
    // Parse and set command line arguments
    while ((option_char = getopt(argc, argv, "s:p:o:h")) != -1) {
        switch (option_char) {
            case 's': // server
                hostname = optarg;
                break; 
            case 'p': // listen-port
                portno = atoi(optarg);
                break;                                        
            case 'o': // filename
                filename = optarg;
                break;       
            case 'h': // help
                fprintf(stdout, "%s", USAGE);
                exit(0);
                break;       
            default:
                fprintf(stderr, "%s", USAGE);
                exit(1);
        }
    }
 
    /* Socket Code Here */
    server = gethostbyname(hostname);
 
    //open socket for client
    sockC = socket(AF_INET, SOCK_STREAM, 0);

    //set value of server_addr to 0
    memset(&server_addr, 0, sizeof(server_addr));
 
    //create socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portno);

    //copies length bytes from h_addr to s_addr.
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);

    setsockopt(sockC, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

    //connects to server
    connect(sockC, (struct sockaddr *) &server_addr, sizeof(server_addr));
        
    //opens filename and writes
    FILE *fp = fopen(filename,"w");

    memset(&recbuf,0,sizeof(recbuf));

    while(1){
        //fprintf(stdout,"rec bytes begin: %d\n",numRec);
        numRec = recv(sockC, recbuf, BUFSIZE, 0);
        if(numRec == 0){
            break;}
        //fprintf(stdout,"rec bytes after: %d\n",numRec);
        numWrite = fwrite(recbuf, sizeof(char), numRec, fp);
        x = x + numWrite;
        //fprintf(stdout, "total written bytes: %zu\n",x);
        //fprintf(stdout, "rec string: %s\n", recbuf);
        memset(&recbuf,0,sizeof(recbuf));
        //fprintf(stdout,"rec bytes 2: %d\n",numRec);
    }
    close(sockC);
    return 0;
}
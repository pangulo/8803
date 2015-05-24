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
    int n = 0;
    int x;
 
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
    //if (sockC<0)
    //    printf("error opening socket");
 
    //set value of server_addr to 0
    memset(&server_addr, 0, sizeof(server_addr));
 
    //create socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portno);
    //copies length bytes from h_addr to s_addr.
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
 
    connect(sockC, (struct sockaddr *) &server_addr, sizeof(server_addr));
    //    printf("error with connection");
 
     FILE *fp = fopen(filename,"w");

     //       printf("error opening file");
     memset(&recbuf,0,sizeof(recbuf));
     //printf("test\n");
     while(n == 0){
        //numRec = recv(sockC, recbuf, BUFSIZE, 0);
        //printf("%d\n",numRec);
        while((numRec = recv(sockC, recbuf, BUFSIZE, 0))){
       //     printf("%d\n",numRec);
            //while num of bytes received is > 0
       //     printf("test3\n");
       //     printf("%d\n",numRec);
       //     printf("%s\n", recbuf);
            fwrite(recbuf, sizeof(char), numRec, fp);
            //memset(&recbuf,0,sizeof(recbuf));
       //     printf("%d\n",numRec);
            //numRec = 0;
            if(numRec < 0){
                break;}
            else if(numRec == 0){
                break;}
        }
        //printf("%d\n",numRec);
        //printf("ok!\n");
        n = 1;
        fclose(fp);
     }
close(sockC);
return 0;
}
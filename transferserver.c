#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
 
#if 0
/* 
 * Structs exported from netinet/in.h (for easy reference)
 */
 
/* Internet address */
struct in_addr {
  unsigned int s_addr; 
};
 
/* Internet style socket address */
struct sockaddr_in  {
  unsigned short int sin_family; /* Address family */
  unsigned short int sin_port;   /* Port number */
  struct in_addr sin_addr;   /* IP address */
  unsigned char sin_zero[...];   /* Pad to size of 'struct sockaddr' */
};
 
/*
 * Struct exported from netdb.h
 */
 
/* Domain name service (DNS) host entry */
struct hostent {
  char    *h_name;        /* official name of host */
  char    **h_aliases;    /* alias list */
  int     h_addrtype;     /* host address type */
  int     h_length;       /* length of address */
  char    **h_addr_list;  /* list of addresses */
}
#endif
 
#define BUFSIZE 256//4096 
 
#define USAGE                                                                 \
"usage:\n"                                                                    \
"  transferserver [options]\n"                                                \
"options:\n"                                                                  \
"  -p                  Port (Default: 8888)\n"                                \
"  -f                  Filename (Default: bar.txt)\n"                         \
"  -h                  Show this help message\n"                             
 
int main(int argc, char **argv) {
  int option_char, sz, sockS, sockS_new;
  int portno = 8888; /* port to listen on */
  int enable = 1;
  int n = 0;
  size_t numRead, numSent;
  size_t x=0; //keeps track of bytes sent
  char *filename = "bar.txt"; /* file to transfer */
  char stbuf[BUFSIZE]; // buffer
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_len;

  // Parse and set command line arguments
  while ((option_char = getopt(argc, argv, "p:f:h")) != -1){
    switch (option_char) {
      case 'p': // listen-port
        portno = atoi(optarg);
        break;                                        
      case 'f': // listen-port
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

  //open socket for server
  sockS = socket(AF_INET, SOCK_STREAM,0);

  //create socket
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(portno);
  server_addr.sin_addr.s_addr = INADDR_ANY;
 
  //setsockopt(sockS, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

  while(1){
    setsockopt(sockS, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
    bind(sockS,(struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(sockS, 5);

    sockS_new = accept(sockS, (struct sockaddr *) &client_addr, &client_len);     //accept new connnection
    FILE *fp = fopen(filename,"r");    //open file to transfer
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);   //find size of the file to be sent
    fseek(fp, 0L, SEEK_SET);    //set seek back to start
    fprintf(stdout,"numRead1: %zd\n", numRead);

    while(x<sz){
      numRead = fread(stbuf, sizeof(char), BUFSIZE, fp); 
      fprintf(stdout, "%s\n", stbuf);
      fprintf(stdout,"file size: %d\n",sz);
      fprintf(stdout,"numRead2: %zd\n", numRead);
      numSent = send(sockS_new, stbuf, numRead, 0);
      x = x + numSent;
      fprintf(stdout,"total sent: %zd\n",x);
      memset(&stbuf,0, sizeof(stbuf));
      if(x==sz){
        printf("sockS closed\n");
        close(sockS_new);
        break;}
    }
    memset(&stbuf,0, sizeof(stbuf));
    x = 0;
    fprintf(stdout,"numRead3: %zd\n", numRead);
  }
  return 0; 
}

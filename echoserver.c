#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

#define BUFSIZE 4096

#define USAGE                                                                 \
"usage:\n"                                                                    \
"  echoserver [options]\n"                                                    \
"options:\n"                                                                  \
"  -p                  Port (Default: 8888)\n"                                \
"  -n                  Maximum pending connections\n"                         \
"  -h                  Show this help message\n"                              
                      

int main(int argc, char **argv) {
  int option_char, sockS, sockS_new;
  int portno = 8888; /* port to listen on */
  char buffer[16];
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_len;
  int maxnpending = 5;
  int enable = 1;

  // Parse and set command line arguments
  while ((option_char = getopt(argc, argv, "p:n:h")) != -1){
    switch (option_char) {
      case 'p': // listen-port
        portno = atoi(optarg);
        break;                                        
      case 'n': // server
        maxnpending = atoi(optarg);
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
  if(sockS < 0)
    printf("error with socket");
  //bzero((char *) &server_addr, sizeof(server_addr));

  //create socket
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(portno);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  
  setsockopt(sockS, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

  //bind
  if(bind(sockS,(struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    printf("Error on binding");

  //listen
  listen(sockS, maxnpending);
  //printf("%s\n","Server running: waiting for connections.");
  while(1){
  //accept incoming connection
    client_len = sizeof(client_addr);
    sockS_new = accept(sockS, (struct sockaddr *) &client_addr, &client_len);
    if(sockS_new < 0)
      printf("Error on acceptance");

    memset(&buffer, 0, sizeof(buffer));
    //bzero(buffer, 16);
    recv(sockS_new, buffer, sizeof(buffer),0);
    fprintf(stdout,"%s\n", buffer);
    send(sockS_new, buffer, sizeof(buffer),0);}

return 0;
}
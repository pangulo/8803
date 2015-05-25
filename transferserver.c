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
#include <errno.h>
 
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
 
#define BUFSIZE 2//4096 
 
#define USAGE                                                                 \
"usage:\n"                                                                    \
"  transferserver [options]\n"                                                \
"options:\n"                                                                  \
"  -p                  Port (Default: 8888)\n"                                \
"  -f                  Filename (Default: bar.txt)\n"                         \
"  -h                  Show this help message\n"                             
 
int main(int argc, char **argv) {
  int option_char, bRead;
  int portno = 8888; /* port to listen on */
  char *filename = "bar.txt"; /* file to transfer */
  int sockS, sockS_new;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_len;
  int enable = 1;
  char stbuf[BUFSIZE]; // buffer
  int numRead;
  int n = 0;
 
 
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
  //printf("%s\n","test" );
  //create socket
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(portno);
  server_addr.sin_addr.s_addr = INADDR_ANY;
 
  setsockopt(sockS, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
   
  //bind
  bind(sockS,(struct sockaddr *) &server_addr, sizeof(server_addr));
  //  printf("Error on binding\n");
 
  //listen
  listen(sockS, 5);
 
  //printf("%s\n","Server running: waiting for connections.");
  sockS_new = accept(sockS, (struct sockaddr *) &client_addr, &client_len);
  //  printf("error on accept");
 
  //open file to transfer
  FILE *fp = fopen(filename,"r");
  
  //bzero(stbuf, BUFSIZE); //clearing buffer
  memset(&stbuf,0, sizeof(stbuf));
  while(n == 0){
    while((numRead = fread(stbuf, sizeof(char), BUFSIZE, fp))){
      //while bytes read from file returns num byte read >0
        printf("%d\n",numRead);
        printf("%s\n", stbuf);

        if(feof(fp)){
          printf("eof\n");
          printf("%d\n",feof(fp));
          printf("%d\n",close(sockS_new));}
        printf("%d\n", errno);
        send(sockS_new, stbuf, numRead, 0);

          //shutdown(sockS_new,1);
          //break;
        //printf("%s\n", stbuf);
        //  printf("Failed to send file");
        //bzero(stbuf, BUFSIZE);//clear buffer
        memset(&stbuf,0, sizeof(stbuf));
        // if(numRead < 0){
        //   break;}
        // else if(numRead == 0){
        //   break;}
    }

    printf("ok!\n");
    //printf("%d\n",shutdown(sockS_new,1));
    n = 1;
  }
  //printf("%s\n", "test");
  close(sockS_new);
  return 0;
 
}
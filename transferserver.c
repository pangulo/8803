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
  size_t numRead;
  int n = 0;
  size_t x=0;
 
 
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
  //sockS_new = accept(sockS, (struct sockaddr *) &client_addr, &client_len);
  //  printf("error on accept");
  
  //sockS_new = accept(sockS, (struct sockaddr *) &client_addr, &client_len);
 
  //open file to transfer
  FILE *fp = fopen(filename,"r");
  
  //bzero(stbuf, BUFSIZE); //clearing buffer
  memset(&stbuf,0, sizeof(stbuf));
    //setsockopt(sockS, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
  sockS_new = accept(sockS, (struct sockaddr *) &client_addr, &client_len);
  fprintf(stdout, "beginning to send\n");
  fprintf(stdout, "numread before: %zd\n",numRead);
  while(n==0){
    fprintf(stdout, "eof1? %d\n",feof(fp));
    if(numRead==0)
      break;
    fprintf(stdout, "!eof!? %d\n",!feof(fp));
    while(!feof(fp)){
    //while((numRead = fread(stbuf, sizeof(char), BUFSIZE, fp)) ){
      numRead = fread(stbuf, sizeof(char), BUFSIZE, fp);
      //x = x + numRead;
      fprintf(stdout, "%s\n", stbuf);
      fprintf(stdout,"x: %zd\n",x);
      fprintf(stdout,"numRead: %zd\n", numRead);
      fprintf(stdout, "eof2? %d\n",feof(fp));
      send(sockS_new, stbuf, numRead, 0);
      //fprintf(stdout,"sent: %zd\n",send(sockS_new, stbuf, numRead, 0));
      memset(&stbuf,0, sizeof(stbuf));
      if(feof(fp)){
        fprintf(stdout, "eof3? %d\n",feof(fp));
        n = 1;
        close(sockS_new);}
      if(numRead==0)
        break;
    }
    // // while(numRead>0){
    // //   fprintf(stdout,"numread after: %zd\n",numRead);
    // //   //while bytes read from file returns num byte read >0
    // //   fprintf(stdout,"%s\n", "eof check");
    // //   fprintf(stdout, "eof? %d\n",feof(fp));
    // //   if(feof(fp)){
    // //       fprintf(stdout, "eoff? %d\n",feof(fp));
    // //       fprintf(stdout, "%d\n",close(sockS_new));
    // //       break;}
    //   //printf("%zd\n",numRead);
    //   //printf("%s\n", stbuf);
    //   send(sockS_new, stbuf, numRead, 0);
    //   fprintf(stdout,"numread after send: %zd\n",numRead);
    //   memset(&stbuf,0, sizeof(stbuf));
    // }
    // if(feof(fp)){
    //   fprintf(stdout, "eoff? %d\n",feof(fp));
    //   fprintf(stdout, "%d\n",close(sockS_new));}
     n = 1;
    // close(sockS_new);
    // fprintf(stdout, "ok!\n");
  }
  if(feof(fp)){
    fprintf(stdout, "eof4? %d\n",feof(fp));
    close(sockS_new);}
  return 0; 
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <getopt.h>

/* Be prepared accept a response of this length */
#define BUFSIZE 4096

#define USAGE                                                                 \
"usage:\n"                                                                    \
"  echoclient [options]\n"                                                    \
"options:\n"                                                                  \
"  -s                  Server (Default: localhost)\n"                         \
"  -p                  Port (Default: 8888)\n"                                \
"  -m                  Message to send to server (Default: \"Hello World!\"\n"\
"  -h                  Show this help message\n"                              

/* Main ========================================================= */
int main(int argc, char **argv) {
	int option_char = 0;
	char *hostname = "localhost";
	unsigned short portno = 8888;
	char *message = "Hello World!";
	char buffer[16];
	int sockC;
	struct sockaddr_in server_addr;
	struct hostent *server;

	// Parse and set command line arguments
	while ((option_char = getopt(argc, argv, "s:p:m:h")) != -1) {
		switch (option_char) {
			case 's': // server
				hostname = optarg;
				break; 
			case 'p': // listen-port
				portno = atoi(optarg);
				break;                                        
			case 'm': // server
				message = optarg;
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
	if (sockC<0)
		printf("error opening socket");

	//set value of server_addr to 0
 	memset(&server_addr, 0, sizeof(server_addr));

 	//create socket
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portno);
	//copies length bytes from h_addr to s_addr.
	bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);

	//connect the client to the socket error if issue
	if(connect(sockC, (struct sockaddr *) &server_addr, sizeof(server_addr))<0){
		printf("error with connection");
		exit(0);}

	send(sockC, message, sizeof(buffer), 0);
	recv(sockC, buffer, sizeof(buffer),0);
	fprintf(stdout, "%s\n", buffer);
	return 0;

}
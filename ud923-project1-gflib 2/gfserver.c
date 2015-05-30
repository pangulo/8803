#include <unistd.h>
#include "gfserver.h"

/* 
 * Modify this file to implement the interface specified in
 * gfserver.h.
 */

ssize_t gfs_sendheader(gfcontext_t *ctx, gfstatus_t status, size_t file_len){
	char message[256];
	if(status == 400 /*&& file_len =0 ?*/){
		//snprintf(messge, sizeof(messge), "%zu", status);
		//fprintf(stdout, "GETFILE GF_FILE_NOT_FOUND");
		//fflush(stdout)
		send(ctx,"GETFILE GF_FILE_NOT_FOUND", sizeof(message));
	}
	return  0;

}

ssize_t gfs_send(gfcontext_t *ctx, void *data, size_t len){

	send(ctx, data, len, 0);

}

void gfs_abort(gfcontext_t *ctx){
	fprintf(stderr,"error1");
	close(ctx);

}

gfserver_t* gfserver_create(){
	unsigned short port;
	int max_npending;
	void sockfd = socket(AF_INET, SOCK_STREAM,0);

}

void gfserver_set_port(gfserver_t *gfs, unsigned short port){
	gfs.port -> port;

}
void gfserver_set_maxpending(gfserver_t *gfs, int max_npending){
	gfs.max_npending -> max_npending;

}

void gfserver_set_handler(gfserver_t *gfs, ssize_t (*handler)(gfcontext_t *, char *, void*)){
	//(*handler = &get_handler;
	handler(gfs.sockfd, gf.content)


}

void gfserver_set_handlerarg(gfserver_t *gfs, void* arg){


}

void gfserver_serve(gfserver_t *gfs){
	struct sockaddr_in server_addr, client_addr;

	while(1){
		//open socket for server
	  	sockfd = socket(AF_INET, SOCK_STREAM,0);

		//create socket
	  	server_addr.sin_family = AF_INET;
	  	server_addr.sin_port = htons(gfs.portno);
	  	server_addr.sin_addr.s_addr = INADDR_ANY;	

	  	//bind
	  	bind(sockfd,(struct sockaddr *) &server_addr, sizeof(server_addr));

	  	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

	  	//listen on socket
	  	listen(sockfd, gfs.maxpending);

	  	sockfdNew = accept(sockfd, (struct sockaddr *) &client_addr, &client_len);
	  }


	
}


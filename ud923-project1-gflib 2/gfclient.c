/* 
 * Modify this file to implement the interface specified in
 * gfclient.h.
 */


#include "gfclient.h"


gfcrequest_t *gfc_create(){
	unsigned short port;
	char server;
	char path;
	size_t dataLen;
	void file;


}

void gfc_set_server(gfcrequest_t *gfr, char* server){
	gfr.server -> server;

  
}

void gfc_set_path(gfcrequest_t *gfr, char* path){
	gfr.path -> path;
  
}

void gfc_set_port(gfcrequest_t *gfr, unsigned short port){
	gfr.port -> port;
  
}

void gfc_set_headerfunc(gfcrequest_t *gfr, void (*headerfunc)(void*, size_t, void *)){
  
}

void gfc_set_headerarg(gfcrequest_t *gfr, void *headerarg){
  
}

void gfc_set_writefunc(gfcrequest_t *gfr, void (*writefunc)(void*, size_t, void *)){
	writefunc(void, gfr.dataLen, void);
  
}

void gfc_set_writearg(gfcrequest_t *gfr, void *writearg){
	gfr.file = writearg;
  
}

int gfc_perform(gfcrequest_t *gfr){
	struct sockaddr_in server_addr;

	server = gethostbyname(hostname);

	server_addr.sin_family =AF_INET;
	server_addr.sin_port = htons(gfr->port);
	bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
	fprintf(stderr, "info: %s %d \n", gfr->server, gfr->port);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	int con =  connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    fprintf(stderr, "con %d\n", con);

  
}

gfstatus_t gfc_get_status(gfcrequest_t *gfr){
	char status;

}

char* gfc_strstatus(gfstatus_t status){
	

}

size_t gfc_get_filelen(gfcrequest_t *gfr){

}

size_t gfc_get_bytesreceived(gfcrequest_t *gfr){

}

void gfc_cleanup(gfcrequest_t *gfr){

}

void gfc_global_init(){
}

void gfc_global_cleanup(){
}

#include <sys/socket.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>

int main(int argc, char **argv){
	char* port = argv[1];
	
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int sockfd = socket(AF_INET, SOCK_STREAM,0);
	bind(sockfd,(const struct sockaddr *)&serverAddress,sizeof(serverAddress));
	
	return 0;
}



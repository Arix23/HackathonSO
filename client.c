/*
Ari Valenzuela - A01635584
Nataly Hernández - A01631314
Diego Vazquez - A00226803
*/
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

//flag para indicar al thread o al main para terminar el programa
int flag = 0;

void *RecibirMensajes(void* sockfd){
	int clientSocket = *((int *) sockfd);
	while(1){
		if(flag==1){
			break;
		}
		char data[250];
		int read = recv(clientSocket,data,250,0);
		data[read] = '\0';
		printf("%s",data);
		if(strcmp(data,"\nBye desde el server\n")==0){
			close(sockfd);
			flag = 1;
			break;
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char **argv){
	char* ip = argv[1];
	int port = atoi(argv[2]);
	char* username = argv[3];
	
	int clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;

	struct hostent *serverInfo = gethostbyname(ip);
	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	bcopy((char *)serverInfo->h_addr,
        (char *)&serverAddr.sin_addr.s_addr, serverInfo->h_length);
	
	int sockfd = socket(AF_INET, SOCK_STREAM,0);
    if(connect(sockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr))<0){
        printf("ERROR\n");
    }
	
	//Una vez conectado, manda mensaje de conectado al chat
	char bienvenida[250];
	strcpy(bienvenida, "");
	strcat(bienvenida, username);
	strcat(bienvenida, " se ha unido al chat...\n");
	send(sockfd,bienvenida,strlen(bienvenida),0);
	
	pthread_t thread;
	pthread_create(&thread, NULL, RecibirMensajes, (void *) &sockfd );
	
	
	while(1){
		
		if(flag==1){
			break;
		}
		
		char c;
		char message[250];
		char bye[250];
		char byebye[250];
		char despedida[250];
		char complete_message[250];
		
		//terminar cliente
		strcpy(byebye, "Bye\n");
		
		strcpy(despedida, "");
		strcat(despedida, username);
		strcat(despedida, " se ha salido del chat...\n");
		strcpy(bye,username);
		strcat(bye,": ");
		strcat(bye,"bye\n");
		
		
		strcpy(complete_message, "");
		strcat(complete_message, username);
		strcat(complete_message, ": ");
		fgets(message,250,stdin);
		strcat(complete_message, message);
		
		if(strcmp(complete_message,bye)==0){
			send(sockfd,byebye,strlen(byebye),0);
			send(sockfd,complete_message,strlen(complete_message),0);
			send(sockfd,despedida,strlen(despedida),0);
			close(sockfd);
			flag=1;
			break;
			
		} else {
			if(flag==1){
				break;
			}
			send(sockfd,complete_message,strlen(complete_message),0);
		}
		
	}
	pthread_exit(NULL);
	
	
	
	return 0;
}

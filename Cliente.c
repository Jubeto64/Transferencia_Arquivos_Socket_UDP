#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//Headers de socket
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(){
	int sd;
	char fileName[2000], file_buffer[2000],c,caufile[70000],audio[50000],video[50000];

	struct sockaddr_in remoteServAddr;

	//Criando socket
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sd < 0){
	    printf("Erro na criação do socket\n");
	    exit(1);
	}

    //Limpando o remoteServAddr
	memset(&remoteServAddr, 0, sizeof(remoteServAddr));

	//Adicionando informações do server
	remoteServAddr.sin_family = AF_INET;
	remoteServAddr.sin_port = htons(8000);
	remoteServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
    char num = 1;
    sendto(sd, &num, sizeof(num), 0,(struct sockaddr *)&remoteServAddr, sizeof(struct sockaddr));

    printf("Digite o nome do arquivo: \n");
    scanf("%s",fileName);
    sendto(sd, fileName, strlen(fileName), 0,(struct sockaddr *)&remoteServAddr, sizeof(struct sockaddr));

    FILE *fp;
    fp=fopen(fileName,"r");

    if(fp){
        printf("Reading file contents.\n");
     	fseek(fp,0,SEEK_END);
       	size_t file_size=ftell(fp);
       	fseek(fp,0,SEEK_SET);
      	if(fread(file_buffer,file_size,1,fp)<=0){
           	printf("Unable to copy file into buffer or empty file.\n");
           	exit(1);
        }
    }
    else{
    	printf("Cannot open file.\n");
    	exit(0);
    }
    printf("FILE CONTENTS TO SEND : %s\n",file_buffer);
    if(sendto(sd, file_buffer, strlen(file_buffer), 0,(struct sockaddr *)&remoteServAddr, sizeof(struct sockaddr))<0){
    	printf("FILE WAS NOT SENT\n");
    }
    else{
    	printf("FILE SENT\n");
    }
    fclose(fp);
}

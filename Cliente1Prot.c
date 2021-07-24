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
    int socketServer, socketCliente;
    char fileName[2000], file_buffer[2000],c,caufile[70000],audio[50000],video[50000];
    char IP[16];

    struct sockaddr_in servaddr, cliaddr, cliaddr1, cliaddr2;

    //Criando socket com o server
    socketServer = socket(AF_INET, SOCK_DGRAM, 0);
    if(socketServer < 0){
        printf("Erro na criação do socket\n");
        exit(1);
    }

    //Criando socket com o cliente
    socketCliente = socket(AF_INET, SOCK_DGRAM, 0);
    if(socketCliente < 0){
        printf("Erro na criação do socket\n");
        exit(1);
    }

    //Limpando o endereço
    memset(&servaddr, 0, sizeof(servaddr));

    //Adicionando informações do server
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8000);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    char num = 1;
    int len = sizeof(servaddr);
    sendto(socketServer, &num, sizeof(num), 0,(struct sockaddr *)&servaddr, sizeof(struct sockaddr));

    printf("Digite o nome do arquivo: \n");
    scanf("%s", fileName);
    sendto(socketServer, fileName, strlen(fileName), 0,(struct sockaddr *)&servaddr, sizeof(struct sockaddr));

    recvfrom(socketServer, IP, 1024,0,(struct sockaddr *)&servaddr, &len);
    printf("IP: %s\n", IP);

    int opcao = 1;

    while(opcao != 3){
        printf("          Menu          \nEscolha uma opção: \n1. Enviar arquivo\n2. Receber arquivo\n3. Sair");
        printf("Digite a opção que deseja: ");
        scanf("%d", &opcao);

        switch(opcao){

            case 1:
                recvfrom(socketCliente, fileName, strlen(fileName), 0, (struct sockaddr *)&cliaddr2, &len);

                FILE *fp;
                fp = fopen(fileName, "r");

                if(fp){
                    fseek(fp, 0, SEEK_END);
                    size_t file_size = ftell(fp);
                    fseek(fp, 0, SEEK_SET);
                    if(fread(file_buffer, file_size, 1, fp) <= 0){
                        printf("Unable to copy file into buffer or empty file");
                        exit(1);
                    }
                }
                else{
                    printf("Cannot open file\n");
                    exit(0);
                }
                printf("Conteúdo a ser enviado: %s\n", file_buffer);
                if(sendto(socketCliente, file_buffer, strlen(file_buffer), 0, (struct sockaddr *)&cliaddr1, sizeof(struct sockaddr)) < 0){
                    printf("Arquivo não foi enviado\n");
                }
                else{
                    printf("Arquivo enviado\n");
                }
                fclose(fp);
                break;

            case 2:

                //Setando o endereço
                cliaddr2.sin_family = AF_INET;
                cliaddr2.sin_port = htons(8000);
                cliaddr2.sin_addr.s_addr = htonl(INADDR_ANY);

                sendto(socketCliente, fileName, strlen(fileName), 0,(struct sockaddr *)&cliaddr1, sizeof(struct sockaddr));

                recvfrom(socketServer, IP, 1024,0,(struct sockaddr *)&servaddr, &len);
                printf("IP: %s\n", IP);

                FILE *fp2;
                printf("Contents in the received text file : \n");
                recvfrom(socketCliente,file_buffer,1024,0,(struct sockaddr *)&cliaddr1, &len);
                printf("%s\n",file_buffer);
                int fsize = strlen(file_buffer);
                fp2 = fopen(fileName,"w");
                if(fp){
                    fwrite(file_buffer, fsize, 1, fp);
                    printf("File received successfully.\n");
                }
                else{
                    printf("Cannot create to output file.\n");
                }
                memset(fileName, '\0', sizeof(fileName));
                fclose(fp);

            case 3:
                fclose(fp);
                break;
        }
    }
}

#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#define maxlen 70000
int main()
{
    char fileName[100];
    char filebuffer[2000],caufile[maxlen], string[50];
    char *vfilep;
    int sd,connfd,len;

    for(int i=0;i<=100;i++){
        fileName[i]='\0';
    }
    struct sockaddr_in servaddr, cliaddr;

    sd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sd==-1)
    {
        printf(" socket not created in server\n");
        exit(0);
    }
    else
    {
        printf("socket created in  server\n");
    }

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8000);
    memset(&(servaddr.sin_zero),'\0',8);
    if ( bind(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0 )
        printf("Not binded\n");
    else
        printf("Binded\n");

    len=sizeof(cliaddr);

    int choice = 1;
    int pertence = 0;
    while(1)
    {
        char num;

        recvfrom(sd,&num,sizeof(num),0,(struct sockaddr *)&cliaddr, &len);


        recvfrom(sd,fileName,1024,0,(struct sockaddr *)&cliaddr, &len);
        printf("NAME OF TEXT FILE RECEIVED : %s\n",fileName);

        FILE *in_file;
        in_file = fopen("arquivos.txt", "r");
        if (in_file == NULL){
            printf("Arquivo não encontrado\n");
            exit(-1);
        }

        while(fscanf(in_file,"%s", string) == 1){
            if(strstr(string, fileName)!= 0){
                pertence = 1;
            }
        }

        if(pertence == 1){
            sendto(sd, fileName, 1024, 0,(struct sockaddr *)&cliaddr, len);
        }
    }
    return(0);
}

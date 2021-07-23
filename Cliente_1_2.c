//Eu não cheguei a testar se a parte do segundo cliente está criando, mas foi a única coisa que achei a respeito

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <windows.h>
#include <winsock.h>

int main(int argc, char *argv[]){
    WSADATA wsaData;
    LPHOSTENT hostEntry;

    int socks, rc, i, fd, choice = 1;
    char afileName[2000], vfileName[2000], file_buffer[2000],c,caufile[70000],aufile[7000000],vfile[1000000];
    struct sockaddr_in	cliAddr, remoteServAddr;

    WSAStartup(MAKEWORD(2,1), &wsaData);

    hostEntry = gethostbyname(argv[1]);
    if(hostEntry == NULL){
        printf("Host desconhecido %s\n", argv[1]);
        return 1;
    }

    // Creating socket file descriptor
    if ( (fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("Erro");
        exit(EXIT_FAILURE);
    }

    memset(&remoteServAddr, 0, sizeof(remoteServAddr));

    // Filling server information
    remoteServAddr.sin_family = AF_INET;
    remoteServAddr.sin_port = htons(8000);
    remoteServAddr.sin_addr.s_addr = INADDR_ANY;

    //Segundo https://stackoverflow.com/questions/15053072/udp-socket-programming-in-c-2-clients-and-1-server?answertab=votes#tab-top
    //Parece que é assim que se cria um segundo cliente, se eu entendi direito

    struct sockaddr_in si_client2;

        memset((char *) &si_client2, 0, sizeof(si_client2));
        si_client2.sin_family = AF_INET;
        si_client2.sin_port = htons(CLIENT2_PORT);
        if(inet_aton(CLIENT2_HOST, &si_client2.sin_addr) == 0)
        printf("Erro\n");

    //Eu não entendi muito bem a parte do servidor, ai se vcs puderem dar uma olhada no link

    while(choice != 4)
    {
        printf("          MENU          ");
        printf("1. Musica");
        printf("2. Video");
        printf("3. Sair");
        scanf("%d", &choice);

        char num = choice;

        sendto(fd, &num, sizeof(num), 0,(struct sockaddr *)&remoteServAddr, sizeof(struct sockaddr));

        switch(choice)
        {
            case 1:
                printf("Digite o nome do arquivo de audio que deseja:\n");
                scanf("%s",afileName);
                sendto(fd, afileName, strlen(afileName), 0,(struct sockaddr *)&remoteServAddr, sizeof(struct sockaddr));
                FILE *afp;
                afp=fopen(afileName,"r");
                fseek(afp,0,SEEK_END);
                size_t  afsize=ftell(afp);
                fseek(afp,0,SEEK_SET);

                if(afp){
                    printf("Lendo arquivo\n");
                    if(fread(aufile,afsize,1,afp)<=0){
                        printf("Nao foi possivel ler o arquivo ou copia-lo ao buffer\n");
                        exit(1);
                    }
                }
                else{
                    printf("Nao foi possivel ler o arquivo\n");
                    exit(0);
                }

                if(sendto(fd, aufile, afsize, 0,(struct sockaddr *)&remoteServAddr, sizeof(struct sockaddr))<0)
                    printf("Arquivo nao pode ser enviado\n");
                else
                    printf("Arquivo enviado com sucesso");
                fclose(afp);
                break;

            case 2:
                printf("Digite o nome do arquivo de video que deseja:\n");
                scanf("%s",vfileName);
                sendto(fd, vfileName, strlen(vfileName), 0,(struct sockaddr *)&remoteServAddr, sizeof(struct sockaddr));
                FILE *vfp;
                vfp=fopen(vfileName,"r");

                fseek(vfp, 0, SEEK_END);
                size_t vfsize = ftell(vfp);
                fseek(vfp, 0, SEEK_SET);

                if(vfp){
                    if(fread(vfile, 1, vfsize, vfp)<=0)
                        printf("Nao foi possivel ler o arquivo\n");
                }
                else{
                    printf("Nao foi possivel ler o arquivo\n");
                    exit(0);
                }
                if(sendto(fd, vfile, vfsize, 0,(struct sockaddr *)&remoteServAddr, sizeof(struct sockaddr))<0)
                    printf("Arquivo nao pode ser enviado");
                else
                    printf("Arquivo enviado com sucesso");
                fclose(vfp);
                break;

            case 3:
                close(fd);
                break;
        }
    }
}
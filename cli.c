#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#include <string.h>

#define LOCAL_SERVER_PORT 1501
#define REMOTE_SERVER_PORT 1500
#define MAX_MSG 100

void recebe_resposta(){
    WSADATA wsaData;

    int socks, rc, n, cliLen;
    struct sockaddr_in cliAddr, servAddr;
    char msg[MAX_MSG];

    // INICIALIZA A DLL DE SOCKETS PARA O WINDOWS
    WSAStartup(MAKEWORD(2,1),&wsaData);

    // CRIACAO DO SOCKET
    socks = socket(AF_INET, SOCK_DGRAM, 0);
    if(socks < 0) {
        printf("\nValor de socks: %d", socks);
        printf("Socket nao pode ser aberto\n");
        return;
    }

    // VINCULAR A PORTA DO SERVIDOR
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(LOCAL_SERVER_PORT);

    // CRIAcaO DO SOCKET
    socks = socket(AF_INET, SOCK_DGRAM, 0);
    if(socks < 0) {
        printf("Socket nao pode ser aberto\n");
        return;
    }

    // TESTA SE A PORTA ESTA DISPONIVEL
    rc = bind (socks, (struct sockaddr *) &servAddr,sizeof(servAddr));
    if(rc<0) {
        printf("Vinculo com numero de porta impossibilitado %d \n", LOCAL_SERVER_PORT);
        return;
    }

    printf("Esperando pelos dados na porta UDP %u\n",LOCAL_SERVER_PORT);

    // LACO INFINITO DE ESPERA DO SERVIDOR
    while(1) {

        // INICIANDO BUFFER
        memset(msg,0x0,MAX_MSG);

        // RECEBENDO UMA MENSGAEM
        cliLen = sizeof(cliAddr);
        n = recvfrom(socks, msg, MAX_MSG, 0, (struct sockaddr *) &cliAddr, &cliLen);

        if(n<0) {
          printf("Nao pode receber os dados \n");
        }

        // IMPRIMIR MENSAGEM RECEBIDA
        printf("De %s:UDP%u : %s \n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port),msg);
        if(strcmp(msg,"FIM") == 0){
            break;
        }

    } // FIM DO LOOP DO SERVIDOR

    closesocket(socks);
    WSACleanup();
}

int main(int argc, char *argv[]) {
	char nome_arquivo[100];
	printf("Digite o nome do arquivo: ");
    scanf("%s", &nome_arquivo);
	
    WSADATA wsaData;
    LPHOSTENT hostEntry;

    int socks, rc, i;
    struct sockaddr_in cliAddr, remoteServAddr;

    // INICIALIZA A DLL DE SOCKETS PARA O WINDOWS
    WSAStartup(MAKEWORD(2,1),&wsaData);

    // VALIDA ENDERECO DE IP RECEBIDO COMO ARGUMENTO
    hostEntry = gethostbyname("127.0.0.1");
    if (hostEntry == NULL){
       printf("Host desconhecido 127.0.0.1\n");
       return 1;
    }

    // VINCULAR A PORTA DO SERVIDOR REMOTO
    remoteServAddr.sin_family = hostEntry->h_addrtype ;
    remoteServAddr.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);
    remoteServAddr.sin_port = htons(REMOTE_SERVER_PORT);	// NUMERO DA PORTA VINDA PELA LINHA DE COMANDO

    // CRIANDO SOCKET
    socks = socket(AF_INET,SOCK_DGRAM,0);
    if(socks < 0) {
        printf("Socket nao pode ser aberto\n");
        return 1;
    }

    /* VINCULAR A PORTA DO CLIENTE */
    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    cliAddr.sin_port = htons(0);

    rc = bind(socks, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
    if(rc<0) {
        printf("Nao pode vincular a porta\n");
        return 1;
    }

    // ENVIANDO OS DADOS    
    rc = sendto(socks, nome_arquivo, strlen(nome_arquivo)+1, 0,
        (LPSOCKADDR) &remoteServAddr,
        sizeof(struct sockaddr));

    if(rc<0) {
      printf("Nao pode enviar dados %d \n",i-1);
      closesocket(socks);
      return 1;
    }

    closesocket(socks);
    WSACleanup();
    recebe_resposta();

    return 0;
}



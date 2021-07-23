#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#include <string.h>

#define REMOTE_SERVER_PORT 1500
#define MAX_MSG 100

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
       printf("Host desconhecido %s\n", argv[1]);
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

    return 0;
}



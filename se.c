#include <windows.h>
#include <winsock.h>
#include <stdio.h>

#define LOCAL_SERVER_PORT 1500
#define MAX_MSG 100

int main(int argc, char *argv[]) {
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
        return(1);
    }

    // VINCULAR A PORTA DO SERVIDOR
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(LOCAL_SERVER_PORT);

    // CRIAcaO DO SOCKET
    socks = socket(AF_INET, SOCK_DGRAM, 0);
    if(socks < 0) {
        printf("Socket nao pode ser aberto\n");
        return 1;
    }

    // TESTA SE A PORTA ESTA DISPONIVEL
    rc = bind (socks, (struct sockaddr *) &servAddr,sizeof(servAddr));
    if(rc<0) {
        printf("Vinculo com numero de porta impossibilitado %d \n", LOCAL_SERVER_PORT);
        return 1;
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
        printf("De %s:UDP%u : %s \n",
               inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port),msg);

        } // FIM DO LOOP DO SERVIDOR

        closesocket(socks);
        WSACleanup();
        return 0;
}

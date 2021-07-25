#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

#define LOCAL_CLIENT_PORT 1503
#define REMOTE_CLIENT_PORT 1502
#define LOCAL_SERVER_PORT 1501
#define REMOTE_SERVER_PORT 1500
#define MAX_MSG 100

void recebe_resposta(char vet_resposta[MAX_MSG][MAX_MSG], int tipo, char nome_arquivo[MAX_MSG]){
    //tipo 0 cliente recebe resposta do servidor
    //tipo 1 cliente que pediu o arquivo recece resposta do cliente que possui o arquivo
    //tipo 2 cliente que possui o arquivo recebe resposta do cliente que pediu o arquivo
    WSADATA wsaData;

    int i, j;
    FILE *target;
    char resultado[50];

    int socks, rc, n, cliLen, k, porta;
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
    if(tipo == 2)   porta = REMOTE_CLIENT_PORT;
    if(tipo == 1)   porta = LOCAL_CLIENT_PORT;
    if(tipo == 0)   porta = LOCAL_SERVER_PORT;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(porta);

    // CRIACAO DO SOCKET
    socks = socket(AF_INET, SOCK_DGRAM, 0);
    if(socks < 0) {
        printf("Socket nao pode ser aberto\n");
        return;
    }

    // TESTA SE A PORTA ESTA DISPONIVEL
    rc = bind (socks, (struct sockaddr *) &servAddr,sizeof(servAddr));
    if(rc<0) {
        printf("Vinculo com numero de porta impossibilitado %d \n", porta);
        return;
    }

    printf("Esperando pelos dados na porta UDP %d\n",porta);

    // LACO INFINITO DE ESPERA DO SERVIDOR
    k=0;
    if(tipo == 1){//abre o novo arquivo para escrita
        char caminho[30];
        strcpy(caminho,".\\arquivos_recebidos_cliente\\");
        target = fopen(strcat(caminho,nome_arquivo), "w");
        if( target == NULL ){
            printf("Erro ao ler o arquivo\n");
            exit(EXIT_FAILURE);
        }
    }
    while(1) {

        // INICIANDO BUFFER
        memset(msg,0x0,MAX_MSG);

        //LIMPANDO A STRING MSG
        strcpy(msg,"");

        // RECEBENDO UMA MENSGAEM
        cliLen = sizeof(cliAddr);
        n = recvfrom(socks, msg, MAX_MSG, 0, (struct sockaddr *) &cliAddr, &cliLen);

        if(n<0) {
          printf("Nao pode receber os dados \n");
        }

        // IMPRIMIR MENSAGEM RECEBIDA
        if(strcmp(msg,"FIM") == 0){
            break;
        }
        if(strcmp(msg,"") != 0){            
            if(tipo == 1){//Recebe o arquivo em pacotes de 50 caracteres e escreve no novo arquivo
                for(i=0; i<50; i++){
                    fputc(msg[i], target);
                }
                strcpy(vet_resposta[k], "Arquivo Recebido");
            }else{
                strcpy(vet_resposta[k], msg);
                k++;
            }
            
        }

    } // FIM DO LOOP DO SERVIDOR
    if(tipo == 1){//fecha o novo arquivo
        fclose(target);
    }
    closesocket(socks);
    WSACleanup();
}

void envia_resposta(char mensagem[], int tipo){
    //tipo 0 cliente enviar a solicitacao do arquivo para o servidor
    //tipo 1 cliente enviar a solicitação do arquivo para outro cliente
    //tipo 2 cliente que possui o arquivo enviar resposta para o cliente que pediu o arquivo
    int i, j, k, porta, len_vet;
    char ch, resultado[50];
    strcpy(resultado, "");
    FILE *source;

    WSADATA wsaData;
    LPHOSTENT hostEntry;

    int socks, rc;
    struct sockaddr_in cliAddr, remoteServAddr;

    // INICIALIZA A DLL DE SOCKETS PARA O WINDOWS
    WSAStartup(MAKEWORD(2,1),&wsaData);

    // VALIDA ENDERECO DE IP RECEBIDO COMO ARGUMENTO
    hostEntry = gethostbyname("127.0.0.1");
    if (hostEntry == NULL){
       printf("Host desconhecido 127.0.0.1\n");
       return;
    }

    // VINCULAR A PORTA DO SERVIDOR REMOTO
    if(tipo == 2)   porta = LOCAL_CLIENT_PORT;
    if(tipo == 1)   porta = REMOTE_CLIENT_PORT;
    if(tipo == 0)   porta = REMOTE_SERVER_PORT;
    remoteServAddr.sin_family = hostEntry->h_addrtype ;
    remoteServAddr.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);
    remoteServAddr.sin_port = htons(porta);	// NUMERO DA PORTA VINDA PELA LINHA DE COMANDO

    // CRIANDO SOCKET
    socks = socket(AF_INET,SOCK_DGRAM,0);
    if(socks < 0) {
        printf("Socket nao pode ser aberto\n");
        return;
    }

    /* VINCULAR A PORTA DO CLIENTE */
    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    cliAddr.sin_port = htons(0);

    rc = bind(socks, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
    if(rc<0) {
        printf("Nao pode vincular a porta\n");
        return;
    }

    // ENVIANDO OS DADOS
    if(tipo == 2){//quebra o arquivo em pacotes de 50 caracteres e envia via socket
        i = 0, j =0, k=0;
        source = fopen(mensagem, "r");
        if( source == NULL ){
            printf("Erro ao ler o arquivo\n");
            exit(EXIT_FAILURE);
        }
        
        while((resultado[i]  = fgetc(source)) != EOF ){
            i++;
            if(i==50){//envia cada vez que chegar no caracter 50
                rc = sendto(socks, resultado, strlen(resultado)+1, 0,(LPSOCKADDR) &remoteServAddr, sizeof(struct sockaddr));
                if(rc<0) {
                    printf("Nao pode enviar dados %d \n",i-1);
                    closesocket(socks);
                    fclose(source);
                    return;
                }
                strcpy(resultado, "");
                i = 0;
            }
        }
        //enviando ultimo pacote que pode ser menor que 50
        char ultimo_pacote[i+1];
        for(k=0; k<i; k++){
            ultimo_pacote[k] = resultado[k];
        }
        rc = sendto(socks, ultimo_pacote, strlen(ultimo_pacote)+1, 0,(LPSOCKADDR) &remoteServAddr, sizeof(struct sockaddr));
        if(rc<0) {
            printf("Nao pode enviar dados %d \n",i-1);
            closesocket(socks);
            fclose(source);
            return;
        }

        fclose(source);
        rc = sendto(socks, "FIM", 4, 0,
            (LPSOCKADDR) &remoteServAddr,
            sizeof(struct sockaddr));
    }else{ //tipo 0 e 1 enviam o mesmo tipo de mensagem
        rc = sendto(socks, mensagem, strlen(mensagem)+1, 0,
            (LPSOCKADDR) &remoteServAddr,
            sizeof(struct sockaddr));
        rc = sendto(socks, "FIM", 4, 0,
            (LPSOCKADDR) &remoteServAddr,
            sizeof(struct sockaddr));
    }

    closesocket(socks);
    WSACleanup();
}

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "Portuguese");
	int i,k;
    char nome_arquivo[100];
	char vet_resposta[MAX_MSG][MAX_MSG];

    int opcao = 1;

    while(opcao != 3){
        printf("\tMenu\nEscolha uma opcao: \n1. Solicitar arquivo\n2. Enviar arquivo\n3. Sair\n");
        
        printf("Digite a opcao que deseja: ");
        scanf("%d", &opcao);

        switch(opcao){
            case 1:
                for(k = 0; k<MAX_MSG; k++)//inicializando o vetor de respostas com strings vazias
                    strcpy(vet_resposta[k], "");
                
                printf("Digite o nome do arquivo: ");
                scanf("%s", &nome_arquivo);

                envia_resposta(nome_arquivo,0);

                recebe_resposta(vet_resposta, 0,"");

                for(k=0; k<MAX_MSG; k++){      
                    if(strcmp(vet_resposta[k],"") != 0)
                        printf("Resposta Recebida: %s Posicao: %d\n", vet_resposta[k], k);
                    else break;
                }

                envia_resposta(nome_arquivo, 1);

                for(k = 0; k<MAX_MSG; k++)//inicializando o vetor de respostas com strings vazias
                    strcpy(vet_resposta[k], "");

                recebe_resposta(vet_resposta, 1,nome_arquivo);

                for(k=0; k<20000; k++){      
                    if(strcmp(vet_resposta[k],"") != 0)
                        printf("Resposta Recebida: %s Posicao: %d\n", vet_resposta[k], k);
                    else break;
                }
                

            break;

            case 2:
                for(k = 0; k<MAX_MSG; k++)//inicializando o vetor de respostas com strings vazias
                    strcpy(vet_resposta[k], "");

                recebe_resposta(vet_resposta, 2,"");

                k=0;
                while(k<MAX_MSG){
                    if(strcmp(vet_resposta[k],"") != 0)
                        printf("Solicitacao Recebida: %s Posicao: %d\n", vet_resposta[k], k);
                    else break;
                    k++;
                }

                if(k > 0){
                    envia_resposta(vet_resposta[0], 2);
                }
            break;

            case 3:
            break;

            default:
                printf("Opcao invalida!\n");
        }
    }

    return 0;
}



#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#include <string.h>

#define LOCAL_SERVER_PORT 1500
#define REMOTE_SERVER_PORT 1501
#define MAX_MSG 100

typedef struct arquivo{
    char nome[100];
    char ip[16];
}t_arquivo;

void escrever_arquivo(t_arquivo arquivos){
	int i;
	FILE * arq;

	// abre o arquivo para escrita no modo append (adiciona ao final)
	arq = fopen("dados.bin", "ab");

	if(arq != NULL){
		fwrite(&arquivos, sizeof(t_arquivo), 1, arq);// escreve cada elemento do vetor no arquivo
		fclose(arq); // aborta o programa
	}else{
		printf("\nErro ao abrir o arquivo para leitura!\n");
		exit(1); // aborta o programa
	}
}

int ler_arquivo(t_arquivo aux_arquivos[MAX_MSG]){
	// abre o arquivo para leitura
	FILE * arq = fopen("dados.bin", "rb");

	if(arq != NULL){
		int indice = 0;
		while(1){
			t_arquivo p;

			// fread le os dados e retorna a quantidade de elementos lidos com sucesso
			size_t r = fread(&p, sizeof(t_arquivo), 1, arq);

			// se retorno for menor que o count, então sai do loop
			if(r < 1)
				break;
			else
				aux_arquivos[indice++] = p;
		}
		fclose(arq); // fecha o arquivo
		return indice;
	}else{
		printf("Erro ao abrir o arquivo para leitura!\n");
		exit(1); // aborta o programa
	}
}

void envia_resposta(char mensagem[]){
    t_arquivo vet_arquivos[MAX_MSG];
    int j, k;
    char vet_resposta[MAX_MSG][16];

    int len_vet = ler_arquivo(vet_arquivos);

    for(k = 0; k<MAX_MSG; k++)//inicializando o vetor de respostas com strings vazias
        strcpy(vet_resposta[k], "");

	// inserindo no vetor de respostas os ips dos elementos do vetor vet_arquivos que possuem o arquivo com nome desejado
    k=0;
	for(j= 0; j < len_vet; j++){
        if(strcmp(vet_arquivos[j].nome,mensagem) == 0){
            fflush(stdin);
            strcpy(vet_resposta[k], vet_arquivos[j].ip);
            k++;
        }
	}
    strcpy(vet_resposta[k], "FIM");
	
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
       return;
    }

    // VINCULAR A PORTA DO SERVIDOR REMOTO
    remoteServAddr.sin_family = hostEntry->h_addrtype ;
    remoteServAddr.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);
    remoteServAddr.sin_port = htons(REMOTE_SERVER_PORT);	// NUMERO DA PORTA VINDA PELA LINHA DE COMANDO

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
    for(k=0; k<MAX_MSG; k++){
        if(strcmp(vet_resposta[k],"") != 0){
            rc = sendto(socks, vet_resposta[k], strlen(vet_resposta[k])+1, 0,(LPSOCKADDR) &remoteServAddr, sizeof(struct sockaddr));
            if(rc<0) {
                printf("Nao pode enviar dados %d \n",i-1);
                closesocket(socks);
                return;
            }    
        }else   break;
    }

    closesocket(socks);
    WSACleanup();
}

void recebe_resposta(char vet_resposta[MAX_MSG][MAX_MSG]){
    WSADATA wsaData;

    int socks, rc, n, cliLen,k;
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

    // CRIACAO DO SOCKET
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
    k=0;
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
        if(strcmp(msg,"FIM") == 0){
            fflush(stdin);
            break;
        }
        if(strcmp(msg,"") != 0){
            fflush(stdin);
            strcpy(vet_resposta[k], msg);
            k++;
        }

    } // FIM DO LOOP DO SERVIDOR

    closesocket(socks);
    WSACleanup();
}


int main(int argc, char *argv[]) {
    int socks, rc, i,k;
    struct sockaddr_in cliAddr, remoteServAddr;
    char nome_arquivo[100];
	char vet_resposta[MAX_MSG][MAX_MSG];
    t_arquivo novo_arquivo;

    for(k = 0; k<MAX_MSG; k++)//inicializando o vetor de respostas com strings vazias
        strcpy(vet_resposta[k], "");

    recebe_resposta(vet_resposta);//recebendo o nome do arquivo solicitado pelo cliente

    k=0;
    while(k<MAX_MSG){
        if(strcmp(vet_resposta[k],"") != 0)
            printf("Solicitacao Recebida: %s\n", vet_resposta[k]);
        else break;
        k++;
    }

    if(k > 0){
        envia_resposta(vet_resposta[0]);//Procurando e enviando IPs
    }

    for(k = 0; k<MAX_MSG; k++)//inicializando o vetor de respostas com strings vazias
        strcpy(vet_resposta[k], "");

    recebe_resposta(vet_resposta);//recebendo o nome do arquivo que o  cliente baixou com sucesso

    k=0;
    while(k<MAX_MSG){
        if(strcmp(vet_resposta[k],"") != 0){
            if(k == 0){
                printf("Arquivo baixado pelo cliente: %s\n", vet_resposta[k]);
                strcpy(novo_arquivo.nome, vet_resposta[k]);
            }
            if(k == 1){
                printf("IP do cliente: %s\n", vet_resposta[k]);
                strcpy(novo_arquivo.ip,  vet_resposta[k]);
                escrever_arquivo(novo_arquivo);
                break;
            }
        }else break;
        k++;
    }

    return 0;
}

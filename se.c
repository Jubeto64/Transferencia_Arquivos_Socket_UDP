#include <windows.h>
#include <winsock.h>
#include <stdio.h>

#define LOCAL_SERVER_PORT 1500
#define REMOTE_SERVER_PORT 1501
#define MAX_MSG 100

typedef struct arquivo{
    char nome[100];
    char ip[16];
}t_arquivo;

void escrever_arquivo(t_arquivo arquivos[]){
	int i;
	int len_vet = sizeof(arquivos) - 1; // tamanho do vetor
	FILE * arq;

	// abre o arquivo para escrita no modo append (adiciona ao final)
	arq = fopen("dados.bin", "ab");

	if(arq != NULL){
		for(i = 0; i < len_vet; i++)			
			fwrite(&arquivos[i], sizeof(t_arquivo), 1, arq);// escreve cada elemento do vetor no arquivo
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

void envia_resposta(char endereco[], unsigned int porta, char mensagem[]){
    t_arquivo vet_arquivos[MAX_MSG];
    int len_vet = ler_arquivo(vet_arquivos);
    int j, k;
    char vet_resposta[MAX_MSG][16];
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

    printf("De %s:UDP%u : %s \n", endereco, porta, mensagem);
	
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
        envia_resposta(inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port),msg);

    } // FIM DO LOOP DO SERVIDOR

    closesocket(socks);
    WSACleanup();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#define MAX 100

typedef struct arquivo{
	char nome[100];
    char ip[16];
} t_arquivo;

int ler_arquivo(t_arquivo aux_arquivos[MAX]){
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


int main(int argc, char *argv[]){
	t_arquivo aux_arquivos[MAX];
	t_arquivo novo_arquivo;

	int len_vet = ler_arquivo(aux_arquivos);
	int i, opcao;

	while(opcao != 3){
        printf("\tMenu\nEscolha uma opcao: \n1. Ler lista de arquivos\n2. Inserir novo arquivo e IP\n3. Sair\n");
        
        printf("Digite a opcao que deseja: ");
        scanf("%d", &opcao);

        switch(opcao){
            case 1:
				// mostrando os elementos do vetor aux_arquivos
				for(i = 0; i < len_vet; i++){
					printf("Nome: %s\n", aux_arquivos[i].nome);
					printf("IP: %s\n\n", aux_arquivos[i].ip);
				}
			break;
			case 2:
				printf("Digite o nome do arquivo: ");
                scanf("%s", novo_arquivo.nome);
				printf("Digite o IP do arquivo: ");
                scanf("%s", novo_arquivo.ip);
				escrever_arquivo(novo_arquivo);
			break;
			case 3:
            break;

            default:
                printf("Opcao invalida!\n");
		}
	}
	return 0;
}

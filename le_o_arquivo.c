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

int main(int argc, char *argv[]){
	t_arquivo aux_arquivos[MAX];

	int len_vet = ler_arquivo(aux_arquivos);
	int i;

	// mostrando os elementos do vetor aux_arquivos
	for(i = 0; i < len_vet; i++){
		printf("Nome: %s\n", aux_arquivos[i].nome);
		printf("IP: %s\n\n", aux_arquivos[i].ip);
	}

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#define MAX 100

typedef struct arquivo{
	char nome[100];
    char ip[16];
} t_arquivo;


// função para escrever os elementos de uma struct no arquivo
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
	}
	else{
		printf("\nErro ao abrir o arquivo para leitura!\n");
		exit(1); // aborta o programa
	}
}

// função para ler do arquivo
// recebe o vetor que ela irá preencher
// retorna a quantidade de elementos preenchidos
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
	// vetor que será escrito no arquivo
	t_arquivo arquivos[] = {{"som.wav", "127.0.0.1"},{"som.wav", "127.0.0.2"},{"texto.txt", "127.0.0.3"},{"texto.txt", "127.0.0.4"}};

	// vetor para onde serão carregados os dados
	// esse vetor foi criado para demonstrar que realmente funciona,
	// mas basta utilizar somente um vetor
	t_arquivo aux_arquivos[MAX];

	escrever_arquivo(arquivos);

	int len_vet = ler_arquivo(aux_arquivos);
	int i;

	// mostrando os elementos do vetor aux_arquivos
	for(i = 0; i < len_vet; i++){
		printf("Nome: %s\n", aux_arquivos[i].nome);
		printf("IP: %s\n\n", aux_arquivos[i].ip);
	}

	return 0;
}
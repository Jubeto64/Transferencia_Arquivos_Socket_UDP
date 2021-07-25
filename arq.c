#include <stdio.h>
#include <stdlib.h>

int le_arquivo(char nome_arquivo[20],  char resultado[100000]){
    int i = 0;
    char ch;
    FILE *source;
    source = fopen(nome_arquivo, "r");
    if( source == NULL ){
        printf("Erro ao ler o arquivo\n");
        exit(EXIT_FAILURE);
    }
    
    while( ( ch = fgetc(source) ) != EOF ){
        resultado[i] = ch;
        i++;
    }
    fclose(source);
    return i;
}

void escreve_arquivo(char novo_arquivo[20], int n,  char resultado[100000]){
    int i = 0;
    FILE *target;
    target = fopen(novo_arquivo, "w");
    if( target == NULL ){
        printf("Erro ao ler o arquivo\n");
        exit(EXIT_FAILURE);
    }
    while(i < n){
        fputc(resultado[i], target);
        i++;
    }
    
}


int main(){
    char ch, nome_arquivo[20], novo_arquivo[20], resultado[100000];
    int i;
    printf("Digite o nome do arquivo a ser copiado\n");
    scanf("%s", &nome_arquivo);

    i = le_arquivo(nome_arquivo, resultado);

    printf("Digite o nome do novo arquivo\n");
    scanf("%s", &novo_arquivo);
    escreve_arquivo(novo_arquivo, i, resultado);
    
    printf("Arquivo copiado com sucesso.\n");
    
    return 0;
}
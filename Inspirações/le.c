#include <stdio.h>
#include <stdlib.h>

int main( int argc, char** argv ){

    if( argc > 1 ){
        // Declaramos um ponteiro(link para o endereço da memória) para o arquivo de nome: 'pf'
        FILE *pf;
        char conteudo[100];

        //Abre o arquivo novamente para leitura
        pf = fopen(argv[1], "rb");

        // Le em conteudo o valor da variável armazenada anteriormente pf
        fread(&conteudo, sizeof(char), 100,pf);

        // Imprime o conteúdo, se existir, do arquivo informado
        printf("\nO CONTEUDO DO ARQUIVO E:\n %s \n", conteudo);

        fclose(pf);
    }else{
        printf("Informe o arquivo. Ex.: ./program arquivo.txt\n");
    }

    return(0);
}
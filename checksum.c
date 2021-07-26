#include <stdio.h>
#include <string.h>
//#include <conio.h>
char ParaBinario(long long n)
{
    char r;
    while (n != 0){
        r += ( n % 2 == 0 ? "0" : "1" );
        n /= 2;
    }
    return r;
}

int CheckSum(char *s, int size, int portaEntrada, int PortaDestino)
{
    long long chkSum = 0;
    char StringBinaria;
    int IntBinario, i;
    for (i = 0; i < size; i++)
        chkSum += (s[i] * i);
    chkSum = chkSum + portaEntrada + PortaDestino;
    /*StringBinaria = ParaBinario(chkSum);
    IntBinario = atoi(StringBinaria);*/
    return chkSum;
}

int main(int argc, char** argv)
{
	char mensagem[50] = "insirindo uma mensagem aleatoria";	
    printf("CheckSum de %s: ", &mensagem);
	printf("%lld\n", CheckSum(mensagem, sizeof(mensagem), 1500, 1501));

    return 0;
}

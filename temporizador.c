#include <stdio.h>
#include <time.h>

void tempo (float tempo) 
{
   float inst1=0, inst2=0;

   inst1 = (float)clock()/(float)CLOCKS_PER_SEC;

   while (inst2-inst1<tempo)
   {
	inst2 = (float)clock()/(float)CLOCKS_PER_SEC;
   }
   

   return;
}

int main (void) {

                recebe_resposta = false;
                while(recebe_resposta == false)
				{
	                envia_resposta(nome_arquivo,0);
	                recebe_resposta(vet_resposta, 0);
	                tempo(10);
				}

   return 0;

}
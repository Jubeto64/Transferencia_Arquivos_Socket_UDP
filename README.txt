Grupo:
João Victor do Nascimento - 2019003332
José Victor de Carvalho Silva - 2019005284
Jubeto José de Oliveira - 2019004572
Matheus Gabriel Cruz Mendez - 2018010870


Este Programa deve ser executado no sistema operacional Windows

Como executar:
-Executar primeiro o servidor se.exe
-Depois o cliente cli.exe
-E escolher a opção 2. Enviar arquivo
-Manter o servidor e do cliente em execução 
-E executar mais uma vez o programa do cliente para criar um novo cliente que solicita o arquivo
-Escolher a opção 1. Solicitar arquivo
-Informar o nome do arquivo com nome.extensão
-Na lista de arquivos/Ips atual estão disponíveis os arquivos texto.txt e som.wav

É importante que a pasta arquivos_recebidos_cliente  esteja com o mesmo nome(não renomear) e no mesmo diretório que os programas 
além do arquivo binário dados.bin, que possui a lista de arquivos/IPs.

O programa le_inser_lista_servidor permite consultar a lista do servidor e inserir um novo arquivivo/IP nela.

Compilação no CMD:
Abrir uma janela para compilar o código e digitar os comandos:
cd "diretorio arquivo"
gcc se.c -o se.exe -l ws2_32

cd "diretorio arquivo"
gcc cli.c -o cli.exe -l ws2_32


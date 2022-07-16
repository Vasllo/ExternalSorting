#include <stdlib.h>
#include <string.h>
#include "cliente.h"

Cliente *cliente(int codCliente, char *nome, char *data)
{
    Cliente *clientes = (Cliente *)malloc(sizeof(Cliente));
    if (clientes)
        memset(clientes, 0, sizeof(Cliente));
    clientes->codCliente = codCliente;
    strcpy(clientes->nome, nome);
    strcpy(clientes->data_nascimento, data);
   
    return clientes;
}

int tamanho_registro()
{
    return sizeof(Cliente);
}

void imprime_cliente(Cliente *clientes)
{
    printf("    %d, %s, %s \n", clientes->codCliente, clientes->nome,  clientes->data_nascimento);
}

void salva_cliente(Cliente *cli, FILE *out)
{
    fwrite(&cli->codCliente, sizeof(int), 1, out);
    fwrite(cli->nome, sizeof(char), sizeof(func->nome), out);
    fwrite(cli->data_nascimento, sizeof(char), sizeof(cli->data_nascimento), out);
    fflush(out);
}

Cliente *le_cliente(FILE *in)
{
    Cliente *cli = (Cliente *)malloc(sizeof(Cliente));
    int result = fread(&cli->codCliente, sizeof(int), 1, in);
   
    if (0 >= result)
    {
        free(cli);
        return NULL;
    }
    fread(cli->nome, sizeof(char), sizeof(cli->nome), in);
    fread(cli->data_nascimento, sizeof(char), sizeof(cli->data_nascimento), in);
    
    return cli;
}

void cria_base_dados_cliente(FILE *arq, FILE *arqIndicePrimario, FILE *arqIndicePrimarioNaoOrdenado, int nCli)
{

    int codCliente;
    char nome_cliente[TAM_NOME];
    char data_nascimento[11];
  
    int chaves[nCli + 1];

    for (int i = 0; i < nCli; i++)
    {
       
        scanf("%d", &codCliente);
        scanf("%s", nome_cliente);
        scanf("%s", data_nascimento);
        Cliente *cli = cliente(codCliente, nome_cliente, data_nascimento);
        fseek(arq, (i)*tamanho_registro(), SEEK_SET);
        salva_cliente(cli, arq);
        chaves[cli->codCliente - 1] = i;
        fwrite(&cli->codCliente, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve a chave
        fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);         //escreve o rrn
    }
    for (int f = 1; f <= nCli; f++)
    {
        fwrite(&f, sizeof(int), 1, arqIndicePrimario);
        fwrite(&chaves[f], sizeof(int), 1, arqIndicePrimario);
    }
}

void cria_base_dados_cliente_teste(FILE *arq, FILE *arqIndicePrimario, FILE *arqIndicePrimarioNaoOrdenado, int nCli)
{
    int i = 0;
    int chaves[nCli + 1];

    //cria arquivo para armazenar os indices primarios uma chave para cada registro de CLIENTE
    Cliente *cli1 = cliente(2, "yoda",  "27/05/1200");
    fseek(arq, (i)*tamanho_registro(), SEEK_SET);
    salva_cliente(cli1, arq);
    chaves[cli1->codCliente] = i;
    fwrite(&cli1->codCliente, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);          //escreve o rrn
    free(cli1);
    i++;
    Cliente *cli2 = cliente(1, "quigon",  "27/05/1900");
    fseek(arq, (i)*tamanho_registro(), SEEK_SET);
    salva_cliente(cli2, arq);
    chaves[cli2->codCliente] = i;
    fwrite(&cli2->codCliente, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);          //escreve o rrn
    free(cli2);
    i++;
    Cliente *cli3 = cliente(5, "anakin",  "27/05/2000");
    fseek(arq, (i)*tamanho_registro(), SEEK_SET);
    salva_cliente(cli3, arq);
    chaves[cli3->codCliente] = i;
    fwrite(&cli3->codCliente, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);          //escreve o rrn
    free(cli3);
    i++;
    Cliente *cli4 = cliente(3, "obiwan",  "27/05/1200");
    fseek(arq, (i)*tamanho_registro(), SEEK_SET);
    salva_cliente(cli4, arq);
    chaves[cli4->codCliente] = i;
    fwrite(&cli4->codCliente, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);          //escreve o rrn
    free(cli4);
    i++;
    Cliente *cli5 = cliente(4, "dooku",  "27/05/1200");
    fseek(arq, (i)*tamanho_registro(), SEEK_SET);
    salva_cliente(cli5, arq);
    chaves[cli5->codCliente] = i;
    fwrite(&cli5->codCliente, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);          //escreve o rrn
    free(cli5);
    i++;

    //escrevendo o arquivo de indice primario (chave e rrn) ordenado pela chave
    for (int f = 1; f <= nCli; f++)
    {
        fwrite(&f, sizeof(int), 1, arqIndicePrimario);         //escreve a chave
        fwrite(&chaves[f], sizeof(int), 1, arqIndicePrimario); //escreve o rrn
    }
}

void imprime_base_dados_cliente(FILE *arq, int nCli)
{
    rewind(arq);
    int i = 0;
    fseek(arq, 0 * sizeof(Cliente), SEEK_SET);
    if (arq != NULL)
    {

        int aux = 1;
        do
        {
            fseek(arq, (i)*tamanho_registro(), SEEK_SET);
            Cliente *cli = le_cliente(arq);
            if (cli)
            {
                imprime_cliente(cli);
                i++;
                free(cli);
            }
            else
                aux = 0;
        } while (aux);
    }
    else
        printf("Erro ao abrir arquivo\n");
}

void imprime_indice_primario_cliente(FILE *arqIndicePrimario, int nCli)
{
    rewind(arqIndicePrimario);
    for (int i = 1; i <= nCli; i++)
    {
        int chave, byteoffset;
        fread(&chave, sizeof(int), 1, arqIndicePrimario);
        fread(&byteoffset, sizeof(int), 1, arqIndicePrimario);
        printf("%d: %d\n", chave, byteoffset);
    }
}

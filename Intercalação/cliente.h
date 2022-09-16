#include <stdio.h>
#include <stdlib.h>

#define TAMANHO_DATA 11
#define TAMANHO_NOME 49

struct Cliente
{
    int codCliente;
    char nome [TAMANHO_NOME];
    char dataDeNascimento[TAMANHO_DATA];
} typedef Cliente;

Cliente* criarCliente ();
void liberarCliente(Cliente **cliente);
void imprimirCliente(Cliente *cliente);
Cliente* lerClienteDeArquivo(FILE *arquivo, int posicao);
int fwriteCliente(FILE* arquivo, Cliente* cliente);
int freadCliente(FILE* arquivo, Cliente* destino);

Cliente* criarCliente () {
    Cliente* auxiliar = (Cliente*) malloc(sizeof(Cliente));
    return auxiliar;
}

void liberarCliente(Cliente **cliente) {
    free(*cliente);
    *cliente = NULL;
}

void imprimirCliente(Cliente *cliente) {
    printf("Dados do cliente:\nCódigo: %d\nNome: %s\nData de nascimento: %s\n" , \
    cliente->codCliente, cliente->nome, cliente->dataDeNascimento);
}

//  Lê um cliente de uma posição específica em um arquivo. Posição como de um vetor.
Cliente* lerClienteDeArquivo(FILE *arquivo, int posicao) {
    Cliente* cliente = (Cliente*) malloc(sizeof(Cliente));
    posicao = sizeof(Cliente) * posicao;

    //  Busca posição desejada e verifica se houve sucesso.
    if(fseek(arquivo, posicao, SEEK_SET)){
        perror("Posição de cliente inexistente");
        exit(EXIT_FAILURE);
    }
    //  Verifica se leu corretamente pelo retorno de fread, \
        reseta o ponteiro do arquivo e retorna o cliente lido.
    if(fread(cliente, sizeof(Cliente), 1, arquivo)){
        rewind(arquivo);
        return cliente;
    } else {
        free(cliente);
        perror("Falha ao ler cliente");
        exit(EXIT_FAILURE);
    }
}

//  Grava os dados de um cliente para um arquivo. flag=0 -> sucesso na escrita, flag=1 -> erro na escrita.
int fwriteCliente(FILE* arquivo, Cliente* cliente) {
    int nEscrito, flag;
    nEscrito = fwrite(&(cliente->codCliente), sizeof(cliente->codCliente), 1, arquivo);
    nEscrito = fwrite(cliente->nome, sizeof(cliente->nome), 1, arquivo);
    nEscrito = fwrite(cliente->dataDeNascimento, sizeof(cliente->dataDeNascimento), 1, arquivo);
    flag = nEscrito > 0? 0 : 1;
    return flag;
}

//  Lê os dados de um arquivo para um cliente. flag=0 -> sucesso na leitura, flag=1 -> erro de leitura, flag=2 -> eof.
int freadCliente(FILE* arquivo, Cliente* destino) {
    int nLido, flag;
    nLido = fread(&(destino->codCliente), sizeof(destino->codCliente), 1, arquivo);
    nLido = fread(destino->nome, sizeof(destino->nome), 1, arquivo);
    nLido = fread(destino->dataDeNascimento, sizeof(destino->dataDeNascimento), 1, arquivo);
    flag = nLido > 0 ? 0 : feof(arquivo) == 0 ? 1 : 2;
    /*if(flag > 0) {
        flag = 0;
    } else if(feof(arquivo)) {
            flag = 2;
        } else {
            flag = 1;
        }
    }*/
    return flag;
}
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

Cliente* criarCliente () {
    Cliente* auxiliar = (Cliente*) malloc(sizeof(Cliente));
    return auxiliar;
}

void liberarCliente(Cliente **cliente){
    free(*cliente);
    *cliente = NULL;
}

void imprimirCliente(Cliente *cliente){
    printf("Dados do cliente:\nCódigo: %d\n Nome: %s\n Data de nascimento: %s\n" , \
    cliente->codCliente, cliente->nome, cliente->dataDeNascimento);
}

//  Lê um cliente de uma posição específica em um arquivo
Cliente* lerClienteDeArquivo(FILE *arquivo, int posicao){
    Cliente* cliente = (Cliente*) malloc(sizeof(Cliente));

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

//  Grava os dados de um determinado cliente para um determinado arquivo
void fprintCliente(FILE* arquivo, Cliente* cliente) {
    fwrite(&(cliente->codCliente), sizeof(cliente->codCliente), 1, arquivo);
    fwrite(cliente->nome, sizeof(cliente->nome), 1, arquivo);
    fwrite(cliente->dataDeNascimento, sizeof(cliente->dataDeNascimento), 1, arquivo);
}

//  Lê os dados de um determinado arquivo para um determinado cliente
void freadCliente(FILE* arquivo, Cliente* destino){
    fread(&(destino->codCliente), sizeof(destino->codCliente), 1, arquivo);
    fread(destino->nome, sizeof(destino->nome), 1, arquivo);
    fread(destino->dataDeNascimento, sizeof(destino->dataDeNascimento), 1, arquivo);
}
#include <stdio.h>
#include <stdlib.h>

#define TAMANHO_DATA 11
#define TAMANHO_NOME 37

enum estadoOperacao {
    sucesso,
    falha
};

typedef struct Cliente
{
    int codCliente;
    char nome [TAMANHO_NOME];
    char dataDeNascimento[TAMANHO_DATA];
}Cliente;

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

int escreverCliente(Cliente *cliente, FILE *arquivo){
	if (cliente && fwrite(cliente, sizeof(Cliente), 1, arquivo))
		sucesso;
    return falha;
}

Cliente *lerCliente(FILE *arquivo, int posicao){
    Cliente* cliente = (Cliente*) malloc(sizeof(Cliente));

    //  Busca posição desejada e verifica se houve sucesso.
    if(fseek(arquivo, posicao, SEEK_SET))
        exit(EXIT_FAILURE);

    //  Busca um registro de cliente e verifica se leu corretamente, \
        então reseta o ponteiro do arquivo e verifica se conseguiu e retorna o cliente lido.
    if(fread(cliente, sizeof(Cliente), 1, arquivo)){
        if(fseek(arquivo, 0, SEEK_SET))
            exit(EXIT_FAILURE);
        free(cliente);
        return cliente;

    } else {
        free(cliente);
        return NULL;
    }
}
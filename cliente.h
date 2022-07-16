#ifndef CLIENTE_H
#define CLIENTE_H

#include <stdio.h>

#define TAM_NOME 200


typedef struct Cliente {
    int codCliente;
    char nome[TAM_NOME];
    char data_nascimento[11];

}Cliente;


Cliente *cliente(int codCliente, char *nome, char *data_nascimento);


int tamanho_registro();


void salva_cliente(Cliente *cli, FILE *out);


Cliente *le_cliente(FILE *in);


void imprime_cliente(Cliente *client);


void cria_base_dados_cliente(FILE* arq, FILE* arqIndicePrimario, FILE* arqIndicePrimarioNaoOrdenado, int nCli);


void cria_base_dados_cliente_teste(FILE* arq, FILE* arqIndicePrimario,  FILE* arqIndicePrimarioNaoOrdenado, int nCli);

/
void imprime_base_dados_cliente(FILE *arq, int nCli);

/
void imprime_indice_primario_cliente(FILE *arqIndicePrimario, int nCli);

#endif

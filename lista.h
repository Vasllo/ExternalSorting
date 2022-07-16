#ifndef LISTA_H
#define LISTA_H

typedef struct Lista {
	char *nome;
	int tamanho;
	struct Lista *prox;
} Lista;

void imprime(Lista *nomes);


char *cria_str(char *str);


Lista *cria(char *nome, Lista *prox);


void libera(Lista *nomes);


int conta(Lista *nomes);


void inverte(Lista *nomes);

#endif
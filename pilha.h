#ifndef PILHA_H
#define PILHA_H

#include "cliente.h"

typedef struct pilha {
    Cliente *info;
    int p;
} TPilha;


void inicializa(int *base, int *lim, int *topo);

int pilha_cheia(int lim, int topo);
int pilha_vazia(int base, int topo);


Cliente *push(TPilha *pilha, int lim, int *topo, Cliente *info, int p);
Cliente *pop(TPilha *pilha, int base, int *topo);


Cliente *peek_cliente(TPilha *pilha, int base, int *topo);
int peek_fp(TPilha *pilha, int base, int *topo);


void imprime_pilha(TPilha *pilha, int *topo);

/
void cria_pilha_particao(TPilha *pilha, FILE *arq, int nCli, int *vetTopo);

#endif
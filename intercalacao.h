#ifndef INTERCALACAO_H
#define INTERCALACAO_H

#include "lista.h"
#include "pilha.h"

typedef struct vetor{
    Cliente *cli;
    int aux_p;
    FILE *f;
}TVet;


void intercalacao_basico(char *nome_arquivo_saida, int num_p, Lista *nome_particoes);

void intercalacao_arvore_de_vencedores(TPilha **pilha, int *vetTop, char *nome_arquivo_saida, int num_p, int nCli);

#endif
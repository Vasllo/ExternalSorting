//  Algoritmo de particionamento de arquivos pelo algoritmo de seleção natural

#ifndef PARTITION_H
#define PARTITION_H

#include <stdio.h>
#include <stdbool.h>

#include "lista.h"


void classificacao_interna(FILE *arq, Lista *nome_arquivos_saida, int M, int nCli);


void selecao_natural(FILE *arq, Lista *nome_arquivos_saida, int M, int nCli, int n, int* nParticoes);


Cliente* getCliente(FILE* arq, int* contLidos);


int menorParaInicio(Cliente* v[], int M, int menor);

int elementosNoVetor(bool controle[], int tam);


void atualizaNomesParticao(Lista* nomes, int* nParticoes);


int getMenor(Cliente* v[], int tam);


int possuiElementos(Cliente* v[], int tam);

#endif

#ifndef ARVBIN_H
#define ARVBIN_H

#include <stdio.h>
#include <stdlib.h>
#include "cliente.h"
#define SEM_FILHO -1

typedef struct noA {
    int info;
    int rrn;
    int pilha;
    struct noA *pai;
    struct noA *esq;
    struct noA *dir;
} TNoA;


typedef struct No{

    Cliente item;   
    long pEsq;    
    long pDir;    
}TNoAX;


TNoA *inicializa_arvore_binaria();

void print2DUtil(TNoA *root, int space);


int  vazia_arvore_binaria(TNoA* t);


void imprime_arvore_binaria(TNoA *nodo);


void inserir_arvore_binaria(TNoA **t, int chave, int rrn);


void cria_arvore_binaria_indice_primario(FILE *arqIndicePrimario, int nFunc);


int busca_arvore_binaria(TNoA* t, int chave);


TNoA *criaNo_arvore_binaria(int chave, int rrn);


void inserir_arvore_binaria_RAIZ(TNoA *t, int chave, int rrn);

void imprime_chaves_arvore_binaria_externa(FILE* arq_saida);

/
void inserir_recursivo_arvore_binaria_externa(long int localraiz, Cliente p, FILE* arq);
void inserir_item_arvore_binaria_externa(Cliente p, FILE* arq);


void criar_arvore_binaria_externa(FILE *arq_entrada, FILE*arq_saida);


void buscar_recursivo_arvore_binaria_externa(long localraiz, int chave, Cliente** resposta, FILE* arq);
Cliente* pesquisar_arvore_binaria_externa(int chave, FILE* arq);

#endif 

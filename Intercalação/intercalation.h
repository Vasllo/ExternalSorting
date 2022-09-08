#include "cliente.h"
#include <math.h>

typedef struct arvoreVencedor {
    int vencedor;
    FILE* pVencedor;
    arvoreVencedor* esquerda;
    arvoreVencedor* direita;
} arvoreVencedor;

arvoreVencedor* criarNoh () {
    arvoreVencedor* noh = (arvoreVencedor*) malloc(sizeof(arvoreVencedor));
    noh->direita = noh->esquerda = NULL;
    noh->pVencedor = NULL;
    noh->vencedor = -1;
    return noh;
}

void liberarNoh (arvoreVencedor* noh) {
    free(noh);
    noh = NULL;
}

//FILE** carregarClientes ()

arvoreVencedor* adicionarFolhas (arvoreVencedor* raiz, FILE** clientes, int nClientes) {
    arvoreVencedor** folhas = (arvoreVencedor**) malloc(sizeof(arvoreVencedor*)*nClientes);
    for(int i=0; i<nClientes; i++) {
        folhas[i] = (arvoreVencedor*) malloc(sizeof(arvoreVencedor));
    }
    Cliente* aux = NULL;

    for(int i=0; i<nClientes; i++) {
        aux = lerClienteDeArquivo(clientes[i], 0);
        folhas[i]->vencedor = aux->codCliente;
        folhas[i]->pVencedor = clientes[i];
        folhas[i]->direita = folhas[i]->esquerda = NULL;
    }

    raiz = _adicionarNos (raiz, folhas, nClientes);

    for(int i=0; i<nClientes; i++) {
        liberarNoh(folhas[i]);
    }
    free(folhas);
    folhas = NULL;
    // Passar responsabilidade de alocação e desalocação para função auxiliar?
    
    return raiz;
}

arvoreVencedor* _adicionarNos (arvoreVencedor* raiz, arvoreVencedor** nos, int nFolhas) {
    if(nFolhas <= 2) {
        switch (nFolhas) {
            case 2:
                int j = -1;
                raiz->esquerda = nos[0];
                raiz->direita = nos[1];

                nos[0]->vencedor <= nos[1]->vencedor? j=0 : j=1;
                raiz->vencedor = nos[j]->vencedor;
                raiz->pVencedor = nos[j]->pVencedor;
                return raiz;
            case 1:
                raiz->esquerda = nos[0];
                raiz->vencedor = nos[0]->vencedor;
                raiz->pVencedor = nos[0]->pVencedor;
                return raiz;
            default:
                return raiz;
    }
    arvoreVencedor** nos = (arvoreVencedor**) malloc(sizeof(arvoreVencedor*)*ceil(nFolhas/2));
    for(int i=0; i<nFolhas; i++) {
        nos[i] = (arvoreVencedor*) malloc(sizeof(arvoreVencedor));
    }
    // Passar responsabilidade de alocação e desalocação para função auxiliar?

    

    return raiz;
}

void atualizarFolha () {

}

int main (){
    FILE** arquivos = NULL;
    for(int i=0; i<10; i++) {
        arquivos = (FILE**) malloc(sizeof(FILE*)*10);
    }
    arquivos[0] = NULL;

}
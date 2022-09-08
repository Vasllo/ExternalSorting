// Legenda: Nível = profundidade
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

// < - !! - > Protótipo não-terminado
//FILE** carregarClientes () {}

// Adiciona todos arquivos à arvore
arvoreVencedor* adicionarFolhas (arvoreVencedor* raiz, FILE** clientes, int nClientes) {
    // Array dinâmico para manter referência de todas folhas
    // < - !! - > Passar responsabilidade de alocação e desalocação para função auxiliar?
    arvoreVencedor** folhas = (arvoreVencedor**) malloc(sizeof(arvoreVencedor*)*nClientes);
    for(int i=0; i<nClientes; i++) {
        folhas[i] = (arvoreVencedor*) malloc(sizeof(arvoreVencedor));
    }
    Cliente* aux = NULL;

    // Construção das folhas com o ponteiro de cada arquivo e o código de cliente do primeiro cliente do arquivo
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
    
    return raiz;
}

// Função interna recursiva para construir os nós internos e retornar a árvore completa
arvoreVencedor* _adicionarNos (arvoreVencedor* raiz, arvoreVencedor** nos, int nFolhas) {
    // Condição de parada, caso haja 2 ou menos sub-arvores
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
    // Caso contrário, são alocados ceil(nFolhas/2) nós para construir o nível superior da árvore
    // < - !! - > Passar responsabilidade de alocação e desalocação para função auxiliar?
    int nNos = ceil(nFolhas/2);
    arvoreVencedor** novosNos = (arvoreVencedor**) malloc(sizeof(arvoreVencedor*)*nNos);
    for(int i=0; i<nNos; i++) {
        novosNos[i] = (arvoreVencedor*) malloc(sizeof(arvoreVencedor));
    }
    // Caso haja um número ímpar de sub-árvores, o último nó copia o último nó do nível inferior, subindo-o de nível em vez de torná-lo filho único (proibido)
    if(nFolhas%2 == 1) {
        novosNos[nNos-1] = nos[nFolhas-1];
        nNos--;
    }
    // Cada nó do nível atual tem 2 sub-árvores do nível inferior alocadas nos ponteiros esq/dir e o menor código das duas é colocado como vencedor
    for(int i=0; i<nNos; i++) {
        int flag=0, j=0;
        novosNos[i]->esquerda = nos[j];
        novosNos[i]->direita = nos[j+1];

        nos[j]->vencedor <= nos[j+1]->vencedor? flag=j : flag=j+1;
        novosNos[i]->vencedor = nos[flag]->vencedor;
        novosNos[i]->pVencedor = nos[flag]->pVencedor;
        j+=2;
    }

    return raiz = _adicionarNos (raiz, nos, nFolhas); // ?
}

// < - !! - > Protótipo não-terminado
void atualizarFolha () {

}

int main (){
    FILE** arquivos = NULL;
    for(int i=0; i<10; i++) {
        arquivos = (FILE**) malloc(sizeof(FILE*)*10);
    }
    arquivos[0] = NULL;

}
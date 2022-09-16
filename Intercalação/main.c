#include "intercalation.h"

int main(int* argc, char* argv[]) {
    FILE** arquivos = (FILE**) malloc(sizeof(FILE*));
    arvoreVencedor* raiz = criarNoh();
    Cliente* aux = criarCliente();
    FILE* saida = (FILE*) malloc(sizeof(FILE));

    arquivos = carregarArquivos();
    montarArvore(raiz, arquivos);
    intercalar(raiz);

    saida = fopen("particao_intercalada.dat", "rb");

    for(int i=0; i<20; i++) {
        aux = lerClienteDeArquivo(saida, i);
        imprimirCliente(aux);
    }

    return 0;
}
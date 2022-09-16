// Legenda: Nível = profundidade
#include "cliente.h"

#define HIGH_VALUE 2147483647
#define NAME_SIZE 50
#define SUFIX_SIZE 10
#define ARQUIVO_DE_SAIDA "particao_intercalada.dat"

int nArquivos = -1;

typedef struct arvoreVencedor {
    int vencedor;
    FILE* pVencedor;
    struct arvoreVencedor* esquerda;
    struct arvoreVencedor* direita;
} arvoreVencedor;

FILE** carregarArquivos ();
arvoreVencedor* criarNoh ();
void liberarNoh (arvoreVencedor* noh);
void montarArvore (arvoreVencedor* raiz, FILE** arquivos);
void _adicionarNos (arvoreVencedor* raiz, arvoreVencedor** nos, int nSubArvores);
Cliente* atualizarFolha (arvoreVencedor* raiz);
void _atualizarNos (arvoreVencedor* noh, int novoVencedor);
void intercalar (arvoreVencedor* raiz);

void print2D(arvoreVencedor *root);

#define COUNT 10
// Function to print binary tree in 2D
// It does reverse inorder traversal
void print2DUtil(arvoreVencedor *root, int space)
{
    // Base case
    if (root == NULL)
        return;
 
    // Increase distance between levels
    space += COUNT;
 
    // Process right child first
    print2DUtil(root->direita, space);
 
    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d\n", root->vencedor);
 
    // Process left child
    print2DUtil(root->esquerda, space);
}
 
// Wrapper over print2DUtil()
void print2D(arvoreVencedor *root)
{
   // Pass initial space count as 0
   print2DUtil(root, 0);
}

// Cria um array com os ponteiros de todos arquivos
FILE** carregarArquivos () {
    FILE** arquivos = (FILE**) malloc(sizeof(FILE*));
    int index = -1;
    char prefixo[NAME_SIZE] = {'\0'};
    char sufixo[SUFIX_SIZE] = {'\0'};
    char nome[NAME_SIZE+SUFIX_SIZE+sizeof(int)] = {'\0'};

    printf("Olá! Por favor, insira o prefixo (nome até o índice da partição), índice da primeira partição e sufixo das partições (formato do arquivo), " \
        "todos separados por um espaço.\n" \
        "< - ? - > Exemplo: particao_clientes_0.dat -> particao_clientes_ 0 .dat\n" \
        "< - ! - > Todas partições devem ter índices sequenciais, o prefixo deve ter até 50 caracteres e o sufixo até 10 caracteres.\n");

    scanf("%s %d %s", prefixo, &index, sufixo);
    printf("\nQuantas partições há, ao todo? ");
    scanf("%d", &nArquivos);
    // Cria string com o nome dos arquivos, concatenando as variáveis
    sprintf(nome, "%s%d%s", prefixo, index, sufixo);
    // Abre todos arquivos, verifica se houve sucesso e gera o nome da próxima partição a ser aberta
    for(int i=0; i<nArquivos; i++) {
        arquivos[i] = (FILE*) malloc(sizeof(FILE));
        if(!(arquivos[i] = fopen(nome, "r+b"))) {
            perror("Não foi possível abrir os arquivos em criarArrayArquivos");
            exit(EXIT_FAILURE);
        }
        index++;
        sprintf(nome, "%s%d%s", prefixo, index, sufixo);
    }
    return arquivos;
}

arvoreVencedor* criarNoh () {
    arvoreVencedor* noh = (arvoreVencedor*) malloc(sizeof(arvoreVencedor));
    noh->direita = noh->esquerda = NULL;
    noh->pVencedor = NULL;
    noh->vencedor = HIGH_VALUE;
    return noh;
}

void liberarNoh (arvoreVencedor* noh) {

}

// Constrói a árvore com todos arquivos passados
void montarArvore (arvoreVencedor* raiz, FILE** arquivos) {
    // Array dinâmico para manter referência de todas folhas
    arvoreVencedor** folhas = (arvoreVencedor**) malloc(sizeof(arvoreVencedor*)*nArquivos);
    Cliente* aux = NULL;

    for(int i=0; i<nArquivos; i++) {
        folhas[i] = (arvoreVencedor*) malloc(sizeof(arvoreVencedor)*nArquivos);
    }

    // Construção das folhas com o ponteiro de cada arquivo e o código de cliente do primeiro cliente do arquivo
    for(int i=0; i<nArquivos; i++) {
        aux = lerClienteDeArquivo(arquivos[i], 0);
        folhas[i]->vencedor = aux->codCliente;
        folhas[i]->pVencedor = arquivos[i];
        folhas[i]->direita = folhas[i]->esquerda = NULL;
        rewind(arquivos[i]);
    }

    _adicionarNos (raiz, folhas, nArquivos);

    free(folhas);
    folhas = NULL;
}

// < - ! - > Função para uso interno < - ! - > Função interna recursiva para construir os nós internos e retornar a árvore completa
void _adicionarNos (arvoreVencedor* raiz, arvoreVencedor** nos, int nSubArvores) {
    int nNos = -1;
    int j = 0;
    int loops = -1;

    // Condição de parada, caso haja 2 ou menos sub-arvores
    if(nSubArvores <= 2) {
        switch (nSubArvores) {
            case 2:
                raiz->esquerda = nos[0];
                raiz->direita = nos[1];

                if (nos[0]->vencedor <= nos[1]->vencedor) {
                    raiz->vencedor = nos[0]->vencedor;
                    raiz->pVencedor = nos[0]->pVencedor;
                } else {
                    raiz->vencedor = nos[1]->vencedor;
                    raiz->pVencedor = nos[1]->pVencedor;
                }
                return;
            case 1:
                raiz->esquerda = nos[0];
                raiz->vencedor = nos[0]->vencedor;
                raiz->pVencedor = nos[0]->pVencedor;
                return;
            default:
                exit(EXIT_FAILURE);
        }
    }
    // Caso contrário, são alocados ceil(nSubArvores/2) nós para construir o nível superior da árvore
    if(nSubArvores%2 != 0)
        nNos = nSubArvores/2 + 1;
    else
        nNos = nSubArvores/2;

    arvoreVencedor** novosNos = (arvoreVencedor**) malloc(sizeof(arvoreVencedor*)*nNos);
    for(int i=0; i<nNos; i++) {
        novosNos[i] = (arvoreVencedor*) malloc(sizeof(arvoreVencedor)*nSubArvores);
    }
    // Caso haja um número ímpar de sub-árvores, o último nó copia o último nó do nível inferior, subindo-o de nível. Uma flag marca que o array tem tamanho ímpar.
    if(nSubArvores%2 == 1) {
        novosNos[nNos-1] = nos[nSubArvores-1];
        loops = nNos - 1;
    } else {
        loops = nNos;
    }

    // Cada nó do nível atual tem 2 sub-árvores do nível inferior alocadas nos ponteiros esq/dir e o menor código das duas é colocado como vencedor
    for(int i=0; i<loops; i++) {
        novosNos[i]->esquerda = nos[j];
        novosNos[i]->direita = nos[j+1];

        if (nos[j]->vencedor <= nos[j+1]->vencedor) {
            novosNos[i]->vencedor = nos[j]->vencedor;
            novosNos[i]->pVencedor = nos[j]->pVencedor;
        } else {
            novosNos[i]->vencedor = nos[j+1]->vencedor;
            novosNos[i]->pVencedor = nos[j+1]->pVencedor;
        }
        j+=2;
    }

    _adicionarNos (raiz, novosNos, nNos);

    free(novosNos);
    novosNos = NULL;
}

// Lê o próximo registro do arquivo vencedor e atualiza a árvore
Cliente* atualizarFolha (arvoreVencedor* raiz) {
    Cliente* vencedor = criarCliente();
    Cliente* proxCliente = criarCliente();
    int posPonteiroArquivo = -1;

    // Lê o atual vencedor, que será usado para gravar no arquivo de saída
    if(freadCliente(raiz->pVencedor, vencedor)) {
        perror("Erro ao ler vencedor");
        exit(EXIT_FAILURE);
    }

    // Salva a posição atual do ponteiro do arquivo para retornar após ler o próximo cliente
    if((posPonteiroArquivo = ftell(raiz->pVencedor)) == -1L) {
        perror("Erro ao voltar ponteiro do arquivo");
        exit(EXIT_FAILURE);
    }

    // Lê o próximo cliente do arquivo vencedor para atualizar o vencedor daquela folha
    switch (freadCliente(raiz->pVencedor, proxCliente)) {
        case 0:
            // Retorna à posição anterior para poder reler o vencedor
            if(fseek(raiz->pVencedor, posPonteiroArquivo, SEEK_SET)) {
                perror("Erro ao retornar ponteiro do arquivo vencedor [1]");
                exit(EXIT_FAILURE);
            }
            _atualizarNos(raiz, proxCliente->codCliente);
            break;
        case 1:
            perror("Erro ao ler arquivo para atualizar folha");
            exit(EXIT_FAILURE);
        case 2:
            if(fseek(raiz->pVencedor, posPonteiroArquivo, SEEK_SET)) {
                perror("Erro ao retornar ponteiro do arquivo vencedor [2]");
                exit(EXIT_FAILURE);
            }
            _atualizarNos(raiz, HIGH_VALUE);
            break;
        default:
            perror("Erro ao atualizar próxima folha");
            exit(EXIT_FAILURE);
    }
    return vencedor;
}

// < - ! - > Função para uso interno < - ! - > Percorre o caminho vencedor da árvore recursivamente para atualizar o vencedor de todos nós
void _atualizarNos (arvoreVencedor* noh, int novoVencedor) {
    int updateFlag = -1;
    // Condição de parada, chegar a uma folha (sem filhos)
    if(noh->direita == NULL && noh->esquerda == NULL) {
        noh->vencedor = novoVencedor;
        if(novoVencedor == HIGH_VALUE) {
            fclose(noh->pVencedor);
            noh->pVencedor = NULL;
        }
        return;
    }

    // Chamada recursiva caso não esteja na folha
    if(noh->esquerda->vencedor == noh->vencedor) {
        _atualizarNos(noh->esquerda, novoVencedor);
    } else if(noh->direita->vencedor == noh->vencedor) {
        _atualizarNos(noh->direita, novoVencedor);
    }

    // No retorno das chamadas, atualiza o vencedor e ponteiro do vencedor de cada nó
    if(noh->esquerda == NULL || noh->direita == NULL) {
        if(noh->esquerda == NULL)
            updateFlag = 1;
        else
            updateFlag = 0;
    } else {
        if(noh->esquerda->vencedor > noh->direita->vencedor)
            updateFlag = 1;
        else
            updateFlag = 0;
    }
    if(updateFlag == 1) {
        noh->vencedor = noh->direita->vencedor;
        noh->pVencedor = noh->direita->pVencedor;
    } else {
        noh->vencedor = noh->esquerda->vencedor;
        noh->pVencedor = noh->esquerda->pVencedor;
    }
}

// Lê o vencedor atual e o grava no arquivo de saída
void intercalar (arvoreVencedor* raiz) {
    char nome[] = ARQUIVO_DE_SAIDA;
    FILE* saida = fopen(nome, "wb");
    Cliente* aux = (Cliente*) malloc(sizeof(Cliente));

    while(raiz->vencedor != HIGH_VALUE) {
        aux = atualizarFolha(raiz);
        fwriteCliente(saida, aux);
    }
    fclose(saida);
    free(aux);
}
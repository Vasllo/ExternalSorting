#include "pilha.h"

int pilha_cheia(int lim, int topo)
{
    if (topo == lim)
        return 1;
    else
        return 0;
}

int pilha_vazia(int base, int topo)
{
    if (topo < base)
        return 1; //pilha vazia
    else
        return 0; //pilha tem pelo menos 1 elemento
}

Cliente *push(TPilha *pilha, int lim, int *topo, Cliente *info, int p)
{
    if (pilha_cheia(lim, *topo))
    {
        return NULL;
    }
    else
    {
        //faz a inserção
        *topo = *topo + 1;
        pilha[*topo].info = info;
        pilha[*topo].p = p;
        return info;
    }
}

Cliente *pop(TPilha *pilha, int base, int *topo)
{
    if (pilha_vazia(base, *topo))
    {
        return NULL;
    }
    else
    {
        //faz a remoção
        Cliente *info = pilha[*topo].info;
        *topo = *topo - 1;
        return info;
    }
}

Cliente *peek_cliente(TPilha *pilha, int base, int *topo)
{
    if (pilha_vazia(base, *topo))
        return NULL;
    else
    {
        //faz consulta
        return pilha[*topo].info;
    }
}

int peek_fp(TPilha *pilha, int base, int *topo)
{
    if (pilha_vazia(base, *topo))
        return NULL;
    else
    {
        //faz consulta
        return pilha[*topo].p;
    }
}

void imprime_pilha(TPilha *pilha, int *topo)
{
    printf("\nEstado atual da Pilha:\n");
    for (int i = *topo; i >= 0; i--)
    {
        printf("posicao [%d]: %d\n", i, pilha[i].info->cod);
    }
    printf("\n");
}

void cria_pilha_particao(TPilha *pilha, FILE *arq, int nCli, int *vetTopo)
{

    int lim = nCli - 1;

    rewind(arq);
    int i = nCli - 1;
    fseek(arq, 0 * sizeof(nCli), SEEK_SET);
    if (arq != NULL)
    {
        int aux = 1;
        do
        {
            fseek(arq, (i)*tamanho_registro(), SEEK_SET);
            Cliente *cli = le_cliente(arq);
            if (cli)
            {
                push(pilha, lim, vetTopo, cli, arq);
                i--;
            }
            else
                aux = 0;
        } while (aux);
    }
    else
        printf("Erro ao abrir arquivo\n");

    imprime_pilha(pilha, vetTopo);
}
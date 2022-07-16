#include <stdlib.h>

#include "lista.h"
#include "particoes.h"
#include "intercalacao.h"

int menu()
{
    int op;
    printf("\nDigite 1 para utilizar a criação da estrutura.");
    printf("\nDigite 2  para gerar particoes ordenadas utilizando selecao natural.");
    printf("\nDigite 3 para criar uma pilha com o conteudo das particoes geradas.");
    printf("\nDigite 4  intercalacao arvore de vencedores.");
    printf("\nDigite -1 para sair: ");
    scanf("%d", &op);

    return op;
}

int main()
{

    /*criando um arquivo lógico para abir para leitura e escrita o arquivo binário*/
    FILE *arq = fopen("base_cliente.dat", "wb+");
    if (arq == NULL)
    {
        printf("Arquivo base_cliente.dat não encontrado.\n");
        return 1;
    }

    int op = menu();
    int nCli;

    int nParticoes, nElementos;
    Lista *nomes = NULL;

    TPilha **vetPilhas;
    int crioubase = 0, crioulistadenomes = 0, criouparticoes = 0, crioupilhas = 0;

    //ponteiro para salvar o início da lista de nomes
    Lista *prox;
    int *vetTopo;

    while (op != -1)
    {
        switch (op)
        {
        case (1):
            nParticoes = 10;
            nElementos = 2;
            system("cls");
            printf("\n\nA estrutura de interpolação será composta por 10 arquivos de no máximo 2 elementos.\n");
            nomes = cria("p1.dat", cria("p2.dat", cria("p3.dat", cria("p4.dat", cria("p5.dat", cria("p6.dat", cria("p7.dat", cria("p8.dat", cria("p9.dat", cria("p10.dat", NULL))))))))));
            crioulistadenomes = 1;
            imprime(nomes);
            printf("\n");
            break;
        case (2):
            if (crioulistadenomes)
            {
                system("cls");
                printf("\n\nGerando partições, utilizando o método selecao natural.\n");
                selecao_natural(arq, nomes, nElementos, nCli, 1, &nParticoes);
                printf("Total de %d partições geradas.\n", nParticoes);
                //salvando o ponteiro para o início da lista de nomes
                prox = nomes;
                criouparticoes = 1;
            }
            else
                printf("Lista de nomes de arquivos inexistente para gerar partições, crie a estrutura utilizando a opção  1 do menu!!!\n");
            break;
          case (3):
             if (criouparticoes)
            {
                system("cls");
                //criando pilha com o conteúdo das partições geradas
                vetPilhas = (TPilha **)malloc(sizeof(TPilha *) * (nParticoes));
                vetTopo = (int *)malloc(sizeof(int) * (nParticoes));
                for (int p = 0; p < nParticoes; p++)
                {
                    //alocando a memória para salvar uma pilha para cada particao
                    vetPilhas[p] = (TPilha *)malloc(sizeof(TPilha) * (nElementos));
                    //abrindo primeira partição para leitura
                    FILE *part = fopen(nomes->nome, "rb+");
                    printf("Particao %s \n", nomes->nome);
                    int tamanho = nomes->tamanho == -1 ? nElementos : nomes->tamanho + 1;
                    nomes = nomes->prox;
                    //imprimindo os cliente gravados nas partições
                    imprime_base_dados_cliente(part, nElementos);
                    //inicializando os topos das pilhas
                    vetTopo[p] = -1;
                    //criando pilha através da leitura da partição
                    cria_pilha_particao(vetPilhas[p], part, tamanho, &vetTopo[p]);
                    printf("Gerou a pilha\n\n***************\n\n");
                    //fechando a partição
                    fclose(part);
                }
                crioupilhas = 1;
                nomes = prox;
            }
            else
                printf("Partições não foram criadas, crie as partições utilizando opção 2 do menu!!!\n");
            break;
        case (4):
            if (criouparticoes)
            {
                if (crioupilhas)
                {
                    system("cls");
                    intercalacao_arvore_de_vencedores(vetPilhas, vetTopo, "arquivo_intercalado.dat", nParticoes, nCli);
                }
                else
                    printf("As pilhas das partições geradas não foram criadas, crie as pilhas utilizando as opção 3!!!\n");
            }
            else
                printf("Partições não foram criadas, crie as partições utilizando as opção 2 do menu!!!\n");

            //imprimindo o arquivo intercalado
            FILE *fi = fopen("arquivo_intercalado.dat", "rb+");
            imprime_base_dados_cliente(fi, nCli);
            printf("\n");
            fclose(fi);
            break;
            
    while (nomes)
    {
        prox = nomes->prox;
        free(nomes);
        nomes = prox;
    }

    for (int p = 0; p < nParticoes; p++)
    {
        free(vetPilhas[p]);
    }
    free(vetPilhas);

   
    fclose(arq);

    return 0;

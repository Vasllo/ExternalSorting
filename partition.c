#include "cliente.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

#include "partition.h"


Cliente* getCliente(FILE* arq, int* contLidos)
{
    fseek(arq, *contLidos * tamanho_registro(), SEEK_SET);
    Cliente* cli = le_cliente(arq);
    (*contLidos)++;
    return cli;
}

// Função que recebe o indice do menor elemento do vetor,
// joga ele para o inicio e retorna sua posição
int menorParaInicio(Cliente* v[], int M, int menor)
{
    Cliente* aux;
    if(menor == 0)
    {
        return(0);
    }
    else
    {
        for(int i = M-1; i > 0; i--)
        {
            if (i == menor)
            {
                aux = v[menor];
                v[i] = v[i-1];
                v[i-1] = aux;
                menor--;
            }
        }
    }
    return(menor);
}

// Função que retorna a quantidade de elementos dentro do vetor
int elementosNoVetor(bool controle[], int tam)
{
    int aux = 0;
    for (int i = 0; i < tam; i++)
    {
        if (controle[i] == false)
        {
            aux++;
        }
    }
    return(aux);
}

// Procedimento que cria novas partições caso necessário
void atualizaNomesParticao(Lista* nomes, int* nParticoes)
{
    if(nomes->prox == NULL)
    {
        char* newNome = malloc(5 * sizeof(char));
        (*nParticoes)++;
        sprintf(newNome, "p%d.dat", *nParticoes);
        nomes->prox = cria(newNome, NULL);
    }
}

// Função que retorna o índice do menor registro do vetor
int getMenor(Cliente* v[], int tam)
{
    int menor = 0;
    for (int i = 1; i < tam; i++)
    {
        if(v[menor]->cod > v[i]->cod)
        {
            menor = i;
        }
    }
    return menor;
}

// Função para verificar se um vetor de tamanho tam possui elementos
int possuiElementos(Cliente* v[], int tam)
{
    for (int i = 0; i < tam; i++)
    {
        if(v[i] != NULL){
            return(1);
        }
    }
    return(0);
}

void classificacao_interna(FILE *arq, Lista *nome_arquivos_saida, int M, int nCli)
{
    rewind(arq); //posiciona cursor no inicio do arquivo
    int reg = 0;

    while (reg != nCli)
    {
        //le o arquivo e coloca no vetor
        Cliente *v[M];
        int i = 0;
        while (!feof(arq))
        {
            fseek(arq, (reg) * tamanho_registro(), SEEK_SET);
            v[i] = le_cliente(arq);
            //     =
            i++;
            reg++;
            if(i>=M)
                break;
        }

        
        if (i != M)
        {
            M = i;
        }

     
        for (int j = 1; j < M; j++)
        {
            Cliente *f = v[j];
            i = j - 1;
            while ((i >= 0) && (v[i]->codCliente > f->codCliente))
            {
                v[i + 1] = v[i];
                i = i - 1;
            }
            v[i + 1] = f;
        }

        
        char *nome_particao = nome_arquivos_saida->nome;
        nome_arquivos_saida = nome_arquivos_saida->prox;
        printf("\n%s\n", nome_particao);
        FILE *p;
        if ((p = fopen(nome_particao, "wb+")) == NULL)
        {
            printf("Erro criar arquivo de saida\n");
        }
        else
        {
            for (int i = 0; i < M; i++)
            {
                fseek(p, (i) * tamanho_registro(), SEEK_SET);
                salva_(v[i], p);
                imprime_cliente(v[i]);
            }
            fclose(p);
        }
        for(int jj = 0; jj<M; jj++)
            free(v[jj]);
    }
}

void selecao_natural(FILE *arq, Lista *nome_arquivos_saida, int M, int nCli, int n, int* nParticoes)
{
    Lista* nomes = nome_arquivos_saida;
    Lista* nome_part = nomes;
    FILE* particao = NULL;
    int cursorPart = 0;
    int totalLidos = 0;

    Cliente** v = malloc(M * sizeof(Cliente*));

    FILE* res = fopen("reservatorio.dat", "w+");
    int cursorRes = 0;
    bool reservatorio_cheio;

    bool controleVet[M];
    int i = 0;

    // M registros do arquivo para a memoria
    while (i < M && (!feof(arq)))
    {
        v[i] = getCliente(arq, &totalLidos);
        controleVet[i] = false;
        i++;
    }

    // Abre arquivo de partição
    if(particao == NULL)
    {
        char* nome = nomes->nome;
        particao = fopen(nome, "wb+");
        nome_part = nomes;

        atualizaNomesParticao(nomes, nParticoes);
        nomes = nomes->prox;
    }

    // Enquanto há elementos no vetor e
    // todos os registros não foram lidos
    while (possuiElementos(v, M) && totalLidos <= nCli)
    {
        reservatorio_cheio = false;
        // Enquanto não chega ao fim do arquivo e
        // o reservatorio não está cheio
        while (!feof(arq) && !reservatorio_cheio)
        {
            // Caso não haja partição aberta
            // abre a próxima partição
            if(particao == NULL)
            {
                char* nome = nomes->nome;
                particao = fopen(nome, "wb+");
                nome_part = nomes;

                atualizaNomesParticao(nomes, nParticoes);
                nomes = nomes->prox;
            }
            // Caso a partição atual esteja  cheia
            if(cursorPart >= M)
            {
                fclose(particao);
                particao = NULL;
                cursorPart = 0;
                continue;
            }
            // Pega o menor valor do vetor
            int menor = getMenor(v, M);
            menor = menorParaInicio(v, M, menor);

            // Salva o menor registro na partição
            fseek(particao, cursorPart * tamanho_registro(), SEEK_SET);
            salva_cliente(v[menor], particao);

            // Salva o cod do registro particionado acima
            int lastKey = v[menor]->cod;
            cursorPart++;
            nome_part->tamanho++;
            controleVet[menor] = true;

            // Enquanto o indice do registro particionado não for trocado e
            // nenhuma das condições de parada anteriores for verdadeira
            while (controleVet[menor] && !reservatorio_cheio && !feof(arq) && totalLidos < nCli)
            {
                if (!feof(arq))
                {
                    // Troca o registro já particionado com próximo registro do arquivo
                    v[menor] = getCliente(arq, &totalLidos);

                    // Se for maior do que o registro da partição
                    // grava na partição normalmente
                    if (v[menor]->cod >= lastKey)
                    {
                        // Caso a partição atual esteja cheia
                        // abre a próxima partição
                        if(cursorPart >= M)
                        {
                            fclose(particao);
                            particao = NULL;
                            cursorPart = 0;

                            char* nome = nomes->nome;
                            particao = fopen(nome, "wb+");
                            nome_part = nomes;

                            atualizaNomesParticao(nomes, nParticoes);
                            nomes = nomes->prox;
                        }
                        fseek(particao, cursorPart * tamanho_registro(), SEEK_SET);
                        salva_cliente(v[menor], particao);
                        controleVet[menor] = false;
                    }
                    // Se for menor do que o registro da partição
                    // grava no reservatório e substitui pelo próximo
                    else
                    {
                        fseek(res, cursorRes * tamanho_registro(), SEEK_SET);
                        salva_cliente(v[menor], res);
                        cursorRes++;
                        if (cursorRes == n)
                        {
                            reservatorio_cheio = true;
                        }
                    }
                }
            }
        }
        // Se ja leu todos os registros está no fim da execução
        // Abre nova partição e armazena os registros restantes
        if(totalLidos == nCli)
        {
            fclose(particao);
            particao = NULL;
            cursorPart = 0;
            char* nome = nomes->nome;
            particao = fopen(nome, "wb+");
            nome_part = nomes;

            for(i = 0; i < M; i++)
            {
                fseek(particao, i * tamanho_registro(), SEEK_SET);
                salva_cliente(v[i], particao);
            }
            cursorPart = 0;
            cursorRes = 0;
        }
        // Se a partição está cheia abre a próxima
        if(cursorPart >= M)
        {
            fclose(particao);
            particao = NULL;
            cursorPart = 0;

            char* nome = nomes->nome;
            particao = fopen(nome, "wb+");
            nome_part = nomes;

            atualizaNomesParticao(nomes, nParticoes);
            nomes = nomes->prox;
        }
        // Se há elementos no reservatório, os passa para o vetor
        if (cursorRes > 0)
        {
            for (i = 0; i < cursorRes; i++)
            {
                fseek(res, i * tamanho_registro(), SEEK_SET);
                v[i] = le_cliente(res);
                controleVet[i] = false;
                cursorRes--;
            }
        }
        int cont = elementosNoVetor(controleVet, M);

        // Se há espaço no vetor, o preenche com registros do arquivo
        if (cont != M && (!feof(arq)))
        {
            while (cont < M && (!feof(arq)))
            {
                v[cont] = getCliente(arq, &totalLidos);
                controleVet[cont] = false;
                cont++;
            }
        }
    }
    // Libera a memoria alocada
    for (int i = 0; i < M; i++)
    {
        free(v[i]);
    }
    fclose(particao);
    free(v);
}

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "lista_encadeada.h"

#define M 10000

typedef struct Client{

 int codCliente;
 char nome [50];
 char data [11];

}Cliente;


void ordenar_arquivo(char *nome_arquivo, int *qtd_particoes);
Cliente encontrar_menor_registro(Cliente* memoria, int tamanho, int *index_menor);
void gravar_particao(FILE* particao, Cliente *registro);
int ler_reposicao(FILE *arquivo, FILE *particao, FILE *reservatorio, int *tamanho_reservatorio, Cliente* menor_registro, Cliente* registro_reposicao);
int gravar_reservatorio(FILE *reservatorio, int *tamanho_reservatorio, Cliente *registro_reposicao);
void gravar_memoria(FILE *particao, Cliente *memoria, int tamanho);
Cliente* ordernar_memoria(Cliente *memoria, int tam);
void intercalacao_F3(char *particao1, char *particao2, char *particao_saida);
void gerenciar_intercalacao(int *qtd_particoes);
void imprimir_particao(char* nome_arquivo);



int main (int ac, char **av){
    if(strcmp(av[1], "classificacao") == 0 ){
        FILE *arquivo_entrada = fopen("cliente.dat", "rb");
        Cliente c;
        int qtd_particoes = 1;
        assert(arquivo_entrada != NULL);
        
        int i = 1;
        while ((fread(&c, sizeof(Cliente), 1, arquivo_entrada) > 0)){
            printf("%d - %d \n", i, c.codCliente);
            i++;
        }//while (fread(&c, sizeof(Cliente), 1, arquivo_entrada) > 0){

        ordenar_arquivo("cliente.dat", &qtd_particoes);  
        gerenciar_intercalacao(&qtd_particoes);
        fclose(arquivo_entrada);
    } 

    return 1;
}

void ordenar_arquivo(char *nome_arquivo, int *qtd_particoes){
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if(arquivo == NULL){
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", nome_arquivo);
        exit(EXIT_FAILURE);
    }

    FILE *particao = NULL;
    FILE *reservatorio = NULL;
    char *titulo_particao = (char *) malloc(2 * M * sizeof(char));

    Cliente *memoria = (Cliente *) malloc(M * sizeof(Cliente));

    int tamanho_reservatorio = 0,
        tamanho_memoria = M - 1,
        indice_menor_registro = 0;
    
    Cliente registro_reposicao;

    fread(memoria, sizeof(Cliente), M, arquivo);
    
    sprintf(titulo_particao, "part-%.5d.dat", *qtd_particoes);
 
    particao = fopen(titulo_particao, "wb");
    assert(particao != NULL);

    reservatorio = fopen("reservatorio.dat", "wb+");

    Cliente menor_registro;
    int fim = 0;

    while(!fim){
        menor_registro = encontrar_menor_registro(memoria, M, &indice_menor_registro);

        gravar_particao(particao, &menor_registro);
        int resultado = ler_reposicao(arquivo, particao, reservatorio, &tamanho_reservatorio, &menor_registro, &registro_reposicao);

        if(resultado == 1){
            memoria[indice_menor_registro] = registro_reposicao;
        } else {
            gravar_memoria(particao, memoria, tamanho_memoria);
            fclose(particao);
            int aux = *qtd_particoes;
            aux++;
            *qtd_particoes = aux;
            sprintf(titulo_particao, "part-%.5d.dat", *qtd_particoes);
            
            if(resultado == 2){
                fflush(reservatorio);

                fseek(reservatorio, 0, SEEK_SET);

                assert(fread(memoria, sizeof(Cliente), tamanho_reservatorio, reservatorio) > 0);

                fseek(reservatorio, 0, SEEK_SET);

                tamanho_reservatorio = 0;

                particao = fopen(titulo_particao, "wb");

                assert(particao != NULL);

            } else if(resultado == -1)  {
                if(tamanho_reservatorio > 0) { 
                    fseek(reservatorio, 0, SEEK_SET);

                    assert(fread(memoria, sizeof(Cliente), tamanho_reservatorio, reservatorio) > 0);
                    
                    fclose(reservatorio);

                    particao = fopen(titulo_particao, "wb");

                    assert(particao != NULL);

                    gravar_memoria(particao, memoria, tamanho_reservatorio);

                    fclose(particao);
                }
                fim = 1;
            }
        }
    }
}

Cliente encontrar_menor_registro(Cliente *memoria, int tamanho, int *index_menor){
    Cliente menor_registro = memoria[0];
    int indice = 0;
    for(int i = 1; i < tamanho; i++){
        if(menor_registro.codCliente > memoria[i].codCliente) {
            indice = i;
            menor_registro = memoria[i];
        }
    }
    *index_menor = indice; 
    return menor_registro;
}

void gravar_particao(FILE *particao, Cliente *registro){

    assert(fwrite(registro, sizeof(Cliente), 1, particao) > 0);

    fflush(particao);
}

int ler_reposicao(FILE *arquivo, FILE *particao, FILE *reservatorio, int *tamanho_reservatorio, Cliente* menor_registro, Cliente* registro_reposicao){
    int resultado;
    int fim = 0;

    while(!fim){
        if(fread(registro_reposicao, sizeof(Cliente), 1, arquivo) > 0){
            if(menor_registro->codCliente < registro_reposicao->codCliente){
                resultado = 1;
                fim = 1;
            } else {
                int resultado_reservatorio = 0;
                resultado_reservatorio = gravar_reservatorio(reservatorio, tamanho_reservatorio, registro_reposicao);

                if(resultado_reservatorio == 1){
                    resultado = 2;
                    fim = 1;
                }
            }
        }else{
            resultado = -1;
            fim = 1;
        }
    }    
    return resultado;
}

int gravar_reservatorio(FILE *reservatorio, int *tamanho_reservatorio, Cliente *registro_reposicao){
    assert(fwrite(registro_reposicao, sizeof(Cliente), 1, reservatorio) > 0);
    
    int indice = *tamanho_reservatorio;
    indice++;
    *tamanho_reservatorio = indice;

    if(indice == M) return 1;

    return -1;
}

void gravar_memoria(FILE *particao, Cliente *memoria, int tamanho){
    memoria = ordernar_memoria(memoria, tamanho);

    for(int i = 0; i < tamanho; i++){
        assert(fwrite((memoria+i), sizeof(Cliente), 1, particao) > 0);
    }
}

Cliente* ordernar_memoria(Cliente *memoria, int tam){
    Cliente temp;

    for(int end = tam - 1; end
     > 0; end--){
        int posicao_maior = 0;

        for(int i = 0; i < end; i++){
            if(memoria[i].codCliente > memoria[posicao_maior].codCliente){
                posicao_maior = i;
            }
        }

        temp = memoria[end];
        memoria[end] = memoria[posicao_maior];
        memoria[posicao_maior] = temp;
    }
    
    return memoria;
}

void intercalacao_F3(char *particao1, char *particao2, char *particao_saida){
    FILE *part1 = fopen(particao1, "rb");
    FILE *part2 = fopen(particao2, "rb");
    FILE *saida = fopen(particao_saida, "wb");

    Cliente* registro_part1 = (Cliente*) malloc(sizeof(Cliente));
    Cliente* registro_part2 = (Cliente*) malloc(sizeof(Cliente));

    int fim_part1 = 0,
        fim_part2 = 0,
        resultado = 0;

    assert(part1 != NULL);
    assert(part2 != NULL);
    assert(saida != NULL);

    fim_part1 = fread(registro_part1, sizeof(Cliente), 1, part1);
    fim_part2 = fread(registro_part2, sizeof(Cliente), 1, part2);

    do{
        
        if(registro_part1->codCliente < registro_part2->codCliente){
            fwrite(registro_part1, sizeof(Cliente), 1, saida);
            fim_part1 = (fread(registro_part1, sizeof(Cliente), 1, part1));
        } else {
            fwrite(registro_part2, sizeof(Cliente), 1, saida);
            fim_part2 = (fread(registro_part2, sizeof(Cliente), 1, part2));
        }
    } while(((fim_part1) > 0) && ((fim_part2) > 0 ));

    if(fim_part1 == 0){
        do{
            fwrite(registro_part2, sizeof(Cliente), 1, saida);
            fim_part2 = fread(registro_part2, sizeof(Cliente), 1, part2);
        } while (fim_part2 > 0);
    } else if(fim_part2 == 0){
        do{
            fwrite(registro_part1, sizeof(Cliente), 1, saida);
            fim_part1 = fread(registro_part1, sizeof(Cliente), 1, part1);
        } while(fim_part1 > 0);
    } else { 
        fprintf(stderr, "Erro ao fazer merge");
        exit(EXIT_FAILURE);
    }
    fclose(part1);
    fclose(part2);
    
    assert(remove(particao1) == 0); /*remover partições que já foram unidas para poupar espaço em disco */
    assert(remove(particao2) == 0);

    fclose(saida);
}

void gerenciar_intercalacao(int *qtd_particoes){
    if(*qtd_particoes > 1){
        char *titulo_saida = (char*) malloc(2*M*sizeof(char));
        int tamanho_lista = 0;

        Lista* lista_particoes = lst_cria();

        char *titulo_particoes = (char*) malloc(sizeof(char)*M*2);
        
        for(int i = 1; i <= *qtd_particoes; i++){
            sprintf(titulo_particoes, "part-%.5d.dat", i);
            lista_particoes = lista_insere(lista_particoes, titulo_particoes, &tamanho_lista);
        }

        Lista *p = lista_particoes;

        int qtd_particoes_saida = 0;
        
        int qtd_merges = 0;
        while((tamanho_lista) > 1){
            sprintf(titulo_saida, "merge-%.5d.dat", ++qtd_particoes_saida);

            printf("Fazendo merge entre %s e %s\n..", p->nomeParticao, p->prox->nomeParticao);

            intercalacao_F3(p->nomeParticao, p->prox->nomeParticao, titulo_saida);
            qtd_merges++;
            lista_particoes = lista_insere(lista_particoes, titulo_saida, &tamanho_lista);
            tamanho_lista -=2;
            p = p->prox->prox;
            lista_particoes = remove_inicio(lista_particoes);
            lista_particoes = remove_inicio(lista_particoes);
        }
        printf("-------------------------------------------------------");
        printf("Part final = %s \n Quantidade de merges realizados: %d", titulo_saida, qtd_merges);
        printf("---------------------------------------------------------");
        
        libera_lst(lista_particoes);

        imprimir_particao(titulo_saida);
    }
}

void imprimir_particao(char *nome_arquivo){
    printf("Lendo saida...");
    FILE *particao = NULL;
    Cliente c;
    particao = fopen(nome_arquivo, "rb");
    assert(particao != NULL);

    int i = 1;

    while ((fread(&c, sizeof(Cliente), 1, particao) > 0)){
        printf("%d - %d \n", i, c.codCliente);
        i++;
    }
    fclose(particao);
}


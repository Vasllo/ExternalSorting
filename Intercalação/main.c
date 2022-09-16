#include "partition.h"
#include <time.h>

#define TAM_AMOSTRAGEM 20 // Número de elementos a serem gerados
#define TAM_MEMORIA 128 //  Em bytes, a struct tem 64B
#define NUM_NOMES 10 // Quantidade de nomes disponíveis
#define TAM_NOMES 49 // Quantos caracteres cada nome pode ter

void criarExemplo();

int main() {
    if(1) {
        FILE* arquivo = fopen("clientes.dat", "w+b");
        if(!arquivo){
            printf("Deu ruim\n");
            exit(EXIT_FAILURE);
        }
        criarExemplo(arquivo);
        particionar(arquivo, TAM_MEMORIA);
    }

    if(0) {
        FILE* arquivo = fopen("clientes_particao_0.dat", "r+b");
        if(!arquivo){
            printf("Deu ruim\n");
            exit(EXIT_FAILURE);
        }
        Cliente* teste = criarCliente();
        teste = lerClienteDeArquivo(arquivo, 0);
        imprimirCliente(teste);
    }

    return 0;
}

void criarExemplo(FILE* arquivo) {
    int auxInt = -1, auxInt2 = -1, auxInt3 = -1;
    char auxString[12] = {'\0'};
    char nomes[NUM_NOMES][TAM_NOMES] = {"Rogerinho", "Reinaldson", "Rolandinho", "Romarido", "Randerson", "Ramarel", "Ruestafo", "Rambo", "Rickastley", "Astrogildo"};
    Cliente cliente;

    srand(time(NULL));

    for(int i=TAM_AMOSTRAGEM; i>=0; i--) {
        auxInt = rand() % NUM_NOMES;
        strncpy(cliente.nome, nomes[auxInt], 49);

        auxInt = rand() % 20 + 1980;
        auxInt2 = rand() % 12 + 1;
        auxInt3 = rand() % 31 + 1;
        sprintf(auxString, "%2d/%2d/%4d", auxInt3, auxInt2, auxInt);
        strncpy(cliente.dataDeNascimento, auxString, 11);

        auxInt = rand() % TAM_AMOSTRAGEM*10;
        cliente.codCliente = auxInt;

        fwriteCliente(arquivo, &cliente);
    }
}
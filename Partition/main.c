#include "partition.h"
#include <time.h>

#define TAM_AMOSTRAGEM 8
#define TAM_MEMORIA 128 //  Em bytes, a struct tem 64B
#define NUM_NOMES 10

void criarExemplo();

int main() {
    FILE* arquivo = fopen("clientes.dat", "w+b");
    criarExemplo(arquivo);

    particionar(arquivo, TAM_MEMORIA);

    return 0;
}

void criarExemplo(FILE* arquivo) {
    int auxInt = -1, auxInt2 = -1, auxInt3 = -1;
    char auxString[12] = {'\0'};
    char nomes[NUM_NOMES][50] = {"Rogerinho", "Reinaldson", "Rolandinho", "Romarido", "Randerson", "Ramarel", "Ruestafo", "Rambo", "Rickastley", "Astrogildo"};
    Cliente cliente;

    srand(time(NULL));

    for(int i=0; i<TAM_AMOSTRAGEM; i++) {
        auxInt = rand() % NUM_NOMES;
        strncpy(cliente.nome, nomes[auxInt], 49);

        auxInt = rand() % 20 + 1980;
        auxInt2 = rand() % 12 + 1;
        auxInt3 = rand() % 31 + 1;
        sprintf(auxString, "%2d/%2d/%4d", auxInt3, auxInt2, auxInt);
        strncpy(cliente.dataDeNascimento, auxString, 11);

        auxInt = rand() % TAM_AMOSTRAGEM*10;
        cliente.codCliente = auxInt;

        fprintCliente(arquivo, &cliente);
    }
}
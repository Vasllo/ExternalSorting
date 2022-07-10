//  Algoritmo de particionamento de arquivos pelo algoritmo de seleção natural

#include "cliente.h"
#include <math.h>
#include <string.h>

#define TAM_NOME_PARTICAO 30
#define HIGH_VALUE 2147483647

void particionar (FILE* arquivo, int tamanhoMemoria);
int valorMinimo (Cliente* clientes, int unidadesEmMemoria);
void apagarCliente (Cliente* cliente);
void fprintCliente(FILE* arquivo, Cliente* cliente);
void freadCliente(FILE* arquivo, Cliente* destino);

//  tamanho de memória é dado em bytes, representa o espaço disponível na RAM
void particionar (FILE* arquivo, int tamanhoMemoria) {
    int numeroParticao = 0, posReservatorio = -1, idMinValor = -1, ultimValor = -1, eoflag = 1;
    char nomeParticao[TAM_NOME_PARTICAO] = {'\0'};

    //  Reservatório precisa ser escrito e lido diversas vezes, e o arquivo deve estar com o ponteiro em seu início
    FILE* reservatorio = fopen("reservatorio.dat", "w+b");
    rewind(arquivo);

    //  É calculado o tamanho da estrutura "Cliente" e então calculado quantas destas cabem na memória dada, e então é alocado um array deste tamanho
    Cliente* clientes = NULL;
    // Burrice abaixo?
    int tamanhoStruct = sizeof(clientes->codCliente) + sizeof(clientes->nome) + sizeof(clientes->dataDeNascimento);
    int unidadesEmMemoria = (int) floor((double)(tamanhoMemoria/tamanhoStruct));
    clientes = (Cliente*) malloc(sizeof(Cliente)*unidadesEmMemoria);

    //  Leitura inicial para preencher o array
    for(int i=0; i<unidadesEmMemoria; i++) {
        freadCliente(arquivo, &(clientes[i]));
    }

    while (eoflag) {
        //  Geração do nome da partição, arquivo e incremento do número diferenciador da partição
        sprintf(nomeParticao, "cliente_particao_%d.dat", numeroParticao);
        FILE* particao = fopen(nomeParticao, "wb");
        numeroParticao++;

        //  Enquanto o registro não encher, o algoritmo é executado em loop
        while (1) {
            idMinValor = valorMinimo(clientes, unidadesEmMemoria);

            //  Verifica se o menor código de cliente em memória é maior que o último salvo na partição de saída, se sim, \
                salva o cliente partição, atualiza o último valor salvo e apaga o cliente da memória
            if(clientes[idMinValor].codCliente > ultimValor) {
                fprintCliente(particao, &(clientes[idMinValor]));
                ultimValor = clientes[idMinValor].codCliente;
                apagarCliente(&(clientes[idMinValor]));
            
            //  Se não, salva o cliente no reservatório, aumenta a contagem de posição do último item no reservatório \
                e apaga o cliente da memória
            } else {
                fprintCliente(reservatorio, &(clientes[idMinValor]));
                posReservatorio++;
                apagarCliente(&(clientes[idMinValor]));
                if(posReservatorio + 1 >= unidadesEmMemoria) {
                    break;
                }
            }
            //  Após encontrar o índice com menor código de cliente e salvá-lo na partição de saída, é lida a próxima entrada e colocada na posição deste índice no array \
                Se não conseguir ler, verifica se chegou ao EOF, ou se houve erro de leitura
            if(!fread((clientes+idMinValor), tamanhoStruct, 1, arquivo)) {
                if(feof(arquivo)){
                    eoflag--;
                    break;
                }else{
                    perror("Erro na leitura do arquivo de entrada");
                    exit(EXIT_FAILURE);
                }
            }
        }
        //  Salva os clientes restantes na memória em ordem na partição de saída
        for(int i=0; i < unidadesEmMemoria-1; i++) {
            idMinValor = valorMinimo(clientes, unidadesEmMemoria);
            fprintCliente(particao, &(clientes[idMinValor]));
            apagarCliente(&(clientes[idMinValor]));
        }

        //  Lê dados do reservatório e armazena na memória
        for(int i=0; i<unidadesEmMemoria; i++) {
            freadCliente(reservatorio, &(clientes[i]));
        }

        //  Reseta a posição do reservatório, último valor e fecha a partição atual
        posReservatorio = ultimValor = -1;
        fclose(particao);
    }
    fclose(reservatorio);
}

//  Encontra o menor código de cliente no array alocado na memória e retorna o índice do cliente com menor código no array
int valorMinimo (Cliente* clientes, int unidadesEmMemoria) {
    int indice = -1, valor = HIGH_VALUE;

    for(int i=0; i < unidadesEmMemoria; i++) {
        if(clientes[i].codCliente < valor) {
            indice = i;
            valor = clientes[i].codCliente;
        }
    }
    return indice;
}

//  Apaga os dados do cliente para removê-lo dos possíveis resultados da busca de valor mínimo
void apagarCliente (Cliente* cliente) {
    cliente->codCliente = HIGH_VALUE;
    memset(cliente->nome, '\0', sizeof(cliente->nome));
    memset(cliente->dataDeNascimento, '\0', sizeof(cliente->dataDeNascimento));
}

//  Grava os dados de um determinado cliente para um determinado arquivo
void fprintCliente(FILE* arquivo, Cliente* cliente) {
    fwrite(&(cliente->codCliente), sizeof(cliente->codCliente), 1, arquivo);
    fwrite(cliente->nome, sizeof(cliente->nome), 1, arquivo);
    fwrite(cliente->dataDeNascimento, sizeof(cliente->dataDeNascimento), 1, arquivo);
}

//  Lê os dados de um determinado arquivo para um determinado cliente
void freadCliente(FILE* arquivo, Cliente* destino){
    fread(&(destino->codCliente), sizeof(destino->codCliente), 1, arquivo);
    fread(destino->nome, sizeof(destino->nome), 1, arquivo);
    fread(destino->dataDeNascimento, sizeof(destino->dataDeNascimento), 1, arquivo);
}
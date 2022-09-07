//  Programa para particionamento de arquivos usando o algoritmo de seleção natural

#include "cliente.h"
#include <math.h>
#include <string.h>

#define TAM_NOME_PARTICAO 30
#define MAX_INT 2147483647

void particionar (FILE* arquivo, int tamanhoMemoria);
int valorMinimo (Cliente* clientes, int unidadesEmMemoria);

//  tamanho de memória é dado em bytes, representa o espaço disponível na memória principal
void particionar (FILE* arquivo, int tamanhoMemoria) {
    int numeroParticao = 0, posReservatorio = -1, idMinValor = -1, ultimValor = -1, tamanhoStruct = 0, eoflag = 1, unidadesEmMemoria = 0;
    char nomeParticao[TAM_NOME_PARTICAO] = {'\0'};
    Cliente* clientes = NULL;

    FILE* reservatorio = fopen("reservatorio.dat", "w+b");
    rewind(arquivo);

    //  É calculado quantas unidades da struct cabem na memória principal e \
        alocado um array com o máximo de posições que cabem
    tamanhoStruct = sizeof(Cliente);
    unidadesEmMemoria = (int) floor((double)(tamanhoMemoria/tamanhoStruct));
    clientes = (Cliente*) malloc(sizeof(Cliente)*unidadesEmMemoria);

    //  Leitura inicial do arquivo para preencher o array
    for(int i=0; i<unidadesEmMemoria; i++) {
        freadCliente(arquivo, &(clientes[i]));
    }

    while (eoflag) {
        //  Geração do nome da partição, arquivo e incremento do número diferenciador da partição
        sprintf(nomeParticao, "clientes_particao_%d.dat", numeroParticao);
        FILE* particao = fopen(nomeParticao, "wb");
        numeroParticao++;

        //  Enquanto o registro não encher, o algoritmo é executado
        while (1) {
            idMinValor = valorMinimo(clientes, unidadesEmMemoria);

            //  Verifica se o menor código de cliente em memória é maior que o último salvo na partição de saída, se sim, \
                salva o cliente partição, atualiza o último valor salvo e limpa o cliente da memória
            if(clientes[idMinValor].codCliente > ultimValor) {
                fprintCliente(particao, &(clientes[idMinValor]));
                ultimValor = clientes[idMinValor].codCliente;
                
            //  Se não, salva o cliente no reservatório, aumenta a contagem de posições ocupadas do reservatório \
                e limpa o cliente da memória
            } else {
                fprintCliente(reservatorio, &(clientes[idMinValor]));
                posReservatorio++;
                if(posReservatorio + 1 == unidadesEmMemoria) {
                    break;
                }
            }
            //  Após encontrar o índice com menor código de cliente e salvá-lo na partição de saída, é lida a próxima entrada \
                e colocada na posição deste índice no array. Se não conseguir ler, verifica se chegou ao EOF, ou se houve erro de leitura
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
        //  Salva os clientes restantes na memória na partição de saída em ordem crescente
        for(int i=0; i<unidadesEmMemoria-1; i++) {
            idMinValor = valorMinimo(clientes, unidadesEmMemoria);
            fprintCliente(particao, &(clientes[idMinValor]));
        }

        //  Reseta o ponteiro do reservatório, lê os dados e os armazena na memória
        rewind(reservatorio);
        for(int i=0; i<unidadesEmMemoria; i++) {
            freadCliente(reservatorio, &(clientes[i]));
        }

        //  Fecha e reabre o reservatório em modo "w+b" para apagar seu conteúdo
        freopen("reservatorio.dat", "w+b", reservatorio);
        //  Reseta a posição do reservatório, último valor e fecha a partição atual
        posReservatorio = -1;
        ultimValor = -(MAX_INT);
        fclose(particao);
    }
    fclose(reservatorio);
}

//  Encontra o menor código de cliente no array alocado na memória e retorna o índice do cliente com menor código no array
int valorMinimo (Cliente* clientes, int unidadesEmMemoria) {
    int indice = -1, valor = MAX_INT;

    for(int i=0; i < unidadesEmMemoria; i++) {
        if(clientes[i].codCliente <= valor) {
            indice = i;
            valor = clientes[i].codCliente;
        }
    }
    return indice;
}
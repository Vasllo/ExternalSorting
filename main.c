#include "partition.h"

void criarExemplo();

int main() {
    FILE* arquivo = fopen("clientes.dat", "w+b");
    criarExemplo(arquivo);



    particionar(arquivo, 102);

    return 0;
}

void criarExemplo(FILE* arquivo) {
    Cliente cliente;
    cliente.codCliente = 0;
    strncpy(cliente.nome, "abababababababababababababababababab", 37); strncpy(cliente.dataDeNascimento, "01/01/1900", 10);

    for(int i=10; i > 0; i--) {
        cliente.codCliente = i;
        fprintCliente(arquivo, &cliente);
    }
}
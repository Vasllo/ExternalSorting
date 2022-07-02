#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <getopt.h>

#define NOME_DO_ARQUIVO 20
#define TAMANHO 500
#define TAMANHO_DATA 11
#define TAMANHO_NOME 100

typedef struct Cliente
{
    int codCliente;
    char nome [TAMANHO_NOME];
    char dataDeNascimento[TAMANHO_DATA];
}Cliente;

typedef struct dadosDeParticao
{
    Cliente **memoria;
    Cliente **reservatorio;
    int indiceMemoria;
    int indiceReservatorio;
    int tamanhoDeMemoria;
}infoParticao;

enum saidaComparar
{
    MENORQUE = -1,
    MAIORQUE = 0,
    IGUAL = 1
};

// Desaloca ponteiro cliente
void liberarCliente(Cliente **cliente)
{
    free(*cliente);
    *cliente = NULL;
}

void imprimirCliente(Cliente *cliente)
{
    printf("Dados do cliente:\nCódigo: %d\n Nome: %s\n Data de nascimento: %s\n" , cliente->codCliente, cliente->nome, cliente->dataDeNascimento);
}

// Retorno 1 sinaliza sucesso na escrita e 0 falha na escrita
int escreverCliente(Cliente *cliente, FILE *arquivo)
{
	int sucesso = 0;
	if (cliente && fwrite(cliente, sizeof(Cliente), 1, arquivo))
		sucesso++;
    return sucesso;
}

Cliente *lerCliente(FILE *arquivo)
{
    Cliente* cliente = (Cliente*) malloc(sizeof(Cliente));
    if (fread(cliente, sizeof(Cliente), 1, arquivo))
        return cliente;
    free(cliente);
    return NULL;
}

infoParticao *criarParticao(int tamanhoDeMemoria)
{
    infoParticao *p = (infoParticao *)malloc(sizeof(infoParticao));

    p->memoria = (Cliente **)malloc(sizeof(Cliente *) * tamanhoDeMemoria);
    p->reservatorio = (Cliente **)malloc(sizeof(Cliente *) * tamanhoDeMemoria);
    p->indiceReservatorio = 0;
    p->tamanhoDeMemoria = tamanhoDeMemoria;

    return p;
}

void liberarParticao(infoParticao **part)
{
    infoParticao *aux = *part;
    for (int i = 0; i < aux->tamanhoDeMemoria; i++)
    {
        if (aux->memoria[i] != NULL)
            liberarCliente(&(aux->memoria[i]));
    }
    free(aux->memoria);
    free(aux->reservatorio);
    free(aux);

    *part = NULL;
}

int comparar(Cliente *cliente1, Cliente *cliente2)
{
    if(cliente1->codCliente < cliente2->codCliente)
		return MENORQUE;
	else if(cliente1->codCliente == cliente2->codCliente)
		return IGUAL;
	else
		return MAIORQUE;
}

int particionar(FILE *arquivo, char *prefix, int tamanhoDeMemoria)
{
	infoParticao *part = criarParticao(tamanhoDeMemoria);
	int geracao_de_particao = 0;

	//ARQUIVO EXISTE?
	if (arquivo != NULL)
	{
		char stringAux[NOME_DO_ARQUIVO];
		Cliente *aux;
		FILE *fPart;

		// copia os primeiros para a memória
		for (int i = 0; i < tamanhoDeMemoria; i++)
			part->memoria[i] = lerCliente(arquivo);
		while (part->memoria[0] != NULL)
		{
			// string para o nome do arquivo
			sprintf(stringAux, "parts/%s%d.dat", prefix, geracao_de_particao);
			fPart = fopen(stringAux, "wb");
			while (part->indiceReservatorio < tamanhoDeMemoria)
			{
				// procurar o primeiro elemento
				part->indiceMemoria = -1;
				for (int i = 0; i < tamanhoDeMemoria; i++)
					if (part->memoria[i] != NULL)
					{
						part->indiceMemoria = i;
						break;
					}
					
				// memoria vazia?
				
				if (part->indiceMemoria != -1)
				{
					// menor elemnto na memoria
					for (int i = 0; i < tamanhoDeMemoria; i++)
						if (part->memoria[i] != NULL && comparar(part->memoria[i], part->memoria[part->indiceMemoria]) == MENORQUE)
							part->indiceMemoria = i;

					// elemento escrito
					escreverCliente(part->memoria[part->indiceMemoria], fPart);
					// vai para o proximo
					while (part->indiceReservatorio < tamanhoDeMemoria)
					{
						aux = lerCliente(arquivo);
						if (aux != NULL)
						{
							// menor que o ultimo, vai para o reservatório
							if (comparar(aux, part->memoria[part->indiceMemoria]) == MENORQUE)
							{
								part->reservatorio[part->indiceReservatorio] = aux;
								part->indiceReservatorio++;
							}
							// maior que ultimo substitui
							else
							{
								liberarCliente(&part->memoria[part->indiceMemoria]);
								part->memoria[part->indiceMemoria] = aux;
								break;
							}
						}
						// se ultimo elemento for NULL, acabamos de ler o arquivo
						else
						{
							liberarCliente(&part->memoria[part->indiceMemoria]);
							break;
						}
					}
				}
				// não tendo elemento na memoria sai do loop
				else
					break;
			}
			// reservatorio cheio , remove ultimo elemento
			if (part->indiceReservatorio == tamanhoDeMemoria && part->memoria[part->indiceMemoria] != NULL)
				liberarCliente(&part->memoria[part->indiceMemoria]);

			// escreve elementos restante na run
			do
			{
				// achar o primeiro elemento na memoria
				part->indiceMemoria = -1;
				for (int i = 0; i < tamanhoDeMemoria; i++)
					if (part->memoria[i] != NULL)
					{
						part->indiceMemoria = i;
						break;
					}
				// achar o menor elemento na memoria
				if (part->indiceMemoria != -1)
				{
					for (int i = 0; i < tamanhoDeMemoria; i++)
						if (part->memoria[i] != NULL && comparar(part->memoria[i], part->memoria[part->indiceMemoria]) == MENORQUE)
							part->indiceMemoria = i;
					// ecrever e liberar o menor
					escreverCliente(part->memoria[part->indiceMemoria], fPart);
					liberarCliente(&part->memoria[part->indiceMemoria]);
				}
			} while (part->indiceMemoria != -1);

			// fechar o arquivo e copia os dados do reservatorio para a memoria
			fclose(fPart);
			for (int i = 0; i < part->indiceReservatorio; i++)
				part->memoria[i] = part->reservatorio[i];
			part->indiceReservatorio = 0;
			geracao_de_particao++;
		}
	}

	liberarParticao(&part);

	return geracao_de_particao;
}
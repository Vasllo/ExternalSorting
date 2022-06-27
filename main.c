#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <getopt.h>
#define NOME_DO_ARQUIVO 20
#define TAMANHO 500

typedef struct cliente{

    int CodCliente;
    char Nome [25];


}CLIENTE;

void liberarCliente(CLIENTE **fc)

{
    free(*fc);
    *fc = NULL;
}


void imprimirCliente(CLIENTE *fc)
{
    printf("codigo do cliente: %d\n" "nome do cliente: %s\n" , fc->CodCliente, fc->Nome);
}

int escreverCliente(CLIENTE *fc, FILE *f)
{
    if (fc != NULL)
        if (fwrite(fc, sizeof(CLIENTE), 1, f) > 0)
            return 1;
    return 0;
}

CLIENTE *lerCliente(FILE *f)
{
    CLIENTE *fc = (CLIENTE)malloc(sizeof(CLIENTE)); // aloca tamanho da struct CLIENTE
    if (fread(fc, sizeof(CLIENTE), 1, f) > 0)
        return fc;
    free(fc);
    return NULL;
}

typedef struct stParticionador
{
    CLIENTE memoria;
    CLIENTE reservatorio;
    int r;
    int indReserv;
    int memSize;

}CLIENTEPARTICIONADOR;

CLIENTEPARTICIONADOR *create(int memSize)
{
    CLIENTEPARTICIONADOR *p = (CLIENTEPARTICIONADOR *)malloc(sizeof(CLIENTEPARTICIONADOR));

    p->memoria = (CLIENTE **)malloc(sizeof(CLIENTE *) * memSize);
    p->reservatorio = (CLIENTE **)malloc(sizeof(CLIENTE *) * memSize);
    p->indReserv = 0;
    p->memSize = memSize;

    return p;
}

void freeparticionador(CLIENTEPARTICIONADOR **part)
{
    CLIENTEPARTICIONADOR *p = *part;

    for (int i = 0; i < p->memSize; i++)
    {
        if (p->memoria[i] != NULL)
            liberarCliente(&p->memoria[i]);
    }
    free(p->memoria);
    free(p->reservatorio);
    free(p);

    (*part) = NULL;
}

enum saidaComparar
{
    MENORQUE,
    MAIORQUE,
    IGUAL
};

int particionar(FILE *f, char *prefix, int memSize)
{
	CLIENTEPARTICIONADOR *part = create(memSize);
	int geracao_de_particao = 0;

	//ARQUIVO EXISTE?
	if (f != NULL)
	{
		char stringAux[NOME_DO_ARQUIVO];
		CLIENTE *aux;
		FILE *fPart;

		// copia os primeiros para a memória
		for (int i = 0; i < memSize; i++)
			part->memoria[i] = lerCliente(f);
		while (part->memoria[0] != NULL)
		{
			// string para o nome do arquivo
			sprintf(stringAux, "parts/%s%d.dat", prefix, geracao_de_particao);
			fPart = fopen(stringAux, "wb");
			while (part->indReserv < memSize)
			{
				// procurar o primeiro elemento
				part->r = -1;
				for (int i = 0; i < memSize; i++)
					if (part->memoria[i] != NULL)
					{
						part->r = i;
						break;
					}
					
				// memoria vazia?
				
				if (part->r != -1)
				{
					// menor elemnto na memoria
					for (int i = 0; i < memSize; i++)
						if (part->memoria[i] != NULL && comparar(part->memoria[i]->CodCliente, part->memoria[part->r]->CodCliente) == MENORQUE)
							part->r = i;

					// elemento escrito
					escreverFilme(part->memoria[part->r], fPart);
					// vai para o proximo
					while (part->indReserv < memSize)
					{
						aux = lerCliente(f);
						if (aux != NULL)
						{
							// menor que o ultimo, vai para o reservatório
							if (comparar(aux->CodCliente, part->memoria[part->r]->CodCliente) == MENORQUE)
							{
								part->reservatorio[part->indReserv] = aux;
								part->indReserv++;
							}
							// maior que ultimo substitui
							else
							{
								liberarCliente(&part->memoria[part->r]);
								part->memoria[part->r] = aux;
								break;
							}
						}
						// se ultimo elemento for NULL, acabamos de ler o arquivo
						else
						{
							liberarCliente(&part->memoria[part->r]);
							break;
						}
					}
				}
				// não tendo elemento na memoria sai do loop
				else
					break;
			}
			// reservatorio cheio , remove ultimo elemento
			if (part->indReserv == memSize && part->memoria[part->r] != NULL)
				liberarCliente(&part->memoria[part->r]);

			// escreve elementos restante na run
			do
			{
				// achar o primeiro elemento na memoria
				part->r = -1;
				for (int i = 0; i < memSize; i++)
					if (part->memoria[i] != NULL)
					{
						part->r = i;
						break;
					}
				// achar o menor elemento na memoria
				if (part->r != -1)
				{
					for (int i = 0; i < memSize; i++)
						if (part->memoria[i] != NULL && comparar(part->memoria[i]->CodCliente, part->memoria[part->r]->CodCliente) == MENORQUE)
							part->r = i;
					// ecrever e liberar o menor
					escreverCliente(part->memoria[part->r], fPart);
					liberarCliente(&part->memoria[part->r]);
				}
			} while (part->r != -1);

			// fechar o arquivo e copia os dados do reservatorio para a memoria
			fclose(fPart);
			for (int i = 0; i < part->indReserv; i++)
				part->memoria[i] = part->reservatorio[i];
			part->indReserv = 0;
			geracao_de_particao++;
		}
	}

	freeparticionador(&part);

	return geracao_de_particao;
}

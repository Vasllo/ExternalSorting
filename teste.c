#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIBERADO 0
#define OCUPADO 1
#define m 7

typedef struct cliente {

	int codcliente;
	char nome[100];
	int prox;
	int ocupado;

} TC;

int Hash (int cod, int m)
{
	return cod % m;
}

TC *aloca(char *arq,int cod, char nome, int status, int prox)
{

	TC *novo = (TC *) malloc(sizeof(TC));
	novo->codcliente = cod;
	novo->nome = nome;
	novo->ocupado = status;
	novo->prox = prox;
	return novo;

}
void inicializa(char *arq, int m)
{

	FILE *fp = fopen(arq, "wb+");
	if(!fp) exit(1);
	int i, elem = -1;
	for (i = 0; i < m; i++) 
	{
		fwrite(&elem, sizeof(int), 1, fp);
	}
	fclose(fp);
}

TC* busca(char *arq, int m, int cod, int *achou)
{

	FILE *fp = fopen(arq, "rb");
	if(!fp) exit(1);
	int pos, h = Hash(cod, m);
	int div = m * sizeof(int);

	fseek(fp, h * sizeof(int), SEEK_SET);
	fread(&pos, sizeof(int), 1,fp);

	if(pos == -1)
	{
		return NULL;
	}

	fseek(fp, pos, SEEK_SET);
	TC *cliente;
	fread(&cliente, sizeof(TC*), 1, fp);


	while (1)
	{
		
		if (cliente->codcliente == cod && cliente->ocupado)
		{


			TC *q = aloca(cliente.cod, cliente.nome);
			q -> prox = cliente.prox;
			fclose(fp);
			return q;

		} 

		else

		{

		if (aluno->prox == -1)
		{

			fclose(fp);
			return NULL;
		}


		fseek(fp, cliente.prox, SEEK_SET);
		fread(&cliente, sizeof(TC), 1, fp);
	}
	
}

void insere (char *arq, int m, int cod, char nome)
{
	FILE *fp = fopen(arq, "rb+");
	if(!fp) exit(1);
	int pos, h = Hash(cod, m), ant = -1, first_pos_free = -1, div = m * sizeof(int);


	fseek(fp, h * sizeof(int), SEEK_SET);
	fread(&pos, sizeof(int), 1,fp);
	TC* cliente;

	int temp = -1;
	int end = busca(tab, m, cod, &achou);
	if(!achou)
	{
		if(end != -1)
		{
			pos = end;
		}
		else
		{
			int i = 1;
			pos = hash(cod, m);
			while(i <= m)
			{
				if(tab[pos] -> ocupado)
				{
					pos = (pos + 1) % m;
					i++;
				}
				else
				{
					i = m + 2;
				}
			}
			if(i == m + 1)
			{
				printf("tabela cheia, ocorreu overflow\n");
				return;
			}
			temp = tab[hash(cod, m)] -> prox;
			tab[hash(cod, m)] -> prox = pos;
			tab[pos]-> prox =temp;
		}
		tab[pos]-> ocupado = OCUPADO;
		tab[pos]-> codliente = cod;
		tab[pos]-> nome = nome; 
	}
	else
	{
		printf("codigo existe, inserção invalida\n");
	}
}


char* exclui(char *arq, int m, int cod)
{
	FILE *fp = fopen(arq, "rb+");
	if (!fp) exit(1);

	int pos, h = Hash(cod, m);
	fseek(fp, h * sizeof(int), SEEK_SET);
	fread(&pos, sizeof(int), 1, fp); // guarda em pos o endereço onde o registro deve estar

	if (pos == -1) return 1; // o registro não está na tabela

	char nome;
	TC* aux;

	while (1) {
		fseek(fp, pos, SEEK_SET);
		fread(&aux, sizeof(TA), 1, fp);

		if (aux.cod == mat && aux.status) { // se o registro existe e seu status é 1
			nome = aux.nome;
			aux.status = 0; // altera status pois o registro será retirado

			fseek(fp, pos, SEEK_SET);
			fwrite(&aux, sizeof(TA), 1, fp); // escreve registro atualizado no mesmo local

			fclose(fp);
			return cr;
		}

		if (aux.prox == -1) { // não há mais registros na lista encadeada
			fclose(fp);
			return -1;
		}

		pos = aux.prox;
	}
}

void imprime(char *arq, int m) {
	FILE *fp = fopen(arq, "rb");
	if (!fp) exit(1);

	fseek(fp, 0L, SEEK_END);
	int fim = ftell(fp);
	rewind(fp);

	int indice[m], i, pos, p, divisao = m * sizeof(int);
	fread(&indice, sizeof(int), m, fp);

	printf("ÍNDICE:\n");

	for (i = 0; i < m; i++) {
		printf("[%d] ", i);
		if (indice[i] != -1) printf("$%03d\n", indice[i]);
		else printf("-\n");
	}

	printf("REGISTROS:\n");

	fseek(fp, divisao, SEEK_SET);
	TC* aux;
	int r = ftell(fp);

	while (r != fim) {
		fread(&aux, sizeof(TC), 1, fp);
		printf("[$%03d] mat: %d | cr: %s | status: %d | prox: ",
			   r, aux.cod, aux.nome, aux.status);

		if (aux.prox != -1) printf("$%03d\n", aux.prox);
		else printf("-\n");

		r = ftell(fp);
	}

	fclose(fp);
}

void limpa_cadeia(char *arq, int m, int cadeia) {
	if (cadeia > m) return;

	FILE *fp = fopen(arq, "rb+");
	if (!fp) exit(1);
	int h = cadeia * sizeof(int), pos, prox;
	fseek(fp, h, SEEK_SET);
	fread(&pos, sizeof(int), 1, fp);
	printf("\ndebug: pos = %d\n", pos);
	TC* aux;

	fseek(fp, pos, SEEK_SET);
	fread(&aux, sizeof(TC), 1, fp);

	while (1) {
		if (aux.status) {
			aux.status = 0; // altera status de 1 para 0
			fseek(fp, pos, SEEK_SET);
			fwrite(&aux, sizeof(TC), 1, fp); // escreve aux atualizado no mesmo local
		}

		prox = aux.prox;

		if (prox != -1) { // se houver próximo
			pos = prox;
			fseek(fp, prox, SEEK_SET); // vai para local do próximo
			fread(&aux, sizeof(TC), 1, fp); // guarda em aux
		}

		else break;
	}

	fclose(fp);
}

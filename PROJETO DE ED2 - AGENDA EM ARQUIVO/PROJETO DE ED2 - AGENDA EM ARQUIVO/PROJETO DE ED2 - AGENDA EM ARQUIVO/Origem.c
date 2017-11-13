#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Typedef.h"
#include "ArquivoNomesSobrenomes.h"
#include "ArquivoDados.h"


FILE *criarArquivo(char nomeArq[]);
void preencherArquivoNoDescritor(FILE *arq);
FILE *criarArquivoNoDescritor(char nomeArq[]);
void inicializarQtdNomeSobrenome(FILE *arq, int *qtd);
void inicializarQtdDados(FILE *arq, int *qtd);
void manutencao(FILE *arqDados, char nome[]);

int main() {
	FILE *arqNomes;
	FILE *arqSobrenomes;
	FILE *arqDados;
	FILE *arqNoDescritorNomes;
	FILE *arqNoDescritorSobrenomes;

	int op_menu;
	int qtdArqNomes;
	int qtdArqSobrenomes;
	int qtdArqDados;
	int verificarRotacao;
	int primeiraVezFuncao;
	int posicao;
	int noInserido;
	char nome[20];
	char sobrenome[20];
	int posArqDados;
	
	noInserido = 1;
	qtdArqNomes = 0;
	qtdArqSobrenomes = 0;
	qtdArqDados = 0;
	primeiraVezFuncao = 1;
	verificarRotacao = 0;
	posicao = -1;


	arqNoDescritorNomes = criarArquivoNoDescritor("arquivoNoDescritorNome.dat");
	arqNoDescritorSobrenomes = criarArquivoNoDescritor("arquivoNoDescritorSobrenome.dat");
	arqNomes = criarArquivo("arquivoNomes.dat");
	arqSobrenomes = criarArquivo("arquivoSobrenomes.dat");
	arqDados = criarArquivo("arquivoDados.dat");

	inicializarQtdNomeSobrenome(arqNomes, &qtdArqNomes);
	inicializarQtdNomeSobrenome(arqSobrenomes, &qtdArqSobrenomes);
	inicializarQtdDados(arqDados, &qtdArqDados);

	do {
		printf("1 - Adicionar.\n2 - Remover.\n3 - Buscar por nome.\n4 - Buscar por sobrenome.\n5 - Sair.\nDigite a opcao: ");
		scanf("%d", &op_menu);
		fflush(stdin);
		switch (op_menu) {
		case 1:
			printf("Digite o nome: ");
			scanf("%s", &nome);
			fflush(stdin);
			printf("Digite o sobrenome: ");
			scanf("%s", &sobrenome);
			fflush(stdin);
			if (buscarArqNomesSobrenomes(arqNomes, arqNoDescritorNomes, nome, qtdArqNomes, 1) >= 0 || buscarArqNomesSobrenomes(arqSobrenomes, arqNoDescritorSobrenomes, sobrenome, qtdArqSobrenomes, 1) >= 0) {
				printf("Ja existe.\n");
			}
			else {
				posArqDados = adicionar(arqDados, nome, sobrenome, &qtdArqDados);
				if (posArqDados == -1) {
					printf("ERRO.\n");

				}
				else {
					adicionarArqNomesSobrenomes(arqNomes, arqNoDescritorNomes, nome, &qtdArqNomes, &primeiraVezFuncao, &verificarRotacao, &posicao, &noInserido, posArqDados);
					noInserido = 1;
					primeiraVezFuncao = 1;
					adicionarArqNomesSobrenomes(arqSobrenomes, arqNoDescritorSobrenomes, sobrenome, &qtdArqSobrenomes, &primeiraVezFuncao, &verificarRotacao, &posicao, &noInserido, posArqDados);
					noInserido = 1;
					primeiraVezFuncao = 1;
				}
			}
			break;
		case 2:
			printf("Digite o nome: ");
			scanf("%s", &nome);
			fflush(stdin);
			printf("Digite o sobrenome: ");
			scanf("%s", &sobrenome);
			fflush(stdin);
			posArqDados = removerArqNomesSobrenomes(arqNomes, arqSobrenomes, arqNoDescritorNomes, arqNoDescritorSobrenomes, nome, sobrenome, qtdArqNomes, qtdArqSobrenomes);
			if (posArqDados == -1) {
				printf("ERRO.\n");
			}
			else if(posArqDados == -2) {
				printf("Contato nao encontrado.\n");
			}
			else {
				remover(arqDados, posArqDados);
			}
			break;
		case 3:
			printf("Digite o nome: ");
			scanf("%s", &nome);
			fflush(stdin);
			posArqDados = buscarArqNomesSobrenomes(arqNomes, arqNoDescritorNomes, nome, qtdArqNomes, 0);
			if (posArqDados == -1) {
				printf("ERRO.\n");
			}
			else if(posArqDados == -2) {
				printf("Contato nao encontrado.\n");
			}
			else {
				buscar(arqDados, posArqDados);
			}
			break;
		case 4: 
			printf("Digite o sobrenome: ");
			scanf("%s", &sobrenome);
			fflush(stdin);
			posArqDados = buscarArqNomesSobrenomes(arqSobrenomes, arqNoDescritorSobrenomes, sobrenome, qtdArqSobrenomes, 0);
			if (posArqDados == -1) {
				printf("ERRO.\n");
			}
			else if(posArqDados == -2) {
				printf("Contato nao encontrado.\n");
			}
			else {
				buscar(arqDados, posArqDados);
			}
			break;
		case 5: manutencao(arqDados, "arquivoDados.dat");
			fclose(arqNoDescritorNomes);
			fclose(arqNoDescritorSobrenomes);
			fclose(arqNomes);
			fclose(arqSobrenomes);
			break;
		default:
			printf("Opcao invalida.\n");
			break;
		}
	} while (op_menu != 5);
	return 0;
}

void manutencao(FILE *arqDados, char nome[]) {
	FILE *arqAux;
	arqDadosStruct aux1;
	int status;
	arqAux = fopen("arquivoAuxiliar", "w+b");
	fseek(arqDados, 0, 0);
	while (1) {
		status = fread(&aux1, sizeof(arqDadosStruct), 1, arqDados);
		if (status != 1) {
			if (!feof(arqDados)) {
				printf("ERRO DE MENUTENCAO.\n");
				return;
			}
			break;
		}
		else {
			if (aux1.status == 1) {
				status = fwrite(&aux1, sizeof(arqDadosStruct), 1, arqAux);
				if (status != 1) {
					printf("ERRO DE MANUTENCAO.\n");
					return;
				}
			}
		}
		
	}
	fclose(arqDados);
	fclose(arqAux);
	remove(nome);
	rename("arquivoAuxiliar", nome);


}

void inicializarQtdDados(FILE *arq, int *qtd){
	arqDadosStruct structArq;
	int status;
	fseek(arq, 0, 0);
	while (1) {
		status = fread(&structArq, sizeof(arqDadosStruct), 1, arq);
		if (status != 1) {
			if (!feof(arq)) {
				printf("ERRO\n");
			}
			break;
		}
		else {
			*qtd = *qtd + 1;
		}
	}
}

void inicializarQtdNomeSobrenome(FILE *arq, int *qtd) {
	arqIndice structArq;
	int status;
	fseek(arq, 0, 0);
	while (1) {
		status = fread(&structArq, sizeof(arqIndice), 1, arq);
		if (status != 1) {
			if (!feof(arq)) {
				printf("ERRO\n");
			}
			break;
		}
		else {
			*qtd = *qtd + 1;
		}
	}
}


void preencherArquivoNoDescritor(FILE *arq) {
	arqIndiceInicio noDescr;
	int status;
	noDescr.comecoArqNomeSobrenomeStatusApagado = -1;
	noDescr.comecoArqNomeSobrenomeStatusOk = -1;
	fseek(arq, 0, 0);
	status = fwrite(&noDescr, sizeof(arqIndiceInicio), 1, arq);
	if (status != 1) {
		printf("ERRO.\n");
		return;
	}
}

FILE *criarArquivoNoDescritor(char nomeArq[]) {
	FILE *arq;
	arq = fopen(nomeArq, "r+b");
	if (arq == NULL) {
		arq = fopen(nomeArq, "w+b");
		preencherArquivoNoDescritor(arq);
	}
	return arq;
}

FILE *criarArquivo(char nomeArq[]) {
	FILE *arq;
	arq = fopen(nomeArq, "r+b");
	if (arq == NULL) {
		arq = fopen(nomeArq, "w+b");
	}
	return arq;
}
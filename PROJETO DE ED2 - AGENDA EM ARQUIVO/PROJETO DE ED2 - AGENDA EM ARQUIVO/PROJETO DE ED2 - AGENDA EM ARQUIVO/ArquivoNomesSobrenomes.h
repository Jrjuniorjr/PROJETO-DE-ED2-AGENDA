#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Typedef.h"


#ifndef ArquivoNomes
#define ArquivoNomes

void adicionarArqNomesSobrenomes(FILE *arqNomesSobrenomes, FILE *arqNoDescritor, char nome[], int *qtd, int *primeiraVezFuncao, int *verificarRotacao, int *posicao, int *noInserido, int posArqDados);
int removerArqNomesSobrenomes(FILE *arqNomes, FILE *arqSobrenomes, FILE *arqNoDescritorNome, FILE *arqNoDescritorSobrenome, char nome[], char sobrenome[], int qtdArqNome, int qtdArqSobrenome);
int buscarArqNomesSobrenomes(FILE *arqNomes, FILE *arqNoDescritor, char nome[], int qtd, int aux1);
void rotacionarArqNomesDir(FILE *arq, FILE *arqNoDescritor, int posicao, int *verificarRotacao);
void rotacionarArqNomesEsq(FILE *arq, FILE *arqNoDescritor, int posicao, int *verificarRotacao);

#endif
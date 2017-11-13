#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Typedef.h"


#ifndef ArquivoDados
#define ArquivoDados

int adicionar(FILE *arqDados, char nome[], char sobrenome[], int *qtdDados);
void remover(FILE *arqDados, int posArqDados);
void buscar(FILE *arqDados, int pos);


#endif
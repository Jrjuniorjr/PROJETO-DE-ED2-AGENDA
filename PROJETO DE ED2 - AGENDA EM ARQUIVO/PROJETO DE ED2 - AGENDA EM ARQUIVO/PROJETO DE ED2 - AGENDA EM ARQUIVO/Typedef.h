#pragma once

#ifndef TAM
#define TAM 20
#endif

#ifndef TYPEDEF
#define TYPEDEF

typedef struct ArqIndice {
	char string[TAM];
	int esq;
	int dir;
	int posArq;
	int posArqDados;
	int fatorBalanceamento;
	int status;
}arqIndice;

typedef struct ArqIndiceInicio {
	int comecoArqNomeSobrenomeStatusOk;
	int comecoArqNomeSobrenomeStatusApagado;

}arqIndiceInicio;

typedef struct ArqDados {
	char nome[TAM];
	char sobrenome[TAM];
	char telefone[TAM];
	char email[TAM];
	int posArq;
	int status;

}arqDadosStruct;



#endif
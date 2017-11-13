#include "ArquivoDados.h"

int adicionar(FILE *arqDados, char nome[], char sobrenome[], int *qtdDados) {
	arqDadosStruct aux;
	char telefone[20];
	char email[20];
	int status;
	int retorno;
	printf("Digite o telefone: ");
	scanf("%s", &telefone);
	fflush(stdin);
	printf("Digite email: ");
	scanf("%s", &email);
	fflush(stdin);
	strcpy(aux.nome, nome);
	strcpy(aux.sobrenome, sobrenome);
	strcpy(aux.telefone, telefone);
	strcpy(aux.email, email);
	aux.posArq = *qtdDados;
	retorno = aux.posArq;
	aux.status = 1;
	fseek(arqDados, 0, 2);
	status = fwrite(&aux, sizeof(arqDadosStruct), 1, arqDados);
	if (status != 1) {
		return -1;
	}
	else {
		*qtdDados = *qtdDados + 1;
		return retorno;
	}

}
void remover(FILE *arqDados, int posArqDados) {
	arqDadosStruct aux;
	int status;
	fseek(arqDados, posArqDados * sizeof(arqDadosStruct), 0);
	status = fread(&aux, sizeof(arqDadosStruct), 1, arqDados);
	if (status != 1) {
		printf("ERRO.\n");
		return;
	}
	aux.status = 0;
	fseek(arqDados, posArqDados * sizeof(arqDadosStruct), 0);
	status = fwrite(&aux, sizeof(arqDadosStruct), 1, arqDados);
	if (status != 1) {
		printf("ERRO.\n");
		return;
	}
	else {
		printf("Contato removido com sucesso.\n");
	}

}

void buscar(FILE *arqDados, int pos) {
	arqDadosStruct dados;
	int status;
	fseek(arqDados, pos * sizeof(arqDadosStruct), 0);
	status = fread(&dados, sizeof(arqDadosStruct), 1, arqDados);
	if(status != 1){
		printf("ERRO.\n");
		return;
	}
	else{
		printf("Nome: %s\n", dados.nome);
		printf("Sobrenome: %s\n", dados.sobrenome);
		printf("Telefone: %s\n", dados.telefone);
		printf("Email: %s\n", dados.email);
	}


}

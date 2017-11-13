#include "ArquivoNomesSobrenomes.h"

//-2 -> FIM DE ARQUIVO
//-1 -> ERRO


void adicionarArqNomesSobrenomes(FILE *arqNomesSobrenomes, FILE *arqNoDescritor, char nome[], int *qtd, int *primeiraVezFuncao, int *verificarRotacao, int *posicao, int *noInserido, int posArqDados) {
	arqIndice nomes;
	arqIndice auxNomes;
	arqIndiceInicio aux;
	int status;
	fseek(arqNoDescritor, 0, 0);
	status = fread(&aux, sizeof(arqIndiceInicio), 1, arqNoDescritor);
	if (status != 1) {
		printf("ERRO AO LER ARQUIVO NO DESCRITOR.\n");
		return;
	}
	else {
		if (aux.comecoArqNomeSobrenomeStatusApagado != -1) {
			fseek(arqNomesSobrenomes, aux.comecoArqNomeSobrenomeStatusApagado * sizeof(arqIndice), 0);
			status = fread(&auxNomes, sizeof(arqIndice), 1, arqNomesSobrenomes);
			strcpy(nomes.string, nome);
			nomes.dir = auxNomes.dir;
			nomes.esq = auxNomes.esq;
			nomes.posArq = auxNomes.posArq;
			nomes.status = 1;
			nomes.fatorBalanceamento = auxNomes.fatorBalanceamento;
			nomes.posArqDados = posArqDados;
			aux.comecoArqNomeSobrenomeStatusApagado = -1;
			fseek(arqNoDescritor, 0, 0);
			status = fwrite(&aux, sizeof(arqIndiceInicio), 1, arqNoDescritor);
			if (status != 1) {
				printf("ERRO.\n");
				return;
			}
			fseek(arqNomesSobrenomes, aux.comecoArqNomeSobrenomeStatusApagado * sizeof(arqIndice), 0);
			status = fwrite(&nomes, sizeof(arqIndice), 1, arqNomesSobrenomes);
			if (status != 1) {
				printf("ERRO AO ESCREVER NO ARQUIVO.\n");
				return;
			}
			else {
				printf("Contato salvo com sucesso.\n");
				*qtd = *qtd + 1;
			}
		}

		else {
			if (*primeiraVezFuncao == 1) {
				*posicao = aux.comecoArqNomeSobrenomeStatusOk;
				*primeiraVezFuncao = 0;
			}
			if (*posicao == -1) {
				strcpy(nomes.string, nome);
				nomes.dir = -1;
				nomes.esq = -1;
				nomes.posArq = *qtd;
				nomes.status = 1;
				nomes.fatorBalanceamento = 0;
				nomes.posArqDados = posArqDados;
				*verificarRotacao = 1;
				if (*qtd == 0) {
					aux.comecoArqNomeSobrenomeStatusOk = nomes.posArq;
					fseek(arqNoDescritor, 0, 0);
					status = fwrite(&aux, sizeof(arqIndiceInicio), 1, arqNoDescritor);
					if (status != 1) {
						printf("ERRO AO GRAVAR ARQUIVO.\n");
						return;
					}
				}
				fseek(arqNomesSobrenomes, 0, 2);
				status = fwrite(&nomes, sizeof(arqIndice), 1, arqNomesSobrenomes);
				if (status != 1) {
					printf("ERRO AO GRAVAR ARQUIVO.\n");
					return;
				}
				else {
					*qtd = *qtd + 1;
					printf("Contato salvo com sucesso.\n");

				}
			}
			else {
				fseek(arqNomesSobrenomes, (*posicao) * sizeof(arqIndice), 0);
				status = fread(&nomes, sizeof(arqIndice), 1, arqNomesSobrenomes);
				if (status != 1) {
					if (!feof(arqNomesSobrenomes)) {
						printf("ERRO\n");
						return;
					}
					printf("FIM DE ARQUIVO.\n");
					return;
				}
				else {
					if (strcmp(nome, nomes.string) == 0) {
						printf("Contato ja existe.\n");
						return;
					}
					else if (strcmp(nome, nomes.string) < 0) {
						*posicao = nomes.esq;
						adicionarArqNomesSobrenomes(arqNomesSobrenomes, arqNoDescritor, nome, qtd, primeiraVezFuncao, verificarRotacao, posicao, noInserido, posArqDados);
						if (*noInserido == 1) {
							nomes.esq = *qtd -1;
							*noInserido = 0;
							fseek(arqNomesSobrenomes, nomes.posArq * sizeof(arqIndice), 0);
							status = fwrite(&nomes, sizeof(arqIndice), 1, arqNomesSobrenomes);
							if (status != 1) {
								printf("ERRO AO ESCREVER ARQUIVO NO DESCRITOR.\n");
								return;
							}
						}
						*posicao = nomes.posArq;
						if (*verificarRotacao == 1) {
							switch (nomes.fatorBalanceamento) {
							case -1:
								nomes.fatorBalanceamento = 0;
								*verificarRotacao = 0;
								fseek(arqNomesSobrenomes, nomes.posArq * sizeof(arqIndice), 0);
								status = fwrite(&nomes, sizeof(arqIndice), 1, arqNomesSobrenomes);
								if (status != 1) {
									printf("ERRO AO ESCREVER ARQUIVO.\n");
									return;
								}
								break;
							case 0:
								nomes.fatorBalanceamento = 1;
								fseek(arqNomesSobrenomes, nomes.posArq * sizeof(arqIndice), 0);
								status = fwrite(&nomes, sizeof(arqIndice), 1, arqNomesSobrenomes);
								if (status != 1) {
									printf("ERRO AO ESCREVER ARQUIVO.\n");
									return;
								}
								break;
							case 1:
								rotacionarArqNomesDir(arqNomesSobrenomes, arqNoDescritor, *posicao, verificarRotacao);
								break;
							}
						}
					}
					else {
						*posicao = nomes.dir;
						adicionarArqNomesSobrenomes(arqNomesSobrenomes, arqNoDescritor, nome, qtd, primeiraVezFuncao, verificarRotacao, posicao, noInserido, posArqDados);
						if (*noInserido == 1) {
							nomes.dir = *qtd -1;
							*noInserido = 0;
							fseek(arqNomesSobrenomes, nomes.posArq * sizeof(arqIndice), 0);
							status = fwrite(&nomes, sizeof(arqIndice), 1, arqNomesSobrenomes);
							if (status != 1) {
								printf("ERRO AO ESCREVER ARQUIVO.\n");
								return;
							}
						}
						*posicao = nomes.posArq;
						if (*verificarRotacao == 1) {
							switch (nomes.fatorBalanceamento) {
							case 1:
								nomes.fatorBalanceamento = 0;
								*verificarRotacao = 0;
								fseek(arqNomesSobrenomes, nomes.posArq * sizeof(arqIndice), 0);
								status = fwrite(&nomes, sizeof(arqIndice), 1, arqNomesSobrenomes);
								if (status != 1) {
									printf("ERRO AO ESCREVER ARQUIVO.\n");
									return;
								}
								break;
							case 0:
								nomes.fatorBalanceamento = -1;
								fseek(arqNomesSobrenomes, nomes.posArq * sizeof(arqIndice), 0);
								status = fwrite(&nomes, sizeof(arqIndice), 1, arqNomesSobrenomes);
								if (status != 1) {
									printf("ERRO AO ESCREVER ARQUIVO.\n");
									return;
								}
								break;
							case -1:
								rotacionarArqNomesEsq(arqNomesSobrenomes, arqNoDescritor, *posicao, verificarRotacao);
								break;
							}
						}
					}
				}
			}
		}
	}
}

int removerArqNomesSobrenomes(FILE *arqNomes, FILE *arqSobrenomes, FILE *arqNoDescritorNome, FILE *arqNoDescritorSobrenome, char nome[], char sobrenome[], int qtdArqNome, int qtdArqSobrenome) {
	arqIndice nomeStruct;
	arqIndice sobrenomeStruct;
	arqIndice dadosStruct;
	arqIndiceInicio structArqNoDescritorNome;
	arqIndiceInicio structArqNoDescritorSobrenome;
	int buscaArqNomes;
	int buscaArqSobrenomes;
	int retorno;
	int status;
	buscaArqNomes = buscarArqNomesSobrenomes(arqNomes, arqNoDescritorNome, nome, qtdArqNome, 1);
	buscaArqSobrenomes = buscarArqNomesSobrenomes(arqSobrenomes, arqNoDescritorSobrenome, sobrenome, qtdArqSobrenome, 1);
	if (buscaArqNomes >= 0 && buscaArqSobrenomes >= 0) {
		fseek(arqNoDescritorNome, 0, 0);
		status = fread(&structArqNoDescritorNome, sizeof(arqIndiceInicio),1, arqNoDescritorNome);
		if (status != 1) {
			return -1;
		}
		
		fseek(arqNoDescritorSobrenome, 0, 0);
		status = fread(&structArqNoDescritorSobrenome, sizeof(arqIndiceInicio), 1, arqNoDescritorSobrenome);
		if (status != 1) {
			return -1;
		}
		fseek(arqNomes, buscaArqNomes * sizeof(arqIndice), 0);
		status = fread(&nomeStruct, sizeof(arqIndice), 1, arqNomes);
		if (status != 1) {
			return -1;
		}
		fseek(arqSobrenomes, buscaArqSobrenomes * sizeof(arqIndice), 0);
		status = fread(&sobrenomeStruct, sizeof(arqIndice), 1, arqSobrenomes);
		if (status != 1) {
			return -1;
		}
		retorno = nomeStruct.posArqDados;
		nomeStruct.status = 0;
		structArqNoDescritorNome.comecoArqNomeSobrenomeStatusApagado = nomeStruct.posArq;
		sobrenomeStruct.status = 0;
		structArqNoDescritorSobrenome.comecoArqNomeSobrenomeStatusApagado = nomeStruct.posArq;
		
		fseek(arqNoDescritorNome, 0, 0);
		status = fwrite(&structArqNoDescritorNome, sizeof(arqIndiceInicio), 1, arqNoDescritorNome);
		if (status != 1) {
			return -1;
		}

		fseek(arqNoDescritorSobrenome, 0, 0);
		status = fwrite(&structArqNoDescritorSobrenome, sizeof(arqIndiceInicio), 1, arqNoDescritorSobrenome);
		if (status != 1) {
			return -1;
		}
		fseek(arqNomes, buscaArqNomes * sizeof(arqIndice), 0);
		status = fwrite(&nomeStruct, sizeof(arqIndice), 1, arqNomes);
		if (status != 1) {
			return -1;
		}
		fseek(arqSobrenomes, buscaArqSobrenomes * sizeof(arqIndice), 0);
		status = fwrite(&sobrenomeStruct, sizeof(arqIndice), 1, arqSobrenomes);
		if (status != 1) {
			return -1;
		}
		return retorno;
	}
	else if(buscaArqNomes == -1 && buscaArqSobrenomes == -1){
		return -1;
	}
	else {
		return -2;
	}

}
int buscarArqNomesSobrenomes(FILE *arqNomesSobrenomes, FILE *arqNoDescritor, char nome[], int qtd, int aux1) {
	arqIndice nomes;
	arqIndiceInicio aux;
	int ctt;
	int posicao;
	int status;
	ctt = 0;
	if (qtd == 0) {
		return -2;
	}
	fseek(arqNoDescritor, 0, 0);
	status = fread(&aux, sizeof(arqIndiceInicio), 1, arqNoDescritor);
	if (status != 1) {
		printf("ERRO AO LER ARQUIVO NO DESCRITOR.\n");
	}
	else {
		posicao = aux.comecoArqNomeSobrenomeStatusOk;
		fseek(arqNomesSobrenomes, posicao * sizeof(arqIndice), 0);
		while(posicao != -1){
			fseek(arqNomesSobrenomes, posicao * sizeof(arqIndice), 0);
			status = fread(&nomes, sizeof(arqIndice), 1, arqNomesSobrenomes);
			if (status != 1) {
				if (!feof(arqNomesSobrenomes)) {
					return -1;
				}
				return -2;
			}
			else {
				if (nomes.status == 0) {
					return -2;
				}
				if (strcmp(nome, nomes.string) == 0) {
					if(aux1 == 1){
						return nomes.posArq;
					}
					else{
						return nomes.posArqDados;
					}
				}
				else if(strcmp(nome, nomes.string) < 0){
					posicao = nomes.esq;
				}
				else {
					posicao = nomes.dir;
				}
			}
		}
		return -2;
			
	}
}

void rotacionarArqNomesDir(FILE *arq, FILE *arqNoDescritor, int posicao, int *verificarRotacao) {
	arqIndice aux1;
	arqIndice aux2;
	arqIndice aux3;
	arqIndiceInicio noDescr;
	int aux_int;
	int status;
	int aux_int1;

	aux_int = 0;
	aux_int1 = 0;
	fseek(arqNoDescritor, 0, 0);
	status = fread(&noDescr, sizeof(arqIndiceInicio), 1, arqNoDescritor);
	if (status != 1) {
		printf("ERRO.\n");
		return;
	}
	if (noDescr.comecoArqNomeSobrenomeStatusOk == posicao) {
		aux_int = 1;
	}

	fseek(arq, posicao * sizeof(arqIndice), 0);
	status = fread(&aux1, sizeof(arqIndice), 1, arq);
	if (status != 1) {
		printf("ERRO.\n");
		return;
	}
	fseek(arq, aux1.esq * sizeof(arqIndice), 0);
	status = fread(&aux2, sizeof(arqIndice), 1, arq);
	if (status != 1) {
		printf("ERRO.\n");
		return;
	}
	if (aux2.fatorBalanceamento == 1) {
		aux1.esq = aux2.dir;
		aux2.dir = aux1.posArq;
		aux1.fatorBalanceamento = 0;
		if (aux_int == 1) {
			noDescr.comecoArqNomeSobrenomeStatusOk = aux2.posArq;
		}
	}
	else {
		aux_int1 = 1;
		fseek(arq, aux2.dir * sizeof(arqIndice), 0);
		status = fread(&aux3, sizeof(arqIndice), 1, arq);
		if (status != 1) {
			printf("ERRO.\n");
			return;
		}
		aux2.dir = aux3.esq;
		aux3.esq = aux2.posArq;
		aux1.esq = aux3.dir;
		aux3.dir = aux1.posArq;
		if (aux3.fatorBalanceamento == 1) {
			aux1.fatorBalanceamento = -1;
		}
		else {
			aux1.fatorBalanceamento = 0;
		}
		if (aux3.fatorBalanceamento == -1) {
			aux2.fatorBalanceamento = 1;
		}
		else {
			aux2.fatorBalanceamento = 0;
		}
		if (aux_int == 1) {
			noDescr.comecoArqNomeSobrenomeStatusOk = aux3.posArq;
		}
		fseek(arq, aux3.posArq * sizeof(arqIndice), 0);
		status = fwrite(&aux3, sizeof(arqIndice), 1, arq);
		if (status != 1) {
			printf("ERRO.\n");
			return;
		}
	}
	if (aux_int1 == 1) {
		aux3.fatorBalanceamento = 0;
	}
	else {
		aux2.fatorBalanceamento = 0;
	}
	*verificarRotacao = 0;
	fseek(arq, aux1.posArq * sizeof(arqIndice), 0);
	status = fwrite(&aux1, sizeof(arqIndice), 1, arq);
	if (status != 1) {
		printf("ERRO.\n");
		return;
	}
	fseek(arq, aux2.posArq * sizeof(arqIndice), 0);
	status = fwrite(&aux2, sizeof(arqIndice), 1, arq);
	if (status != 1) {
		printf("ERRO.\n");
		return;
	}
	if (aux_int == 1) {
		fseek(arqNoDescritor, 0, 0);
		status = fwrite(&noDescr, sizeof(arqIndice), 1, arqNoDescritor);
		if (status != 1) {
			printf("ERRO.\n");
			return;
		}
	}
}

void rotacionarArqNomesEsq(FILE *arq, FILE *arqNoDescritor, int posicao, int *verificarRotacao) {
	arqIndice aux1;
	arqIndice aux2;
	arqIndice aux3;
	arqIndiceInicio noDescr;
	int aux_int;
	int status;
	int aux_int1;

	aux_int = 0;
	aux_int1 = 0;
	fseek(arqNoDescritor, 0, 0);
	status = fread(&noDescr, sizeof(arqIndiceInicio), 1, arqNoDescritor);
	if (status != 1) {
		printf("ERRO.\n");
		return;
	}
	if (noDescr.comecoArqNomeSobrenomeStatusOk == posicao) {
		aux_int = 1;
	}

	fseek(arq, posicao * sizeof(arqIndice), 0);
	status = fread(&aux1, sizeof(arqIndice), 1, arq);
	if (status != 1) {
		printf("ERRO.\n");
		return;
	}
	fseek(arq, aux1.dir * sizeof(arqIndice), 0);
	status = fread(&aux2, sizeof(arqIndice), 1, arq);
	if (status != 1) {
		printf("ERRO.\n");
		return;
	}
	if (aux2.fatorBalanceamento == 1) {
		aux1.dir = aux2.esq;
		aux2.esq = aux1.posArq;
		aux1.fatorBalanceamento = 0;
		if (aux_int == 1) {
			noDescr.comecoArqNomeSobrenomeStatusOk = aux2.posArq;
		}
	}
	else {
		aux_int1 = 1;
		fseek(arq, aux2.esq * sizeof(arqIndice), 0);
		status = fread(&aux3, sizeof(arqIndice), 1, arq);
		if (status != 1) {
			printf("ERRO.\n");
			return;
		}
		aux2.esq = aux3.dir;
		aux3.dir = aux2.posArq;
		aux1.dir = aux3.esq;
		aux3.esq = aux1.posArq;
		if (aux3.fatorBalanceamento == 1) {
			aux1.fatorBalanceamento = -1;
		}
		else {
			aux1.fatorBalanceamento = 0;
		}
		if (aux3.fatorBalanceamento == -1) {
			aux2.fatorBalanceamento = 1;
		}
		else {
			aux2.fatorBalanceamento = 0;
		}
		if (aux_int == 1) {
			noDescr.comecoArqNomeSobrenomeStatusOk = aux3.posArq;
		}
		fseek(arq, aux3.posArq * sizeof(arqIndice), 0);
		status = fwrite(&aux3, sizeof(arqIndice), 1, arq);
		if (status != 1) {
			printf("ERRO.\n");
			return;
		}
	}
	if (aux_int1 == 1) {
		aux3.fatorBalanceamento = 0;
	}
	else {
		aux2.fatorBalanceamento = 0;
	}
	*verificarRotacao = 0;
	fseek(arq, aux1.posArq * sizeof(arqIndice), 0);
	status = fwrite(&aux1, sizeof(arqIndice), 1, arq);
	if (status != 1) {
		printf("ERRO.\n");
		return;
	}
	fseek(arq, aux2.posArq * sizeof(arqIndice), 0);
	status = fwrite(&aux2, sizeof(arqIndice), 1, arq);
	if (status != 1) {
		printf("ERRO.\n");
		return;
	}
	if (aux_int == 1) {
		fseek(arqNoDescritor, 0, 0);
		status = fwrite(&noDescr, sizeof(arqIndiceInicio), 1, arqNoDescritor);
		if (status != 1) {
			printf("ERRO.\n");
			return;
		}
	}
}
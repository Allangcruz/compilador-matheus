#include <locale.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**
 * Total de memoria consumida no compilador.
 */
int TOTAL_CONSUMO_MEMORIA = 0;

/**
 * 256000 bytes => 256 kilobyte
 */
int MAX_TOTAL_CONSUMO_MEMORIA = 256000;

/**
 * Palavras reservadas
 */
char reservadas[9][8] = {
    "main",
    "puts",
    "gets",
    "if",
    "else",
    "for",
    "integer",
    "string",
    "decimal",
};

/**
 * Referencias de palavras reservadas
 */
int REF_MAIN = 0;
int REF_PUTS = 1;
int REF_GETS = 2;
int REF_IF = 3;
int REF_ELSE = 4;
int REF_FOR = 5;
int REF_INTEGER = 6;
int REF_STRING = 7;
int REF_DECIMAL = 8;

void exibirError(int nuLinha, int tipoErro, char *palavra);
void removerQuebraLinha(char* palavra);
void limparArray(char vetor[]);
void getConsumoMemoria();
void setMemoria(int memoria, int situacao);
void removerTabulacao(char* palavra);
void removerCaracterEspaco(char *palavra);
void removePalavrasComAspas(char *palavra, char *palavraComAspas);

int isCondicaoParada(int ascii);
int isLinhaVazia(char *palavra);
int isReservada(char *palavra);
int isTipoVariavel(char *palavra);
int isVerificaFinalizacaoPontoVingula(char *palavra, int nuLinha);
int isMainValido(char *palavra, int nuLinha);
int isVerificaDeclaracaoVariavel(char *palavra, int nuLinha);


void removePalavrasComAspas(char *palavra, char *palavraComAspas) {

	int i, valorAscii, countPalavra = 0, countPalavraValida = 0, isPossuiAspas = 0;

	char palavraComConteudoAspas[UCHAR_MAX];
	char palavraSemConteudoAspas[UCHAR_MAX];

	setMemoria(sizeof(i), 1);
	setMemoria(sizeof(valorAscii), 1);
	setMemoria(sizeof(countPalavra), 1);
	setMemoria(sizeof(countPalavraValida), 1);
	setMemoria(sizeof(isPossuiAspas), 1);
	setMemoria(sizeof(palavraComConteudoAspas), 1);
	setMemoria(sizeof(palavraSemConteudoAspas), 1);

	limparArray(palavraComConteudoAspas);
	limparArray(palavraSemConteudoAspas);
	limparArray(palavraComAspas);

	for (i = 0; i < strlen(palavra); i++) {

		valorAscii = (int) palavra[i];

		if (isPossuiAspas == 0 && valorAscii == 34) {
			isPossuiAspas = 1;
			palavraComConteudoAspas[countPalavra] = palavra[i];
			countPalavra++;
			continue;
		}

		if (isPossuiAspas > 0 && valorAscii != 34) {
			palavraComConteudoAspas[countPalavra] = palavra[i];
			countPalavra++;
			continue;
		}

		if (isPossuiAspas == 1 && valorAscii == 34) {
			palavraComConteudoAspas[countPalavra] = palavra[i];
			countPalavra++;
			isPossuiAspas = 0;
			continue;
		}

		if ((isPossuiAspas == 0)) {
			palavraSemConteudoAspas[countPalavraValida] = palavra[i];
			countPalavraValida++;
		}
	}

	strcpy(palavra, palavraSemConteudoAspas);
	strcpy(palavraComAspas, palavraComConteudoAspas);
}

int isVerificaDeclaracaoVariavel(char *palavra, int nuLinha) {
    int i, ascii, primeiroAscii;

	ascii = (int) palavra[0];

    if (strlen(palavra) == 0) {
        return 0;
    }
	if (ascii == 35) {
        primeiroAscii =(int)palavra[1];
		if (!(primeiroAscii >= 97 && primeiroAscii <= 122)) {
			exibirError(nuLinha, 7, palavra);
		}

		for (i = 2; i < strlen(palavra); i++) {
			ascii = (int) palavra[i];

			if (! (
                (ascii >= 97 && ascii <= 122) || 
                (ascii >= 48 && ascii <= 57) || 
                (ascii >= 65 && ascii <= 90) ||
                (ascii == 91 || ascii == 93 || ascii == 46))
            )
            {
			    exibirError(nuLinha, 7, palavra);
			}
		}
	} else {
        return 0;
    }

	return 1;
}

int isMainValido(char *palavra, int nuLinha) {
	int i, ascii, count = 0, isInValido = 0;
	char palavraTmp[UCHAR_MAX];	
	char palavraProcessada[UCHAR_MAX];
	limparArray(palavraTmp);
	limparArray(palavraProcessada);
	
	strcpy(palavraProcessada, palavra);
	
	removerQuebraLinha(palavraProcessada);
	removerTabulacao(palavraProcessada);
	removerCaracterEspaco(palavraProcessada);
		
	for (i = 0; i < strlen(palavraProcessada); i++) {
		ascii = (int) palavraProcessada[i];
		palavraTmp[count] = (char) ascii;
		count++;
				
		if (strcmp(palavraTmp, "main(") == 0) {
			isInValido = 1;
			break;
		}
	}
	
	if (!isInValido) {
		exibirError(nuLinha, 2, palavra);
	}
	
	isInValido = 0;
	count = 0;
	limparArray(palavraTmp);
	
	for (i = 0; i < strlen(palavraProcessada); i++) {
		ascii = (int) palavraProcessada[i];
		palavraTmp[count] = (char) ascii;
		count++;
				
		if (strcmp(palavraTmp, "main(){") == 0) {
			isInValido = 1;
			break;
		}
	}
	
	if (!isInValido) {
		exibirError(nuLinha, 3, palavra);
	}
	
	return 1;
}

void removerCaracterEspaco(char *palavra) {
	int i, ascii, count = 0;
	char palavraTmp[UCHAR_MAX];
	limparArray(palavraTmp);

	for (i = 0; i < strlen(palavra); i++) {
		ascii = (int) palavra[i];
		// Espaco => 32
		if (ascii != 32) {
			palavraTmp[count] = palavra[i];
			count++;
		}
	}

	strcpy(palavra, palavraTmp);
	setMemoria(sizeof(i), 1);
	setMemoria(sizeof(ascii), 1);
	setMemoria(sizeof(count), 1);
	setMemoria(sizeof(palavraTmp), 1);
}

int isVerificaFinalizacaoPontoVingula(char *palavra, int nuLinha) {
	char palavraProcessada[UCHAR_MAX];
	limparArray(palavraProcessada);
	
	strcpy(palavraProcessada, palavra);
	
	removerQuebraLinha(palavraProcessada);
	removerCaracterEspaco(palavraProcessada);
	
	int i, ascii;
	int tamanhoPalavra = strlen(palavraProcessada) - 1;
	int tamanhoPalavraPosicao = tamanhoPalavra;
	int quantidadeVezesTestarCondicao = 0;
	
	
	for (i = tamanhoPalavra; i >= 0; i--) {
		ascii = palavraProcessada[i];
		
		if (ascii == 59 && i == tamanhoPalavraPosicao) {
			return 1;
		}
		
		if (ascii == 59 && i == tamanhoPalavraPosicao) {
			return 1;
		}
		
		if (quantidadeVezesTestarCondicao > 2) {
			return 0;
		}
		
		quantidadeVezesTestarCondicao++;
		tamanhoPalavraPosicao--;
	}
	return 0;
}

int isTipoVariavel(char *palavra) {
	if (strcmp(palavra, reservadas[REF_INTEGER]) == 0) {
		return 1;
	}

	if (strcmp(palavra, reservadas[REF_STRING]) == 0) {
		return 1;
	}

	if (strcmp(palavra, reservadas[REF_DECIMAL]) == 0) {
		return 1;
	}

	return 0;
}

int isReservada(char *palavra) {
    int i;

    setMemoria(1, sizeof(i));

    for (i = 0; i < 12; i++) {
		if (strcmp(palavra, reservadas[i]) == 0) {
			return 1;
		}		
	}
    return 0;
}

void limparArray(char vetor[]) { // TODO metodo duplicado
	int i = 0;

	while (i < UCHAR_MAX) {
		vetor[i] = '\0';
		i ++;
	}
}

void removerQuebraLinha(char* palavra) {
	int i, valorAscii, count = 0;
	char palavraAux[UCHAR_MAX];
	limparArray(palavraAux);

	for (i = 0; i < strlen(palavra); i++) {
		valorAscii = (int) palavra[i];

		if ((valorAscii != 13) && (valorAscii != 10)) {
			palavraAux[count] = palavra[i];
			count++;
		}
	}

	strcpy(palavra, palavraAux);

	setMemoria(sizeof(i), 1);
	setMemoria(sizeof(valorAscii), 1);
	setMemoria(sizeof(count), 1);
	setMemoria(sizeof(palavraAux), 1);
}

void exibirError(int nuLinha, int tipoErro, char *palavra) {
	
	if (tipoErro != 8) {
		removerQuebraLinha(palavra);
	}

	switch(tipoErro) {
        case 1:
			printf("\nlinha => %i - Simbolo ou palavra nao identificados (%s).\n", nuLinha, palavra);
			getConsumoMemoria();
			exit(1);
		break;

		case 2:
			printf("linha => %i -  Palavra 'main' esta incorreta, favor verificar abertura de parentes - (%s).\n", nuLinha, palavra);
			getConsumoMemoria();
            exit(0);
		break;
		
		case 3:
			printf("linha => %i -  Palavra 'main' esta incorreta, favor verificar abertura de chaves ou remova os parametros - (%s).\n", nuLinha, palavra);
			getConsumoMemoria();
            exit(0);
		break;

		case 4:
            printf("linha => %i -  declaracao de 'puts' esta incorreta. Verificar os parenteses ou existencia de parametros. - (%s).\n", nuLinha, palavra);
            getConsumoMemoria();
            exit(0);
        break;

		case 5:
            printf("linha => %i - declaracao de 'gets' esta incorreta. Verificar os parenteses ou existencia de parametros. - (%s).\n", nuLinha, palavra);
            getConsumoMemoria();
            exit(0);
        break;

		case 6:
            printf("linha => %i - Nao existe ponto e virgula. - (%s).\n", nuLinha, palavra);
            getConsumoMemoria();
            exit(0);
        break;

		case 7:
            printf("linha => %i - A declaracao da variavel e invalida. - (%s).\n", nuLinha, palavra);
            getConsumoMemoria();
            exit(0);
        break;

		case 8:
			printf("Funcao 'main()' Inexistente.\n");
			getConsumoMemoria();
			exit(1);
		break;

		default:
			printf("\nError informado nao encontrado - (%d) - (%d) - (%s).\n", nuLinha, tipoErro, palavra);
		break;
	}
	printf("\n");
}

void setMemoria(int memoria, int situacao) {
	if (situacao == 1) {
		TOTAL_CONSUMO_MEMORIA = TOTAL_CONSUMO_MEMORIA + memoria;
	} else {
		TOTAL_CONSUMO_MEMORIA = TOTAL_CONSUMO_MEMORIA - memoria;
	}

	float porcentagem = 0;
	if (MAX_TOTAL_CONSUMO_MEMORIA > 0) {
		porcentagem = (TOTAL_CONSUMO_MEMORIA * 100) / MAX_TOTAL_CONSUMO_MEMORIA;
	}

	if (porcentagem > 90 && porcentagem < 99) {
		printf("Sua memoria esta entre 90 %% a 99 %% do total disponível, memoria atual: %.2f %%\n\n", porcentagem);
	}

	if (TOTAL_CONSUMO_MEMORIA > MAX_TOTAL_CONSUMO_MEMORIA) {
		printf ("Memoria ultrapassou o limite disponivel.\n");
		getConsumoMemoria();
		exit(0);
	}
}

void getConsumoMemoria() {
	printf("\n---------------------------------------------------------------------\n");
	printf("\nCONSUMO DE MEMORIA: %d bytes\n\n", TOTAL_CONSUMO_MEMORIA);
	float porcentagem = 0;
	if (MAX_TOTAL_CONSUMO_MEMORIA > 0) {
		porcentagem = (TOTAL_CONSUMO_MEMORIA * 100) / MAX_TOTAL_CONSUMO_MEMORIA;
	}	 

	printf("Porcentagem consumida => %.2f %% de %i bytes\n\n" , porcentagem, MAX_TOTAL_CONSUMO_MEMORIA);
}

int isLinhaVazia(char *palavra) {
    int i, ascii;

    for (i = 0; i < strlen(palavra); i++) {
        ascii = (int) palavra[i];
        if (ascii != 32 && ascii != 9 && ascii != 10 && ascii != 13) {
            return 0;
        }
    }

    return 1;
}

void removerTabulacao(char* palavra) {
	int i, valorAscii, count = 0;
	char palavraAux[UCHAR_MAX];
	limparArray(palavraAux);

	for (i = 0; i < strlen(palavra); i++) {
		valorAscii = (int) palavra[i];
		// Tab => 9
		if (valorAscii != 9) {
			palavraAux[count] = palavra[i];
			count++;
		}
	}

	strcpy(palavra, palavraAux);

	setMemoria(sizeof(i), 1);
	setMemoria(sizeof(valorAscii), 1);
	setMemoria(sizeof(count), 1);
	setMemoria(sizeof(palavraAux), 1);
}

int isCondicaoParada(int ascii) {
    setMemoria(1, sizeof(ascii));

    if (
        (ascii != 10) && // \0 -> 10
        (ascii != 9) && // tab -> 32
        (ascii != 32) && // espaco -> 32
        (ascii != 40) && // ( -> 40 
        (ascii != 41) && // ) -> 41
        (ascii != 59) && // ; -> 59
        (ascii != 123) && // { -> 123
        (ascii != 125) && // } -> 125
        (ascii != 44) && // , -> 44
        (ascii != 36) && // $ -> 36
        (ascii != 33) && // ! -> 33
        (ascii != 58) && // : -> 58
        (ascii != 34) && // " -> 34
        (ascii != 60) && // < : 60
        (ascii != 61) && // = : 61
        (ascii != 62) && // > : 62
        (ascii != 43) && // + : 43
        (ascii != 10) && // Line Feed - LF (Windows) -> 10
        (ascii != 13) // Enter - CR (Unix) -> 13

    ) {
        return 0;
    }
    return 1;
}

int main () {
	printf("---------------------------------------------------------------------\n");
	printf("=> Compiladores\n");
	printf("=> Matheus Torres\n");
	printf("---------------------------------------------------------------------\n");
	
	char arquivo[] = "arquivo.txt";

	setMemoria(sizeof(arquivo), 1);

	FILE *arquivoTxt;
    arquivoTxt = fopen(arquivo, "r");
	setMemoria(sizeof(arquivoTxt), 1);

    if (arquivoTxt == NULL) {
    	printf("\nArquivo nao encontrado.\n");
        return 0;
	}

	// ---------------------------------------------------------------------
	int i = 0;
    char acumulador[UCHAR_MAX];
    char conteudoLinha[UCHAR_MAX];
	char nomeTipoVariavel[UCHAR_MAX];
    char conteudoPorCaracter[UCHAR_MAX];
	char conteudoLinhaComAspas[UCHAR_MAX];

	// Limpando lixo de memoria.
    limparArray(acumulador);
    limparArray(conteudoLinha);
	limparArray(nomeTipoVariavel);
	limparArray(conteudoPorCaracter);
	limparArray(conteudoLinhaComAspas);

	int nuLinha = 1;
    int ascii;
    int tamanhoLinha = 0;
    int isUltimoCaracter = 0;
	int count = 0;
	int isPuts = 0;
	int isGets = 0;
	int isMainExistente = 0;
	int isVariavelValida = 0;
	int isPossuiDeclaracaoVariavel = 0;
	int isReservadaValida = 0;
	int isAtribuicao = 0;

	if (arquivoTxt != NULL) {
		while ((fgets(conteudoLinha, sizeof(conteudoLinha), arquivoTxt)) != NULL) {
			strcpy(conteudoPorCaracter, conteudoLinha);
			removerTabulacao(conteudoPorCaracter);

			removePalavrasComAspas(conteudoPorCaracter, conteudoLinhaComAspas);
			tamanhoLinha = strlen(conteudoPorCaracter);

			if (tamanhoLinha > 0 && isLinhaVazia(conteudoPorCaracter) == 0) {

				isUltimoCaracter = 0;
				if (tamanhoLinha == (i + 1)) {
					isUltimoCaracter = 1;
				}

				for (i = 0; i < tamanhoLinha; i++) {
					// TODO ...
					ascii = (int) conteudoPorCaracter[i];
					

					if (i == (tamanhoLinha -1) && !isCondicaoParada(ascii)) {
						acumulador[count] = (char) ascii;
						count++;
					}

					if (! isCondicaoParada(ascii) && (i != (tamanhoLinha -1))) {
						acumulador[count] = (char) ascii;
						count++;
						// printf("Acumulador => (%d) - (%c) - (%s)\n", nuLinha, ascii, acumulador);
					} else {
						// verifica se é declaração de variavel e atribuição '=' => 61
						if (ascii == 61) {
							isAtribuicao = true;
						}

						isVariavelValida = isVerificaDeclaracaoVariavel(acumulador, nuLinha);
						if (isVariavelValida) {
							// TODO tratar... 
						}

						isReservadaValida = isReservada(acumulador);
						if (isReservadaValida) {
							if (isTipoVariavel(acumulador) == 1) {
								isPossuiDeclaracaoVariavel = 1;
								
								if (! isVerificaFinalizacaoPontoVingula(conteudoPorCaracter, nuLinha)) {
									exibirError(nuLinha, 6, conteudoLinha);
								}
								
								strcpy(nomeTipoVariavel, acumulador);
							}
							
							if (strcmp(acumulador, reservadas[REF_GETS]) == 0) {
								if (! isVerificaFinalizacaoPontoVingula(conteudoPorCaracter, nuLinha)) {
									exibirError(nuLinha, 6, conteudoLinha);
								}
							}
							
							if (strcmp(acumulador, reservadas[REF_PUTS]) == 0) {
								if (! isVerificaFinalizacaoPontoVingula(conteudoPorCaracter, nuLinha)) {
									exibirError(nuLinha, 6, conteudoLinha);
								}
							}
							
							if (strcmp(acumulador, reservadas[REF_MAIN]) == 0) {
								isMainExistente ++;
								isMainValido(conteudoPorCaracter, nuLinha);								
							}
						}

						// TODO ...
						// printf("Debug => (%d) - (%c) - (%s)\n", nuLinha, ascii, acumulador);
						if (strlen(acumulador) > 0) {
							if (!isVariavelValida && !isReservadaValida && !isAtribuicao) {
								exibirError(nuLinha, 1, acumulador);
							}
						}

						if (isAtribuicao == true) {
							if (isCondicaoParada(ascii) == 0) {
								acumulador[count] = (char) ascii;
								count++;
							}
						}

						limparArray(acumulador);
						count = 0;

					} // fim else

					if (ascii == 36) {
						acumulador[count] = (char) ascii;
						count++;
					}					
				}

				// limpar para a proxima linha
				limparArray(acumulador);
				count = 0;
				isAtribuicao = 0;
				isReservadaValida = 0;
				isVariavelValida = 0;
				nuLinha ++;
			}
		} // fim else

		if (!isMainExistente) {
			char tmp[]= {""};
			exibirError(0, 8, tmp);
		}
	}
    getConsumoMemoria();
}
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define SCRIPT		"http://127.0.0.1/cgi-bin/cartas"
#define BARALHO		"http://127.0.0.1/cards"

#define NAIPES		"DCHS"
#define VALORES		"3456789TJQKA2"

/**
Estado inicial com todas as 52 cartas do baralho
Cada carta é representada por um bit que está
a 1 caso ela pertença à mão ou 0 caso contrário
*/
long long int ESTADO_INICIAL = 0xfffffffffffff;

/**
Devolve o índice da carta
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		O índice correspondente à carta
*/
int indice(int naipe, int valor) {
	return naipe * 13 + valor;
}

/**
Adiciona uma carta ao estado
@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		O novo estado
*/
long long int add_carta(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return ESTADO | ((long long int) 1 << idx);
}

/**
Remove uma carta do estado
@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		O novo estado
*/
long long int rem_carta(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return ESTADO & ~((long long int) 1 << idx);
}

/**
Verifica se uma carta pertence ao estado
@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		1 se a carta existe e 0 caso contrário
*/
int carta_existe(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return (ESTADO >> idx) & 1;
}


/*int nrosCartas(long long int MAO){
	int  a = 0;
	while(MAO != 0){
		if(MAO % 2 == 1){
			a ++;
		}
		MAO = MAO/2;
	}
	return a;
}*/



/**
Funçao que distribui as cartas
*/
long long int* distribuir(long long int ESTADO) {
	int x,y,n;
	static long long int maos [4]; //fixa a variavel na memoria, enquanto o programa estiver a correr
	long long int maoUser=0;
	long long int maoCP1=0;
	long long int maoCP2=0;
	long long int maoCP3=0;
	srandom(time(NULL));
	n=0;
	while (n<13) {
		x = random() % 4;
		y = random() % 13;
		if (carta_existe(ESTADO,x,y)) {
		maoUser = add_carta(maoUser,x,y);
		ESTADO = rem_carta(ESTADO,x,y);
		n=n+1;
		}
	}
	maos[0] = maoUser;
	n=0;
	while (n<13) {
		x = random() % 4;
		y = random() % 13;
		if (carta_existe(ESTADO,x,y)) {
		maoCP1 = add_carta(maoCP1,x,y);
		ESTADO = rem_carta(ESTADO,x,y);
		n=n+1;
		}
	}
	maos[1] = maoCP1;
	n=0;
	while (n<13) {
		x = random() % 4;
		y = random() % 13;
		if (carta_existe(ESTADO,x,y)) {
		maoCP2 = add_carta(maoCP2,x,y);
		ESTADO = rem_carta(ESTADO,x,y);
		n=n+1;
		}
	}
	maos[2] = maoCP2;
	n=0;
	while (n<13) {
		x = random() % 4;
		y = random() % 13;
		if (carta_existe(ESTADO,x,y)) {
		maoCP3 = add_carta(maoCP3,x,y);
		ESTADO = rem_carta(ESTADO,x,y);
		n=n+1;
		}
	}
	maos[3] = maoCP3;
	return (&maos[1]);
}

/**
Imprime o html correspondente a uma carta
@param path	o URL correspondente à pasta que contém todas as cartas
@param x A coordenada x da carta
@param y A coordenada y da carta
@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
*/
void imprime_carta(char *path, int x, int y, long long int ESTADO, int naipe, int valor) {
	char *suit = NAIPES;
	char *rank = VALORES;
	char script[10240];
	sprintf(script, "%s?q=%lld", SCRIPT, rem_carta(ESTADO, naipe, valor));
	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%c%c.svg\" /></a>\n", script, x, y, path, rank[valor], suit[naipe]);
}

/**
Esta função está a imprimir o estado em quatro colunas: uma para cada naipe
@param path	o URL correspondente à pasta que contém todas as cartas
@param ESTADO	O estado atual
*/
void imprime(char *path, long long int ESTADO) {
	int n, v;
	int x, y;

	printf("<svg height = \"800\" width = \"800\">\n");
	printf("<rect x = \"0\" y = \"0\" height = \"800\" width = \"800\" style = \"fill:#007700\"/>\n");

	for(y = 10, n = 0; n < 4; n++, y += 120) {
		for(x = 10, v = 0; v < 13; v++)
			if(carta_existe(ESTADO, n, v)) {
				x += 20;
				imprime_carta(path, x, y, ESTADO, n, v);
			}
	}
	printf("</svg>\n");
}

/**
Esta função recebe a query que é passada à cgi-bin e trata-a.
Neste momento, a query contém o estado que é um inteiro que representa um conjunto de cartas.
Cada carta corresponde a um bit que está a 1 se essa carta está no conjunto e a 0 caso contrário.
Caso não seja passado nada à cgi-bin, ela assume que todas as cartas estão presentes.
@query A query que é passada à cgi-bin
 */
void parse(char *query) {
	long long int ESTADO;
	//long long int* maos;
	long long int* maos;
	long long int maoUser;
	//long long int maoCP1;
	if(sscanf(query, "q=%lld", &ESTADO) == 1) {
		imprime(BARALHO, ESTADO);
	} else {
		maos = distribuir(ESTADO_INICIAL);
		maoUser = *maos;
		//printf("%lld", maoUser);
		//maoCP1 = *((&maoUser) ++);
		imprime(BARALHO, maoUser);
	}
}

/**
Função principal do programa que imprime os cabeçalhos necessários e depois disso invoca
a função que vai imprimir o código html para desenhar as cartas
 */
int main() {
/*
 * Cabeçalhos necessários numa CGI
 */
	printf("Content-Type: text/html; charset=utf-8\n\n");
	printf("<header><title>Exemplo</title></header>\n");
	printf("<body>\n");

	printf("<h1>Exemplo de utilização</h1>\n");

/*
 * Ler os valores passados à cgi que estão na variável ambiente e passá-los ao programa
 */
	parse(getenv("QUERY_STRING"));

	printf("</body>\n");
	return 0;
}

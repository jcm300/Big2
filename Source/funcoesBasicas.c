#include <stdio.h>
#include "estruturas.h"
#include "funcoesBasicas.h"

/**
Passa uma string para o estado STATE
*/
STATE str2estado (char* str) {
	STATE e;
	sscanf(str, FORMATO, &e.mao[0], &e.mao[1], &e.mao[2], &e.mao[3], &e.tamanho[0], &e.tamanho[1], &e.tamanho[2],&e.tamanho[3], &e.pontos[0], &e.pontos[1], &e.pontos[2],&e.pontos[3],&e.selecao, &e.acao, &e.passar, &e.ultimo_jogador, &e.ultima_jogada, &e.ordem);
	return e;
}

/**
Passa o estado STATE para string 
*/
char* estado2str (STATE e){
	static char res[10240];
	sprintf(res, FORMATO, e.mao[0], e.mao[1], e.mao[2], e.mao[3], e.tamanho[0],e.tamanho[1],e.tamanho[2],e.tamanho[3], e.pontos[0], e.pontos[1], e.pontos[2], e.pontos[3], e.selecao, e.acao, e.passar, e.ultimo_jogador, e.ultima_jogada, e.ordem);
	return res;
}

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

/**
Número de cartas presentes na mão
@param m Mao onde se verifca o número de cartas
@return Número de cartas existentes na mão
*/
int nroCartas(MAO m){
	int r = 0;

	while(m > 0){
		if(m%2 == 1) r ++;
		m /= 2;
	}
	return r;	
}

/**Verifica se um determinado valor pertenceà mao, independentemente do naipe do mesmo
@param mao A mão onde se irá procurar o valor indicado 
@param valor O valor a procurar
@return 1 se o valor existir, 0 caso este não esteja presente
*/
int valor_existe(MAO mao, int valor) {
	int idx[4], i;
	for(i = 0; i < 4; i ++)	idx[i] = indice(i, valor);
	return (((mao >> idx[0]) & 1) || ((mao >> idx[1]) & 1) || ((mao >> idx[2]) & 1) || ((mao >> idx[3]) & 1));
}
/**Devolve a primeira ocorrência de um determinado valor na mão dada
@param mao Mao a percorrer
@param v valor a procurar
@return naipe do valor indicado, caso este valor não esteja presente a funcao devolve 4
*/
int existeValor (MAO mao, int v) {
	int n;
	for (n=0; n<4; n++)
		if(carta_existe(mao,n,v)) break;
	return n;
}

/** Função que adiciona à primeira mão as cartas que estejam presentes na segunda mão
@param mao mao qe contém as cartas a serem adicionadas
@param s mao onde as cartas vao ser adicionadas
@return mao que contém as cartas escolhidas
*/
MAO adiciona_cartas (MAO mao, MAO s) {
	int n,v;
	for(v = 0; v < 13; v++) {
		for(n = 0; n < 4; n++)
				if(carta_existe(s, n, v)) {
					mao=add_carta(mao,n,v);
			}
	}	
	return mao;
}

/** Função que retira da primeira mão as cartas que estejam presentes na segunda mão
@param mao mao que contém as cartas a serem removidas
@param s mao de onde serão retiradas as carta
@return mao resultante de remover as cartas 
*/
MAO retira_cartas (MAO mao, MAO s) {
	int n,v;
	for(v = 0; v < 13; v++) {
		for(n = 0; n < 4; n++)
				if(carta_existe(s, n, v)) {
					mao=rem_carta(mao,n,v);
				}
	}	
	return mao;
}

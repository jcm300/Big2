#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct {
  long long unsigned  int mao[4];
  int tamanho[4];
  long long unsigned int selecao;
  char acao;
} STATE;



int indice(int naipe, int valor) {
	return naipe * 13 + valor;
}



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



int nrosCartas(long long int MAO){
	int  a = 0;
	
	while(MAO != 0){
		if(MAO % 2 == 1){
			a ++;
		}
		MAO = MAO/2;
	}
	return a;
}

void distribuir(long long int ESTADO,long long int mao[]) {
	int x,y,n;
	mao[0]=0; /*mao do user*/
	mao[1]=0; /*mao do cp1*/
	mao[2]=0; /*mao do cp2*/
	mao[3]=0; /*mao do cp3*/
	srandom(time(NULL));
	n=0;
	while (n<13) {
		x = random() % 4;
		y = random() % 13;
		if (carta_existe(ESTADO,x,y)) {
		mao[0] = add_carta(mao[0],x,y);
		ESTADO = rem_carta(ESTADO,x,y);
		n=n+1;
		}
	}
	n=0;
	while (n<13) {
		x = random() % 4;
		y = random() % 13;
		if (carta_existe(ESTADO,x,y)) {
		mao[1] = add_carta(mao[1],x,y);
		ESTADO = rem_carta(ESTADO,x,y);
		n=n+1;
		}
	}
	n=0;
	while (n<13) {
		x = random() % 4;
		y = random() % 13;
		if (carta_existe(ESTADO,x,y)) {
		mao[2] = add_carta(mao[2],x,y);
		ESTADO = rem_carta(ESTADO,x,y);
		n=n+1;
		}
	}
	n=0;
	while (n<13) {
		x = random() % 4;
		y = random() % 13;
		if (carta_existe(ESTADO,x,y)) {
		mao[3] = add_carta(mao[3],x,y);
		ESTADO = rem_carta(ESTADO,x,y);
		n=n+1;
		}
	}
}


const long long int ESTADO_INICIAL = 0xfffffffffffff;


int main(){
	STATE e;


	distribuir(ESTADO_INICIAL, e.mao);


	printf("%d\n", nrosCartas(*(e.mao)));

	return 0;
}
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define SCRIPT		"http://127.0.0.1/cgi-bin/cartas"
#define BARALHO		"http://127.0.0.1/cards"

#define NAIPES		"DCHS"
#define VALORES		"3456789TJQKA2"
#define FORMATO 	"%lld_%lld_%lld_%lld_%d_%d_%d_%d_%lld_%d_%d_%d"

/**
Guarda o estado do jogo
@param mao cada uma das 4 mãos
@param tamanho o número de cartas em cada mão
@param selecao Que cartas foram neste momento selecionadas pelo jogador
@param acao se o jogador carregou em algum botão (e.g., jogar ou passar)
*/
typedef long long int MAO;

typedef struct {
  	long long int mao[4];
  	int tamanho[4];
  	long long int selecao;
  	int acao, jogar, selecionar;
	} STATE;

/**
Devolve o índice da carta
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		O índice correspondente à carta
*/
int indice(int naipe, int valor) {
	return naipe * 13 + valor;
}

STATE str2estado (char* str) {
	STATE e;
	sscanf(str, FORMATO, &e.mao[0], &e.mao[1], &e.mao[2], &e.mao[3], &e.tamanho[0], &e.tamanho[1], &e.tamanho[2],&e.tamanho[3], &e.selecao, &e.acao, &e.selecionar, &e.jogar);
	return e;
}

char* estado2str (STATE e){
	static char res[10240];
	sprintf(res, FORMATO, e.mao[0], e.mao[1], e.mao[2], e.mao[3], e.tamanho[0],e.tamanho[1],e.tamanho[2],e.tamanho[3], e.selecao, e.acao, e.selecionar, e.jogar);
	return res;
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
Funçao que distribui as cartas
*/
STATE distribuir(STATE e) {
	/**
	Estado com todas as 52 cartas do baralho
	*/
	long long int ESTADO = 0xfffffffffffff;
	int x,y,n;
	srandom(time(NULL));
	n=0;
	while (n<13) {
		x = random() % 4;
		y = random() % 13;
		if (carta_existe(ESTADO,x,y)) {
			e.mao[0] = add_carta(e.mao[0],x,y);
			ESTADO = rem_carta(ESTADO,x,y);
			n ++;
			e.tamanho[0] += 1;
		}
	}
	n=0;
	while (n<13) {
		x = random() % 4;
		y = random() % 13;
		if (carta_existe(ESTADO,x,y)) {
			e.mao[1] = add_carta(e.mao[1],x,y);
			ESTADO = rem_carta(ESTADO,x,y);
			n++;
			e.tamanho[1] += 1;
		}
	}
	n=0;
	while (n<13) {
		x = random() % 4;
		y = random() % 13;
		if (carta_existe(ESTADO,x,y)) {
		e.mao[2] = add_carta(e.mao[2],x,y);
		ESTADO = rem_carta(ESTADO,x,y);
		n ++;
		e.tamanho[2] += 1;
		}
	}
	e.mao[3] = ESTADO;
	e.tamanho[3] = 13;
	return e;
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
void imprime_carta(int x, int y, STATE e, int i, int naipe, int valor) {
	char *suit = NAIPES;
	char *rank = VALORES;
	char script[10240];
	if (carta_existe(e.selecao,naipe,valor)) {
		e.selecao = rem_carta(e.selecao, naipe, valor);
	}else { 
		e.selecao = add_carta(e.selecao, naipe, valor);	
	}
	sprintf(script, "%s?%s", SCRIPT, estado2str(e));
	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%c%c.svg\" /></a>\n", script, x, y, BARALHO, rank[valor], suit[naipe]);
}

/*
void imprime_Bcarta(char *path, int x, int y, STATE e, int i) {
	char script[10240];
	sprintf(script, "%s?%s", SCRIPT, estado2str(e));
	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%c%c.svg\" /></a>\n", script, x, y, path, 'c', 'B'); 
}
*/


void imprime_mao(int x, int y, STATE e, MAO mao, int m) {
	int n, v;

	for(v = 0; v < 13; v++) {
		for(n = 0; n < 4; n++)
			if (m==3) {
				if(carta_existe(mao, n, v)) {
					x += 30;
                    if(carta_existe(e.selecao, n , v))	
                        imprime_carta(x, (y-20), e, m, n, v);
                    else imprime_carta(x, y, e, m, n, v);
				}
			} else {
				if(carta_existe(mao, n, v)) {
					x += 30;
                    imprime_carta(x, y, e, m, n, v);
				}	
			}
    }	
}

void imprime_butoes(int x, int y, STATE e, int jv){
	char script[10240];	
	e.acao=1;
	sprintf(script, "%s?%s", SCRIPT, estado2str(e));
	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/baralhar.png\" /></a>\n", script, x, y, BARALHO);
	e.acao=2;
	sprintf(script, "%s?%s", SCRIPT, estado2str(e));
	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/Passa.png\" /></a>\n", script, x+100, y, BARALHO);
	e.acao=3;
	if (jv==1) {
		sprintf(script, "%s?%s", SCRIPT, estado2str(e));
		printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/Jogar_enabled.png\" /></a>\n", script, x+200, y, BARALHO);
	} else {
		printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/Jogar_disabled.png\" /></a>\n", script, x+200, y, BARALHO);
	}
}

int jogadaValida(STATE e, MAO jogadaAnt){

}

/** Funçao que retira as cartas de uma mao caso ja esteja presente na seleçao*/
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

void joga_cartas_cpu (STATE e, MAO selecao) {
	int n,v,i=0;
	int x=10,y=10;
	MAO temp=selecao;
	for (i=0;i<3;i++) {
		y+=120;
		for(v = 0; v < 13; v++) {
			for(n = 0; n < 4; n++) {
				if(carta_existe(e.mao[i], n, v) && jogada_valida(temp)==1) {
					imprime_carta(x,y,e,i,n,v);
					temp=retira_cartas(temp,selecao);
					temp=add_carta(temp,n,v);
					e.mao[i]=rem_carta(e.mao[i],n,v);
				}
				x+=50;
			}
        }
	}
}
	

/**
Esta função está a imprimir o estado em quatro colunas: uma para cada naipe
@param path	o URL correspondente à pasta que contém todas as cartas
@param ESTADO	O estado atual
*/
void imprime(STATE e) {
	//int jv;
	
	printf("<svg height = \"900\" width = \"850\">\n");
	printf("<rect x = \"0\" y = \"0\" height = \"900\" width = \"850\" style = \"fill:#007700\"/>\n"); 
	if (e.acao==3) {
		imprime_mao(500,390,e,e.selecao,4);
		e.mao[3]=retira_cartas(e.mao[3],e.selecao);
		joga_cartas_cpu(e,e.selecao);
		e.selecao=0;
		e.acao=0;
	}
	imprime_mao(10,10,e,e.mao[0],0);
	imprime_mao(10,130,e,e.mao[1],1);
	imprime_mao(10,250,e,e.mao[2],2);
	imprime_mao(10,390,e,e.mao[3],3);

	//jv=jogada_valida(e);
	imprime_butoes(40,510,e,1);

	printf("</svg>\n");
}
/**
Esta função recebe a query que é passada à cgi-bin e trata-a.
Neste momento, a query contém o estado que é um inteiro que representa um conjunto de cartas.
Cada carta corresponde a um bit que está a 1 se essa carta está no conjunto e a 0 caso contrário.
Caso não seja passado nada à cgi-bin, ela assume que todas as cartas estão presentes.
@query A query que é passada à cgi-bin
 */
void parse(STATE e) {

	if(e.mao[0] == 0) {
		e = distribuir(e);
	}
	imprime(e);
}

/**
Função principal do programa que imprime os cabeçalhos necessários e depois disso invoca
a função que vai imprimir o código html para desenhar as cartas
 */
int main() {

	STATE e;
	int i;
    	for (i=0;i<4;i++) {
	    	e.mao[i]=0;
    		e.tamanho[i]=0;
    	}
	e.selecao=0x000000000000;
	e.acao = e.jogar = e.selecionar = 0;
	
	printf("Content-Type: text/html; charset=utf-8\n\n");
	printf("<header><title>Big Two</title></header>\n");
	printf("<body>\n");
		
	printf("<h1>BIG TWO</h1>\n");
	
	if(strlen(getenv("QUERY_STRING")) != 0){
    		e = str2estado(getenv("QUERY_STRING"));
    	}	
	if(e.acao == 1){
		for (i=0;i<4;i++) {
	    		e.mao[i]=0;
    			e.tamanho[i]=0;
    			e.acao=0;
    			e.selecao=0;
    		}
	}

	parse(e);
		
/* * Ler os valores passados à cgi que estão na variável ambiente e passá-los ao programa
 */

	printf("</body>\n");
	return 0;
}

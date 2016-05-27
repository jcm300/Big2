#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "parteGrafica.c"
#include "jogaCartas.c"
#include "comparaCartas.c"

#define SCRIPT		"http://127.0.0.1/cgi-bin/cartas"
#define BARALHO		"http://127.0.0.1/cards"
#define BOTOES		"http://127.0.0.1/botoes"

#define NAIPES		"DCHS"
#define VALORES		"3456789TJQKA2"
#define FORMATO 	"%lld_%lld_%lld_%lld_%d_%d_%d_%d_%d_%d_%d_%d_%lld_%d_%d_%d_%lld_%d"
/**
Tipo MAO defenido para representar long long int
*/
typedef long long int MAO;

/**
Guarda o estado do jogo
@param mão cada uma das 4 mãos
@param tamanho o número de cartas em cada mão
@param selecao cartas selecionadas pelo jogador
@param acao se o jogador carregou em algum botão (e.g., jogar ou passar ou baralhar)
@param passar número de jogadores que passaram consecutivamente
@param ultimo_jogador o último jogador que jogou
@param ultima_jogada a última jogada jogada
*/
typedef struct {
  	MAO mao[4];
  	int tamanho[4], pontos[4];
  	MAO selecao;
  	int acao, passar, ultimo_jogador, ordem;
  	MAO ultima_jogada;
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

/**
Função que distribui as cartas
@param e situação atual do jogo 
@return estado após a distribuição das cartas aos players
*/
STATE distribuir(STATE e) {
	/**
	Estado com todas as 52 cartas do baralho
	*/
	long long int ESTADO = 0xfffffffffffff;
	int x,y, i;
	srandom(time(NULL));
	
	for(i = 0; i < 3; i ++){
		while (e.tamanho[i]<13){
			x = random() % 4;
			y = random() % 13;
			if (carta_existe(ESTADO,x,y)) {
				e.mao[i] = add_carta(e.mao[i],x,y);
				ESTADO = rem_carta(ESTADO,x,y);
				e.tamanho[i] += 1;
			}
		}
	}
	e.mao[3] = ESTADO;
	e.tamanho[3] = 13;
	return e;
}

/**
Esta função está encarregue de imprimir o estado do jogo tendo em conta certos aspetos do mesmo. 
@param STATE	O estado atual
*/
void imprime(STATE e) {
	int jv;
	int i;
	
	printf("<svg height = \"600\" width = \"1050\">\n");
	printf("<rect x = \"0\" y = \"0\" height = \"600\" width = \"1050\" style = \"fill:#007700\"/>\n"); 

	printf("<text x=\"750\" y=\"75\" fill=\"black\" font-size=\"50\">Big Two</text>\n");
	
	
	if (e.tamanho[0]==13 && e.tamanho[1]==13 && e.tamanho[2]==13 && e.tamanho[3]==13) {
		e=joga_fst_cpu(e);
		if(e.ultimo_jogador == 3){
			e.selecao = add_carta(e.selecao, 0, 0);
		}
		else e = joga_cpu(e);
	}
	
	if(e.acao == 4){
		if(carta_existe(e.mao[3], 0, 0)){
			e=sugereJogada (e, 1); 
		}
		else e=sugereJogada(e, 0);
		e.acao = 0;
	}

	if (e.acao==3) {
		e.ultima_jogada=e.selecao;
		e.mao[3]=retira_cartas(e.mao[3],e.ultima_jogada);
		e.tamanho[3]=nroCartas(e.mao[3]);
		e.ultimo_jogador=3;
		e.passar=0;
		e.acao=0;
		imprime_mao(500,390,e,e.selecao,4);
		e.selecao=0;
		if(e.tamanho[3] != 0) 
			e=joga_cpu(e);
	}
	
	if (e.acao==2 && !carta_existe(e.mao[3],0,0)) {
		e.passar++;
		printf("<text x=\"500\" y=\"450\" fill=\"white\" font-size=\"20\">Passou</text>\n");
		e.ultimo_jogador=3;
		e.acao=0;
		e=joga_cpu(e);
	}
	
	for(i = 0; i < 3; i ++) imprime_mao(10,10+ 120*i,e,e.mao[i], i);
	imprime_mao(10,390,e,e.mao[3],3);

	if (!(e.tamanho[0] && e.tamanho[1] && e.tamanho[2] && e.tamanho[3])) {
		e.acao = 10;
		e = fim(e);
		printf("</svg>\n");
		return ;
	}
	
	jv=jogadaValida(e.ultima_jogada, e.selecao,e.passar);
	imprime_butoes(40,510,e,jv);


	printf("</svg>\n");
}
/**
Função encarregue de distribuir as cartas, caso estas não tenham sido já distribuídas, e de chamar a funçao que faz o jogo "correr"
@param e Estado do jogo
 */
void parse(STATE e) {
	int i;

	if(e.mao[0] == 0 && e.acao == 0) {
		e=distribuir(e);
	}
	for(i=0;i<4;i++)
		e.tamanho[i]=nroCartas(e.mao[i]);
	imprime(e);
}

/**
Faz o reset das pontuações e inicia o jogo de novo
@param e Estado do jogo que será reiniciado
*/
void novoJogo(STATE *e){
	int i;

	for (i=0;i<4;i++) {
		e->mao[i]=0;
		e->tamanho[i]=0;
		e->pontos[i]=0;
	}
	e->ordem=e->acao=e->ultimo_jogador=e->passar=0;
	e->selecao=e->ultima_jogada=0;
}

/**
Função principal do programa que imprime os cabeçalhos necessários e depois disso invoca
a função que vai "gerir" o jogo
 */
int main() {

	STATE e;
	int i;
    
    novoJogo(&e);
	
	printf("Content-Type: text/html; charset=utf-8\n\n");
	printf("<header><title>Big Two</title></header>\n");
	printf("<body>\n");
			
	if(strlen(getenv("QUERY_STRING")) != 0){
    		e = str2estado(getenv("QUERY_STRING"));
    	}	
	if(e.acao == 1){
		for (i=0;i<4;i++) {
	    		e.mao[i]=0;
    			e.tamanho[i]=0;
    			}
		e.acao=e.ultimo_jogador=e.passar=0;
		e.selecao=e.ultima_jogada=0;
	}
	else if(e.acao == 5){
		novoJogo(&e);
	}
	parse(e);

	printf("</body>\n");
	return 0;
}
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas.h"
#include "funcoesBasicas.h"
#include "parteGrafica.h"
#include "comparaCartas.h"
#include "jogaCartas.h"

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

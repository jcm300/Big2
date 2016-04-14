#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define SCRIPT		"http://127.0.0.1/cgi-bin/cartas"
#define BARALHO		"http://127.0.0.1/cards"
#define BOTOES		"http://127.0.0.1/botoes"

#define NAIPES		"DCHS"
#define VALORES		"3456789TJQKA2"
#define FORMATO 	"%lld_%lld_%lld_%lld_%d_%d_%d_%d_%lld_%d_%d_%d_%lld_%d"
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
  	int tamanho[4];
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
	sscanf(str, FORMATO, &e.mao[0], &e.mao[1], &e.mao[2], &e.mao[3], &e.tamanho[0], &e.tamanho[1], &e.tamanho[2],&e.tamanho[3], &e.selecao, &e.acao, &e.passar, &e.ultimo_jogador, &e.ultima_jogada, &e.ordem);
	return e;
}

/**
Passa o estado STATE para string 
*/
char* estado2str (STATE e){
	static char res[10240];
	sprintf(res, FORMATO, e.mao[0], e.mao[1], e.mao[2], e.mao[3], e.tamanho[0],e.tamanho[1],e.tamanho[2],e.tamanho[3], e.selecao, e.acao, e.passar, e.ultimo_jogador, e.ultima_jogada, e.ordem);
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
*/
int nroCartas(MAO m){
	int r = 0;

	while(m > 0){
		if(m%2 == 1) r ++;
		m /= 2;
	}
	return r;	
}

/**
Função que distribui as cartas
*/
void distribuir(STATE* e) {
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
			e->mao[0] = add_carta(e->mao[0],x,y);
			ESTADO = rem_carta(ESTADO,x,y);
			n++;
			e->tamanho[0] += 1;
		}
	}
	n=0;
	while (n<13) {
		x = random() % 4;
		y = random() % 13;
		if (carta_existe(ESTADO,x,y)) {
			e->mao[1] = add_carta(e->mao[1],x,y);
			ESTADO = rem_carta(ESTADO,x,y);
			n++;
			e->tamanho[1] += 1;
		}
	}
	n=0;
	while (n<13) {
		x = random() % 4;
		y = random() % 13;
		if (carta_existe(ESTADO,x,y)) {
		e->mao[2] = add_carta(e->mao[2],x,y);
		ESTADO = rem_carta(ESTADO,x,y);
		n ++;
		e->tamanho[2] += 1;
		}
	}
	e->mao[3] = ESTADO;
	e->tamanho[3] = 13;
}

/**
Imprime o html correspondente a uma carta
@param x A coordenada x da carta
@param y A coordenada y da carta
@param STATE	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
*/
void imprime_carta(int x, int y, STATE e, int naipe, int valor) {
	char *suit = NAIPES;
	char *rank = VALORES;
	char script[10240];
	if (carta_existe(e.mao[3],naipe,valor)) {
		if (carta_existe(e.selecao,naipe,valor)) {
			e.selecao = rem_carta(e.selecao, naipe, valor);
		}else { 
			e.selecao = add_carta(e.selecao, naipe, valor);	
		}
	}
	sprintf(script, "%s?%s", SCRIPT, estado2str(e));
	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%c%c.svg\" /></a>\n", script, x, y, BARALHO, rank[valor], suit[naipe]);
}

/** Imprime o html correspondente a uma carta de "costas" */
void imprime_carta_costas(int x, int y, STATE e) {
	char script[10240];
	sprintf(script, "%s?%s", SCRIPT, estado2str(e));
	printf("<image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%c%c.jpg\" />\n", x, y, BARALHO, 'c', 'B'); 
}


/**
Imprime uma mão consoante o x e o y
*/
void imprime_mao(int x, int y, STATE e, MAO mao, int m) {
	int n, v;

	if(!(e.ordem)){
		for(v = 0; v < 13; v++) {
			for(n = 0; n < 4; n++)
				if (m==3) {
					if(carta_existe(mao, n, v)) {
						x += 30;
						if(carta_existe(e.selecao, n , v)) imprime_carta(x, (y-20), e, n, v);
						else imprime_carta(x, y, e, n, v);
					}
				} else {
					if(carta_existe(mao, n, v)) {
						x += 30;
						imprime_carta(x, y, e, n, v);
					}	
				}
		    }	
	}
	
	else {
		for(n = 0; n < 4; n++) {
			for(v = 0; v < 13; v++)
				if (m==3) {
					if(carta_existe(mao, n, v)) {
						x += 30;
						if(carta_existe(e.selecao, n , v)) imprime_carta(x, (y-20), e, n, v);
						else imprime_carta(x, y, e, n, v);
					}
				} else {
					if(carta_existe(mao, n, v)) {
						x += 30;
						imprime_carta(x, y, e, n, v);
					}	
				}
		}
	}
}

/**
Imprime uma mão de "costas" consoante o x e o y
*/
void imprime_mao_costas(int x, int y, STATE e, MAO mao) {
	int n, i;
	n = nroCartas(mao);
	for(i = 0; i < n; i++) {
		x += 30;
		imprime_carta_costas(x, y, e);
	}		
}

/**
Imprime os butões baralhar, passar e jogar
*/
void imprime_butoes(int x, int y, STATE e, int jv){
	char script[10240];	
	e.acao=1;
	sprintf(script, "%s?%s", SCRIPT, estado2str(e));
	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/baralhar.png\" /></a>\n", script, x, y, BOTOES);
	e.acao=2;
	sprintf(script, "%s?%s", SCRIPT, estado2str(e));
	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/Passa.png\" /></a>\n", script, x+100, y, BOTOES);
	e.acao=3;
	if (jv) {
		sprintf(script, "%s?%s", SCRIPT, estado2str(e));
		printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/Jogar_enabled.png\" /></a>\n", script, x+200, y, BOTOES);
	} else {
		printf("<image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/Jogar_disabled.png\" />\n", x+200, y, BOTOES);
	}
	e.acao = 4;
	sprintf(script, "%s?%s", SCRIPT, estado2str(e));
	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/Sugestao.png\" /></a>\n", script, x+400, y, BOTOES);
	e.ordem = !(e.ordem);
	sprintf(script, "%s?%s", SCRIPT, estado2str(e));
	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/Ordenar.png\" /></a>\n", script, x + 300, y, BOTOES);
}


void fim(STATE *e){

	int x = 40;
	int y =510;
	int i;
	char script[10240];	

	printf("<h1>Jogador Pontuação </h1>\n");
	for(i = 0; i < 3; i ++){
		if(e->tamanho[i] <= 9 && e->tamanho[i] > 0)
			printf("<p>%d  %d\n</p>\n", i, -e->tamanho[i]);
		else if(e->tamanho[i] >= 10 && e->tamanho[i] <= 12)
			printf("<p>%d  %d\n</p>\n", i, (-2)* e->tamanho[i]);
		else if(e->tamanho[i] == 13)
		printf("<p>%d  %d\n</p>\n", i, (-3) * e->tamanho[i]);
		else printf("<p>%d  Winner</p>\n", i);
	}
	if(e->tamanho[i] <= 9 && e->tamanho[i] > 0)
		printf("<p>Jogador  %d\n</p>\n",  -e->tamanho[i]);
	else if(e->tamanho[i] >= 10 && e->tamanho[i] <= 12)
		printf("<p>Jogador  %d\n</p>\n",  (-2)* e->tamanho[i]);
	else if(e->tamanho[i] == 13)
		printf("<p>Jogador  %d\n</p>\n",  (-3) * e->tamanho[i]);
	else printf("<p>Jogador  Winner</p>\n" );
	e->acao=1;
	sprintf(script, "%s?%s", SCRIPT, estado2str(*e));
	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/baralhar.png\" /></a>\n", script, x, y, BOTOES);
}

/**
Compara duas mãos por forma a validar a jogadaAtual em relação a jogadaAnt
*/
int comparaMaos(MAO jogadaAnt, MAO jogadaAtual){
	int c  = 0, v1 = 0, v2 = 0, n1 = 0, n2 = 0;

	while(jogadaAnt > 0){
		if(c > 12){
			c = 0;
			n1 ++;
		}
		if(jogadaAnt%2 == 1)
			v1 = c;
		
		jogadaAnt /=2;
		c ++;
	}
	c = 0;
	while(jogadaAtual > 0){
		if(c > 12){
			c = 0;
			n2 ++;
		}
		if(jogadaAtual%2 == 1)
			v2 = c;
		
		jogadaAtual /=2;
		c ++;
	}
	if(v1 == v2) return (n1 < n2);
	else return (v1<v2);
}

/**Verifica se as cartas selecionadas pelo jogador são do mesmo valor*/
int cartasDiferentes(MAO jogadaAtual){
	int c = 0, ct;	

	while(jogadaAtual % 2 != 1 && jogadaAtual > 0){
		if(c > 12)
			c = 0;
		jogadaAtual /= 2;
		c ++;
	} 
	ct = c + 1;
	jogadaAtual /= 2;
	while(jogadaAtual > 0){
		if(ct > 12)
			ct = 0;		
		if(jogadaAtual%2 == 1 && ct != c) return 0;
		jogadaAtual /=2;
		ct ++;
	}
	return 1;
}

/**Função encarregue de verificar a validade da seleção do jogador*/
int jogadaValida(MAO jogadaAnt, MAO jogadaAtual, int passar){

	int nroAg, nroAnt;
	nroAnt = nroCartas(jogadaAnt);
	nroAg = nroCartas(jogadaAtual);

	if(nroAg <= 0 || nroAg >=4) return 0;
	else if(!(cartasDiferentes(jogadaAtual))) return 0;	
	else if(passar >=3) return 1;
	else if(nroAnt == 0) return 1;
	else if(nroAnt != nroAg) return 0;
	else if(comparaMaos(jogadaAnt, jogadaAtual)) return 1;
	else return 0;
}


/** Função que retira as cartas de uma mão caso já esteja presente na seleção*/
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
Realiza uma jogada tendo em conta a mão e a ultima_jogada
*/
void joga_cartas_cpu (STATE *e, int y) {
	int n, v;
	int x = 500;
	int nro = nroCartas(e->ultima_jogada);
	int nt;
	MAO temp;
				
	for(n = 0; n < 4; n ++){
		for(v = 0; v < 13; v ++){
			temp = 0x0000000000000;
			if(carta_existe(e->mao[e->ultimo_jogador], n, v)){
				temp = add_carta(temp, n, v);
				if((e->passar) >= 3){
					imprime_mao(x,y,*e,temp,4);
					(e->ultima_jogada) = temp;	
					(e->passar) = 0;
					break;
				}
				else if(comparaMaos(e->ultima_jogada, temp)){
					 nt = 0;
					 while(nroCartas(temp) < nro && nt < 4){
						if(carta_existe(e->mao[(e->ultimo_jogador)], nt, v) && nt != n)
							temp = add_carta(temp, nt, v);
						nt ++;			
					 }
					 if(nroCartas(temp) == nro){
						imprime_mao(x, y, *e, temp, 4);
						(e->ultima_jogada)= temp;
						(e->passar) = 0;
						break;	
					 }
				}
			}
		}
	}

	if (n==4 && v==13) {
		(e->passar) += 1;
	}
}

/**
Função encarregue de encontrar uma possível jogada para player
*/
void sugereJogada (STATE *e) {
	int n, v;
	int x = 10;
	int y = 600;
	int nro = nroCartas(e->ultima_jogada);
	int nt;
	MAO temp;
				
	for(n = 0; n < 4; n ++){
		for(v = 0; v < 13; v ++){
			temp = 0x0000000000000;
			if(carta_existe(e->mao[3], n, v)){
				temp = add_carta(temp, n, v);
				if(e->passar >= 3){
					imprime_mao(x,y,*e,temp,v);
					break;
				}	
				else if(comparaMaos(e->ultima_jogada, temp)){
					 nt = 0;
					 while(nroCartas(temp) < nro && nt < 4){
						if(carta_existe(e->mao[3], nt, v) && nt != n)
							temp = add_carta(temp, nt, v);
						nt ++;			
					 }
					 if(nroCartas(temp) == nro){
						imprime_mao(x, y, *e, temp, v);
						break;	
					 }
				}
			}
		}
	}
}


/**
Se os cpus tiverem o 3 de ouros esta função joga o 3 de ouros
*/	
void joga_fst_cpu (STATE *e) {
	int y,i;
	for (y=10, i=0; i<3; i++, y+=120) {
		if (carta_existe(e->mao[i],0,0)) {
			imprime_carta(530,y,*e,0,0);
			e->ultima_jogada=add_carta(e->ultima_jogada,0,0);
			break;
		}
	}
	e->ultimo_jogador=i;
	e->mao[e->ultimo_jogador]=retira_cartas(e->mao[e->ultimo_jogador],e->ultima_jogada);
	e->tamanho[e->ultimo_jogador]=nroCartas(e->mao[e->ultimo_jogador]);
}

/**
Função encarrege de fazer os cpus jogar
*/
void joga_cpu (STATE *e) {
	if(e->ultimo_jogador == 3){
		e->ultimo_jogador=0;
		joga_cartas_cpu(e,10);
		e->mao[0]=retira_cartas(e->mao[0],e->ultima_jogada);
		e->tamanho[0]=nroCartas(e->mao[0]);
		if(e->tamanho[0] == 0){
			e->ultimo_jogador = 4;
		}
	}

	if (e->ultimo_jogador==0) {
		e->ultimo_jogador=1;
		joga_cartas_cpu(e,130);
		e->mao[1]=retira_cartas(e->mao[1],e->ultima_jogada);
		e->tamanho[1]=nroCartas(e->mao[1]);
		if(e->tamanho[1] == 0){
			e->ultimo_jogador = 4;
		}
	}

	if (e->ultimo_jogador==1) {
		e->ultimo_jogador=2;
		joga_cartas_cpu(e,250);
		e->mao[2]=retira_cartas(e->mao[2],e->ultima_jogada);
		e->tamanho[2]=nroCartas(e->mao[2]);
		if(e->tamanho[2] == 0){
			e->ultimo_jogador = 4;
		}
	}

}
/**
Esta função está encarregue de imprimir o estado do jogo tendo em conta certos aspetos do mesmo. 
@param STATE	O estado atual
*/
void imprime(STATE *e) {
	int jv;
	
	if (e->tamanho[0]==0 || e->tamanho[1]==0 || e->tamanho[2]==0 || e->tamanho[3]==0) {
		e->acao = 0;
		fim(e);
		return ;
	}
	else{
		printf("<svg height = \"900\" width = \"1050\">\n");
		printf("<rect x = \"0\" y = \"0\" height = \"900\" width = \"1050\" style = \"fill:#007700\"/>\n"); 
		
		if (e->tamanho[0]==13 && e->tamanho[1]==13 && e->tamanho[2]==13 && e->tamanho[3]==13) {
			joga_fst_cpu(e);
			if(e->ultimo_jogador == 3){
				e->selecao = add_carta(e->selecao, 0, 0);
			}
			else joga_cpu(e);
		}
		
		if(e->acao == 4){
			sugereJogada (e); 
			e->acao = 0;
		}
	
		if (e->acao==3) {
			imprime_mao(500,390,*e,e->selecao,4);
			e->ultima_jogada=e->selecao;
			e->mao[3]=retira_cartas(e->mao[3],e->ultima_jogada);
			e->tamanho[3]=nroCartas(e->mao[3]);
			e->ultimo_jogador=3;
			e->selecao=0;
			e->acao=0;
			e->passar=0;
			joga_cpu(e);
		}
		else if (e->acao==2) {
			e->passar++;
			e->ultimo_jogador=3;
			e->acao=0;
			joga_cpu(e);
		}

		imprime_mao(10,10,*e,e->mao[0], 0);
		imprime_mao(10,130,*e,e->mao[1],1);
		imprime_mao(10,250,*e,e->mao[2],2);
		imprime_mao(10,390,*e,e->mao[3],3);


		jv=jogadaValida(e->ultima_jogada, e->selecao,e->passar);
		imprime_butoes(40,510,*e,jv);

		printf("</svg>\n");
	}
}
/**
Função encarregue de distribuir as cartas, caso estas não tenham sido já distribuídas, e de chamar a funçao que faz o jogo "correr"
 */
void parse(STATE *e) {

	if(e->mao[0] == 0) {
		distribuir(e);
	}

	imprime(e);
}

/**
Função principal do programa que imprime os cabeçalhos necessários e depois disso invoca
a função que vai "gerir" o jogo
 */
int main() {

	STATE e;
	int i;
    	for (i=0;i<4;i++) {
	    	e.mao[i]=0;
    		e.tamanho[i]=0;
    	}
	e.selecao=0x000000000000;
	e.ultima_jogada=0x000000000000;
	e.ordem=e.acao = e.passar = e.ultimo_jogador = 0;
	
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
    			}
		e.acao=e.ultimo_jogador=e.passar=0;
		e.selecao=e.ultima_jogada=0;
	}

	parse(&e);

	printf("</body>\n");
	return 0;
}

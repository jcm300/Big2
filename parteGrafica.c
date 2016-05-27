/**
Imprime o html correspondente a uma carta
@param x A coordenada x da carta
@param y A coordenada y da carta
@param e O estado atual
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

/** Imprime o html correspondente a uma carta de "costas" 
@param x A coordenada x da carta
@param y A coordenada y da carta
@param e O estado atual
*/
void imprime_carta_costas(int x, int y, STATE e) {
	char script[10240];
	sprintf(script, "%s?%s", SCRIPT, estado2str(e));
	printf("<image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%c%c.jpg\" />\n", x, y, BARALHO, 'c', 'B'); 
}

/**
Imprime uma mão consoante o x e o y
@param x A coordenada x da carta
@param y A coordenada y da carta
@param e O estado atual
@param mao mao a imprimir
@param m indica se a mao e referente ao jogador ou aos bots
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
Imprime uma mão de "costas" partindo nas coordenadas fornecidas 
@param x A coordenada x da carta
@param y A coordenada y da carta
@param e O estado atual
@param mao mao a imprimir
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
Imprime os butões baralhar, passar e jogar e sugestão
@param x A coordenada x do primeiro botão
@param y A coordenada x do primeiro botão
@param e Estado atual do jogo	
@param jv Variável que indica o estado da jogada do jogador(se está é válida ou não)
*/
void imprime_butoes(int x, int y, STATE e, int jv){
	char script[10240];	
	e.acao=1;
	sprintf(script, "%s?%s", SCRIPT, estado2str(e));
	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/baralhar.png\" /></a>\n", script, x, y, BOTOES);
	e.acao=2;
	sprintf(script, "%s?%s", SCRIPT, estado2str(e));
	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/Passa.png\" /></a>\n", script, x+100, y, BOTOES);
	e.acao = 4;
	sprintf(script, "%s?%s", SCRIPT, estado2str(e));
	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/Sugestao.png\" /></a>\n", script, x+300, y, BOTOES);
	e.acao=3;
	if (jv) {
		sprintf(script, "%s?%s", SCRIPT, estado2str(e));
		printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/Jogar_enabled.png\" /></a>\n", script, x+200, y, BOTOES);
	} else {
		sprintf(script, "%s?%s", SCRIPT, estado2str(e));
		printf("<image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/Jogar_disabled.png\" />\n", x+200, y, BOTOES);
	}
	e.acao=0;
	e.ordem = !(e.ordem);
	sprintf(script, "%s?%s", SCRIPT, estado2str(e));
	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/Ordenar.png\" /></a>\n", script, x +400, y, BOTOES);
}

/**
Função encarregue de registar os scores dos jogadores e de dar continuidade ao jogo
@param e Estado final do jogo 
@return Estado do jogo com as pontuações atualizadas
*/
STATE fim(STATE e){

	int x = 800;
	int y = 80;
	int i;
	char script[10240];	

	printf("<rect x = \"750\" y = \"0\" height = \"600\" width = \"300\" style = \"fill:#007700\"/>\n"); 
	printf("<text x=\"775\" y=\"40\" fill=\"black\" font-size=\"35\">Pontuações</text>\n");
	for(i = 0; i < 4; i ++, y += 120){
		if(e.tamanho[i] == 0)
			printf("<text x=\"%d\" y=\"%d\" fill=\"black\" font-size=\"25\">Winner (%d) </text>\n", x, y , e.pontos[i]);
		else{
			if(e.tamanho[i] <= 9 && e.tamanho[i] > 0)
				e.pontos[i] += -e.tamanho[i];
			else if(e.tamanho[i] >= 10 && e.tamanho[i] <= 12)
				e.pontos[i] += (-2)*e.tamanho[i];
			else if(e.tamanho[i] == 13)
				e.pontos[i] += (-3)* e.tamanho[i];
			printf("<text x=\"%d\" y=\"%d\" fill=\"black\" font-size=\"25\"> %d </text>\n", x+30, y , e.pontos[i]);
		}
	}
	
	
	y -= 75;
	e.acao=1;
	sprintf(script, "%s?%s", SCRIPT, estado2str(e));
	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/baralhar.png\" /></a>\n", script, x-25, y, BOTOES);
	e.acao=5;
	sprintf(script, "%s?%s", SCRIPT, estado2str(e));
	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/NovoJogo.png\" /></a>\n", script, x+75, y, BOTOES);
	return e;
}
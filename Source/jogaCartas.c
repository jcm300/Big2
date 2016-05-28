#include "estruturas.h"

/**
Função encarregue de procurar um Straight válido na mão dos bot's
@param mao Mao do bot onde se irá procurar por um Straight
@param jogadaAnt Última jogada a ser efetuada no jogo
@return Uma mão diferente caso se encontre um Straight válido, 0 caso contrário
*/
MAO jogaStraight(MAO mao, MAO jogadaAnt){
	MAO temp;
	int vt, n, v, nt;

	for(n = 0; n < 4; n ++){
		for(v = 0; v < 13; v ++){
			temp = 0x0000000000000;
			if(carta_existe(mao, n, v)){
				temp = add_carta(temp,n, v); 
				vt = v+1;
				while(nroCartas(temp) < 5 && vt < 13){
					nt = 0;		
					while(nt<4){
						if(carta_existe(mao, nt,vt)){
							temp=add_carta(temp,nt, vt);	
							break;
						}
						nt ++;
					}
					if(nt == 4) break;
					vt ++;
				}
				if(nroCartas(temp) == 5 && comparaStrEStrFlush(temp, jogadaAnt)){
					return temp;
				}
				else break;
			}		
		}
	}		
	return 0;
}


/**
Função encarregue de procurar um Straight Flush válido na mão dos bot's
@param mao Mao do bot onde se irá procurar por um Straight Flush
@param jogadaAnt Última jogada a ser efetuada no jogo
@return Uma mão diferente caso se encontre um Straight Flush válido, 0 caso contrário
*/
MAO jogaStraightFlush(MAO mao, MAO jogadaAnt){
	MAO temp;
	int vt, n, v;

	for(n = 0; n < 4; n ++){
		for(v = 0; v < 13; v ++){
			temp = 0x0000000000000;
			if(carta_existe(mao, n, v)){
				temp = add_carta(temp,n, v); 
				vt = 0;
				while(nroCartas(temp) < 5 && vt < 13){
					if(carta_existe(mao, n, vt) && vt != v)
						temp=add_carta(temp,n, vt);	
					vt ++;			
				}	
				if(nroCartas(temp) == 5 && comparaStrEStrFlush(jogadaAnt, temp)){
					return temp;
				}
				else break;
			}		
		}
	}		
	return 0;
}


/**
Função encarregue de procurar um Quads válido na mão dos bot's
@param mao Mao do bot onde se irá procurar por um Quads
@param jogadaAnt Última jogada a ser efetuada no jogo
@return Uma mão diferente caso se encontre um Quads válido, 0 caso contrário
*/
MAO jogaQuads (MAO mao,MAO jogadaAnt) {
	int n,v;
	MAO temp=0;
	for(v = 0; v < 13; v ++){
		if(carta_existe(mao,0,v) && carta_existe(mao,1,v) && carta_existe(mao,2,v) && carta_existe(mao,3,v)) {
			for (n=0; n<4; n++) {
				temp=add_carta(temp,n,v);
				mao=rem_carta(mao,n,v);
			}
			for(n = 0; n < 4; n ++){
				for(v = 0; v < 13; v ++){
					if(carta_existe(mao,n,v)) {
						temp=add_carta(temp,n,v);
						if(comparaQuads(jogadaAnt,temp)) return temp;
					}
				}
			}
		}
	}
	return 0;
}


/**
Função encarregue de procurar um Flush válido na mão dos bot's
@param mao Mao do bot onde se irá procurar por um Flush
@param jogadaAnt Última jogada a ser efetuada no jogo
@return Uma mão diferente caso se encontre um Flush válido, 0 caso contrário
*/
MAO jogaFlush (MAO mao, MAO jogadaAnt) {
	int n,v,j=0;
	MAO temp=0;
	for(n = 0; n < 4; n ++){
		j=0;
		temp=0;
		for(v = 0; v < 13; v ++){
			if(carta_existe(mao,n,v)){
				j++;
				temp=add_carta(temp,n,v);
			}
			if (comparaFlush(jogadaAnt,temp) && j==5) {
				return temp;
			}
		}
	}
	return 0;
}


/**
Função encarregue de procurar um Full House válido na mão dos bot's
@param mao Mao do bot onde se irá procurar por um Full House 
@param jogadaAnt Última jogada a ser efetuada no jogo
@return Uma mão diferente caso se encontre um Full House válido, 0 caso contrário
*/
MAO jogaFullHouse (MAO mao,MAO jogadaAnt) {
	int n,v,n2,v2,j=0;
	MAO temp=0;
	MAO temp2=0;
	for(v = 0; v < 13; v ++){
		j=0;
		temp=0;
		for(n = 0; n < 4; n ++){
			if(carta_existe(mao,n,v)){
				j++;
				temp=add_carta(temp,n,v);
			}
			if(j==3){
				mao=retira_cartas(mao,temp);
				for(v2 = 0; v2 < 13; v2 ++){
					j=0;
					temp2=temp;
					for(n2 = 0; n2 < 4; n2 ++){
						if(carta_existe(mao,n2,v2)){
							j++;
							temp2=add_carta(temp2,n2,v2);
						}
						if(j==2 && comparaFullHouse(jogadaAnt,temp2)){
							return temp2;
						}
					}
				}
				mao=adiciona_cartas(mao,temp);
			}
		}
	}
	return 0;
}

/**
Função encarregue de jogar uma combinação, se possível, pelos bots
@param e Guarda o estado atual do jogo
@param y Coordenada inicial onde serão impressas as cartas jogadas pelo primeiro bot
@return Estado do jogo após as jogadas dos bots
*/
STATE jogaComb(STATE e, int y){
	MAO jogadaAjogar  = 0x0000000000000;
	int tjogada = identificaJogada(e.ultima_jogada);
	switch(tjogada){
		case 1:
			jogadaAjogar=jogaQuads(e.mao[e.ultimo_jogador],e.ultima_jogada);
			if (jogadaAjogar==0) jogadaAjogar=jogaStraightFlush(e.mao[e.ultimo_jogador],0);
			break;
		case 2:
			jogadaAjogar=jogaFullHouse(e.mao[e.ultimo_jogador],e.ultima_jogada);
			if (jogadaAjogar==0) jogadaAjogar=jogaQuads(e.mao[e.ultimo_jogador],0);
			if (jogadaAjogar==0) jogadaAjogar=jogaStraightFlush(e.mao[e.ultimo_jogador],0);
			break;
		case 3:
			jogadaAjogar=jogaStraightFlush(e.mao[e.ultimo_jogador],e.ultima_jogada);
			break;
		case 4:
			jogadaAjogar=jogaFlush(e.mao[e.ultimo_jogador],e.ultima_jogada);
			if (jogadaAjogar==0) jogadaAjogar=jogaFullHouse(e.mao[e.ultimo_jogador],0);
			if (jogadaAjogar==0) jogadaAjogar=jogaQuads(e.mao[e.ultimo_jogador],0);
			if (jogadaAjogar==0) jogadaAjogar=jogaStraightFlush(e.mao[e.ultimo_jogador],0);
			break;
		case 5:
			jogadaAjogar=jogaStraight(e.mao[e.ultimo_jogador],e.ultima_jogada);
			if (jogadaAjogar==0) jogadaAjogar=jogaFlush(e.mao[e.ultimo_jogador],0);
			if (jogadaAjogar==0) jogadaAjogar=jogaFullHouse(e.mao[e.ultimo_jogador],0);
			if (jogadaAjogar==0) jogadaAjogar=jogaQuads(e.mao[e.ultimo_jogador],0);
			if (jogadaAjogar==0) jogadaAjogar=jogaStraightFlush(e.mao[e.ultimo_jogador],0);
			break;
	}
	if (jogadaAjogar == 0) {
		e.passar++;
		printf("<text x=\"500\" y=\"%d\" fill=\"white\" font-size=\"20\">Passou</text>\n", y + 60);
	} else {
		imprime_mao(500, y, e, jogadaAjogar, 4);
		e.mao[e.ultimo_jogador]=retira_cartas(e.mao[e.ultimo_jogador],jogadaAjogar);
		e.ultima_jogada=jogadaAjogar;
		e.passar=0;
	}
	return e;
}


/**
Função que realiza as jogadas de 1, 2 e 3 cartas pelos bots
@param e Guarda o estado do jogo
@param y Coordenada y onde será impressa a jogada do bot
@return Estado do jogo após o término do turno do bot
*/
STATE joga_cartas_cpu (STATE e, int y) {
	int n, v;
	int x = 500;
	int nro = nroCartas(e.ultima_jogada);
	int nt;
	MAO temp = 0x0000000000000;
				
	for(n = 0; n < 4; n ++){
		for(v = 0; v < 13; v ++){
			temp = 0x0000000000000;
			if(carta_existe(e.mao[(e.ultimo_jogador)], n, v)){
				temp = add_carta(temp, n, v);
				if(e.passar >= 3 || nro == 0){
					e.ultima_jogada = temp;	
					e.passar = 0;
					imprime_mao(x,y,e,temp,4);
					return e;
				}
				if(comparaMaos(e.ultima_jogada, temp)){
					 nt = 0;
					 while(nroCartas(temp) < nro && nt < 4){
						if(carta_existe(e.mao[(e.ultimo_jogador)], nt, v) && nt != n)
							temp = add_carta(temp, nt, v);
							nt ++;			
					 }
					 if(nroCartas(temp) == nro){
						e.passar = 0;
						e.ultima_jogada= temp;
						imprime_mao(x, y, e, temp, 4);
						return e;	
					 }
				}
			}
		}
	}
	e.passar += 1;
	printf("<text x=\"500\" y=\"%d\" fill=\"white\" font-size=\"20\">Passou</text>\n", y + 60);
	return e;
}

/**
Função encarregue de encontrar uma possível jogada de 1,2 e 3 cartas para o player
@param e Guarda o estado atual do jogo
@return Estado do jogo contendo a sugestão dada ao player
*/
STATE sugereDuTri (STATE e) {
	int n, v;
	int nro = nroCartas(e.ultima_jogada);
	int nt;
	MAO temp;
				
	for(n = 0; n < 4; n ++){
		for(v = 0; v < 13; v ++){
			temp = 0x0000000000000;
			if(carta_existe(e.mao[3], n, v)){
				temp = add_carta(temp, n, v);
				if(e.passar >= 3 || nro == 0){
					e.selecao=temp;
					n = 4;
					break;
				}	
				else if(comparaMaos(e.ultima_jogada, temp)){
					 nt = 0;
					 while(nroCartas(temp) < nro && nt < 4){
						if(carta_existe(e.mao[3], nt, v) && nt != n)
							temp = add_carta(temp, nt, v);
						nt ++;			
					 }
					 if(nroCartas(temp) == nro){
						e.selecao=temp;
						n = 4;
						break;	
					 }
				}
			}
		}
	}
	if(!temp)printf("<text x=\"500\" y=\"450\" fill=\"white\" font-size=\"20\">Não há jogada possível</text>\n");
	return e;
}


/**
Função encarregue de encontrar uma possível combinação para o player
@param e Guarda o estado atual do jogo
@return Estado do jogo contendo a sugestão dada ao player
*/
STATE sugereComb(STATE e){
	MAO jogadaAjogar;
	MAO  temp = 0x0000000000000;
	
	switch(identificaJogada(e.ultima_jogada)){
		case 1: jogadaAjogar = jogaQuads(e.mao[3], e.ultima_jogada);
			if(jogadaAjogar == 0) jogadaAjogar = jogaStraightFlush(e.mao[3], temp);
			break;
		case 2:	jogadaAjogar = jogaFullHouse(e.mao[3], e.ultima_jogada);
			if(jogadaAjogar == 0) jogadaAjogar = jogaQuads(e.mao[3], temp);
			if(jogadaAjogar == 0) jogadaAjogar = jogaStraightFlush(e.mao[3], temp);
			break;
		case 3: jogadaAjogar = jogaStraightFlush(e.mao[3], e.ultima_jogada);
			break;
		case 4: jogadaAjogar = jogaFlush(e.mao[3], e.ultima_jogada);
			if(jogadaAjogar == 0) jogadaAjogar = jogaQuads(e.mao[3], temp);
			if(jogadaAjogar == 0) jogadaAjogar = jogaStraightFlush(e.mao[3], temp);
			if(jogadaAjogar == 0) jogadaAjogar = jogaFullHouse(e.mao[3], temp);
			break;
		default:jogadaAjogar = jogaStraight(e.mao[3], e.ultima_jogada); 
			if(jogadaAjogar == 0) jogadaAjogar = jogaFlush(e.mao[3], temp);
			if(jogadaAjogar == 0) jogadaAjogar = jogaQuads(e.mao[3], temp);
			if(jogadaAjogar == 0) jogadaAjogar = jogaStraightFlush(e.mao[3], temp);
			if(jogadaAjogar == 0) jogadaAjogar = jogaFullHouse(e.mao[3], temp);
			break;
	} 
	if(jogadaAjogar != 0){
		e.selecao = jogadaAjogar;
	}else if(e.passar < 3) printf("<text x=\"500\" y=\"450\" fill=\"white\" font-size=\"20\">Não há jogada possível</text>\n");
	return e;
}

/**
Realiza uma sugestão ao player baseada na última jogad, no número de passes consecutivos e na mão atual do player
@param e Estado atual do jogo
@param fst Indica se o player é o primeiro a jogar
@return Estado do jogo após a sugestão dada ao player
*/
STATE sugereJogada(STATE e, int fst){
	if(e.passar >=3 || fst){
		e = sugereComb(e);
		if(e.selecao == 0) e = sugereDuTri(e);
	}
	if(nroCartas(e.ultima_jogada) == 5) e = sugereComb(e);
	else e =sugereDuTri(e);
	
	return e;
}

/**
Função encarregue de gerir as jogadas de combinações por parte dos bots, agindo tendo em conta a última combinação a ser jogada bem como se o bot é o primeiro a jogar
@param ultima_jogada Última jogada efetuada na partida
@param mao Mão do bot
@param fst Indica se o bot é o primeiro a jogar
@return Devolve 0 caso não haja combinação possível ou uma mão caso contrário
*/
MAO joga5CPU(MAO ultima_jogada, MAO mao, int fst) {
	MAO jogadaAjogar;
	jogadaAjogar=jogaStraight(mao,ultima_jogada);
	if (jogadaAjogar==0 || (!carta_existe(jogadaAjogar, 0, 0) && fst)) jogadaAjogar=jogaFlush(mao,ultima_jogada);
	if (jogadaAjogar==0 || (!carta_existe(jogadaAjogar, 0, 0) && fst)) jogadaAjogar=jogaFullHouse(mao,ultima_jogada);
	if (jogadaAjogar==0 || (!carta_existe(jogadaAjogar, 0, 0) && fst)) jogadaAjogar=jogaQuads(mao,ultima_jogada);
	if (jogadaAjogar==0 || (!carta_existe(jogadaAjogar, 0, 0) && fst)) jogadaAjogar=jogaStraightFlush(mao,ultima_jogada);
	return jogadaAjogar;
}


/**
Função encarregue de efetuar as jogadas de 1, 2 e 3 cartas pelos bots
@param mao Mao do bot
@param Jogada a efetuar pelo bot
*/
MAO jogaDuTri(MAO mao){
	int v, n, c;
	MAO jogada;
	
	for(v = 0; v < 13; v ++){
		if(valor_existe(mao, v)){
			jogada = 0x000000000000;
			c = n = 0;
			while(c < 3 && n < 4){
				if(carta_existe(mao, n, v)){
				jogada = add_carta(jogada, n, v);
					c ++;
				}
				n ++;
			}
			if(c >= 2)
				return jogada; 
		}
	}
	return 0;	
}


/**
Função chamada quando o 3 de ouros se encontrar na mão de um dos bots e que realiza a jogada por estes
@param e Estado incial do jogo
@return Estado após a jogada do primeiro bot
*/	
STATE joga_fst_cpu (STATE e) {
	int y,i;
	MAO jogadaAjogar;
	for (y=10, i=0; i<3; i++, y+=120) {
		if (carta_existe(e.mao[i],0,0)) {
			jogadaAjogar = joga5CPU(0, e.mao[i], 1);
			if(jogadaAjogar){
				imprime_mao(500, 10 + i * 120, e, jogadaAjogar, 4);
				e.ultima_jogada= jogadaAjogar;
							}
			else{
				imprime_carta(530,y,e,0,0);
				e.ultima_jogada=add_carta(e.ultima_jogada,0,0);
			}
			break;
		}
	}
	e.ultimo_jogador=i;
	e.mao[e.ultimo_jogador]=retira_cartas(e.mao[e.ultimo_jogador],e.ultima_jogada);
	e.tamanho[e.ultimo_jogador]=nroCartas(e.mao[e.ultimo_jogador]);
	return e;
}


/**
Função que gere a jogada de cada um dos bots
@param e Estado atual do jogo
@param n Identifica o bobot que irá efetuar a jogada
@return Estado do jogo após o turno do bot atual
*/
STATE jogaUmCPU (STATE e, int n) {
	MAO jogadaAjogar;
	e.ultimo_jogador=n;
	if (e.passar>=3){
			jogadaAjogar=joga5CPU(e.ultima_jogada,e.mao[n], 0);
			if (jogadaAjogar==0){
				jogadaAjogar = jogaDuTri(e.mao[n]);
				if(jogadaAjogar==0){
					e = joga_cartas_cpu(e, 10 + 120 * n);
					e.mao[n] = retira_cartas(e.mao[n], e.ultima_jogada);
				}else{
					e.ultima_jogada = jogadaAjogar;
					e.mao[n] = retira_cartas(e.mao[n], e.ultima_jogada);
					imprime_mao(500, 10 + 120 * n, e, e.ultima_jogada, 4);
					e.passar = 0;
				}
			}else {
				imprime_mao(500, 10 + 120 * n, e, jogadaAjogar, 4);
				e.mao[n]=retira_cartas(e.mao[n],jogadaAjogar);
				e.ultima_jogada=jogadaAjogar;
				e.passar=0;
			}
	}else {
		if(nroCartas(e.ultima_jogada) == 5){
			e=jogaComb(e,10 + 120 * n);
		}else {
			e=joga_cartas_cpu(e,10 + 120 * n);
			e.mao[n]=retira_cartas(e.mao[n],e.ultima_jogada);
		}
	}
	e.tamanho[n]=nroCartas(e.mao[n]);
	if(e.tamanho[n] == 0){
		e.ultimo_jogador = 4;
	}
	return e; 
}

/**
Função encarrege de fazer os bots jogar
@param e Estado atual do jogo
@return Estado após os bots realizarem o seu turno
*/
STATE joga_cpu (STATE e) {
	if(e.ultimo_jogador == 3){
		e=jogaUmCPU(e,0);
	}

	if (e.ultimo_jogador==0) {
		e=jogaUmCPU(e,1);
	}

	if (e.ultimo_jogador==1) {
		e=jogaUmCPU(e,2);
	}
	return e;
}

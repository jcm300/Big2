/**
Função que compara dois Flush's e verifica se a jogada é válida
@param jogadaAnt Última mão a ser jogada
@param jogadaAtual Intenção de jogada atual 
@return 1 se a jogada for válida, 0 caso contrário
*/
int comparaFlush(MAO jogadaAnt, MAO jogadaAtual){
	int c  = 0, v1 = 0, v2 = 0, n1 = 0, n2 = 0;

	if(jogadaAnt == 0) return 1;

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
	if(n1 == n2) return (v1 < v2);
	else return (n1<n2);
}

/**
Função encarregue de comparar as jogadas que não são combinações
@param jogadaAnt Última mão a ser jogada
@param jogadaAtual Intenção de jogada atual 
@return 1 se a jogada for válida, 0 caso contrário
*/
int comparaMaos(MAO jogadaAnt, MAO jogadaAtual){
	int c  = 0, v1 = 0, v2 = 0, n1 = 0, n2 = 0;

	if(jogadaAnt == 0) return 1;

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

/**
Função chamada para verificar, caso a jogada em causa não seja uma combinação, se as carta presentes na mesma são todas do mesmo valor
@param jogadaAtual Intenção de jogada do player
@return 1 caso não hajam cartas diferentes, 0 caso contrário 
*/
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

/**
Função que compara dois Quads's e verifica se a jogada é válida
@param jogadaAnt Última mão a ser jogada
@param jogadaAtual Intenção de jogada atual 
@return 1 se a jogada for válida, 0 caso contrário
*/
int comparaQuads(MAO jogadaAnt, MAO jogadaAtual){
	int v  = 0, v1 = 0, v2 = 0;
	
	if(jogadaAnt == 0) return 1;
	
	for(v = 0; v < 13; v ++){
		if(carta_existe(jogadaAnt, 0, v)){
			if(carta_existe(jogadaAnt, 1, v)){
				v1 = v;
				break;
			}
		}
	}
	
	for(v = 0; v < 13; v ++){
		if(carta_existe(jogadaAtual, 0, v)){
			if(carta_existe(jogadaAtual, 1, v)){
				v2 = v;
				break;
			}
		}
	}
	return (v1<v2);
}


/**
Função que compara dois Straight's e Straight Flush' Straight Flush's verifica se a jogada é válida
@param jogadaAnt Última mão a ser jogada
@param jogadaAtual Intenção de jogada atual 
@return 1 se a jogada for válida, 0 caso contrário
*/
int comparaStrEStrFlush(MAO jogadaAtual, MAO jogadaAnt){
	int n1,n2;
	if(jogadaAnt == 0) return 1;
	n1=valor_existe(jogadaAtual,11);
	n2=valor_existe(jogadaAtual,12);
	if(n2) {
		if (n1) jogadaAtual=rem_carta(jogadaAtual,n1,11);
		jogadaAtual=rem_carta(jogadaAtual,n2,12);
	} 
	n1=valor_existe(jogadaAnt,11);
	n2=valor_existe(jogadaAnt,12);
	if(n2) {
		if (n1) jogadaAnt=rem_carta(jogadaAnt,n1,11);
		jogadaAnt=rem_carta(jogadaAnt,n2,12);
	}
	return (comparaMaos(jogadaAnt,jogadaAtual));
}


/**
Função que compara dois Full Houses e verifica se a jogada é válida
@param jogadaAnt Última mão a ser jogada
@param jogadaAtual Intenção de jogada atual 
@return 1 se a jogada for válida, 0 caso contrário
*/
int comparaFullHouse(MAO jogadaAnt, MAO jogadaAtual){
	int v = 0, v1 = 0, v2 = 0, c, n;
	
	if(jogadaAnt == 0) return 1;	
 
	for(v = 0; v < 13; v ++){
		n = c = 0;
		while(n<4){
			if(carta_existe(jogadaAnt, n, v)){
				c ++; 
			}
			n++;
		}
		if(c == 3){
			v1 = v;
			break;
		}
	}

	for(v = 0; v < 13; v ++){
		n = c = 0;
		while(n<4){
			if(carta_existe(jogadaAtual, n, v)){
				c ++; 
			}
			n++;
		}
		if(c == 3){
			v2 = v;
			break;
		}
	}	
	return (v1<v2);
}

/**
Função que identifica se a jogada se trata de uma combinação de entre as diferentes combinações existentes
@param jogada Combinação a identificar
@return Valor de 1 a 5 para uma combinação válida e 0 caso esta não seja válida 
*/
int identificaJogada(MAO jogada){
	int n1, n2, n, v, vt, c;


	/*Identifica um Quads*/
	for(v = 0; v < 13; v ++){
		n = 0;
		while(carta_existe(jogada, n, v) && n < 4) n ++;
		if(n == 4) return 1;
	}
	
	/*Identifica um full house*/
	for(v = 0; v < 13; v ++){
		n = 0;
		c = 0;
		while(n < 4){
			if(carta_existe(jogada, n, v)){
				c ++;
			}
			n ++;
		}
		if(c == 3) return 2;
	}
	
	/*Identifica um straight flush ou um flush*/
	for(n = 0; n < 4; n ++){
		n1 = carta_existe(jogada, n, 11);
		n2 = carta_existe(jogada, n, 12);
		for(v = 0; v < 8; v ++){ 
			if(carta_existe(jogada, n, v)){
				c = 1;
				vt = v + 1;
				while(c < 5 && vt < 13){
					if(carta_existe(jogada, n, vt)) c ++;	
					vt ++;
				}
				if(vt == v+4)
					return 3;
				else if(vt == v+3 && (n1 != n2) && v == 0)
					return 3; 	
				else if(vt == v+2 && n1 && n2 && v == 0)
					return 3;
				else if(c == 5)
					return 4;
			}
		}
	}	
	
	n1 = valor_existe(jogada, 11);
	n2 = valor_existe(jogada, 12);
	/*Identifica um straight*/
	for(v = 0; v < 8; v ++){
		if(valor_existe(jogada, v)){
			vt = v + 1;
			c = 1;
			while(c < 5 && valor_existe(jogada, vt) && vt < 13){
					vt ++;
					c ++;
			}
			if(c == 5) return 5;
			else if(c == 4 && (n1 || n2) && v == 0)
				return 5; 	
			else if(c == 3 && n1 && n2 && v == 0)
				return 5;
		}
	}
	return 0;
}

/**Função encarregue de verificar a validade da seleção do jogador
@param jogadaAnt Última jogada a ser efetuada, se alguma
@param jogadaAtual Intenção de jogada do player
@param passar Número de vezes consecutivas que foi efetuado o passar imediatamente antes do turno do player
@return	1 se a jogada for válida, 0 caso contrário
*/
int jogadaValida(MAO jogadaAnt, MAO jogadaAtual, int passar){

	int nroAg, nroAnt, tipo;
	nroAnt = nroCartas(jogadaAnt);
	nroAg = nroCartas(jogadaAtual);
	tipo = identificaJogada(jogadaAnt);

	if(nroAg <= 0 || nroAg >5 || nroAg == 4) return 0;
	else if(nroAg == 5){ 
		if(nroAnt == 0 && identificaJogada(jogadaAtual)) return 1;
		else if(nroAg != nroAnt && passar < 3) return 0;
		else if(identificaJogada(jogadaAtual) && passar >= 3) return 1;
		else{
			switch(identificaJogada(jogadaAtual)){
				case 1: 
					if(tipo == 3) return 0;
					else if(tipo == 1) return comparaQuads(jogadaAnt, jogadaAtual);
					else return 1;
					break;
				case 2: 
					if(tipo == 5 || tipo ==4) return 1; 
					else if(tipo == 2) return comparaFullHouse(jogadaAnt, jogadaAtual);
					else return 0;
					break;
				case 3:
					if(tipo != 3) return 1;
					else return comparaStrEStrFlush(jogadaAtual, jogadaAnt);
					break;
				case 4:
					if(tipo == 5) return 1; 
					else if(tipo == 4) return comparaFlush(jogadaAnt, jogadaAtual);
					else return 0;
					break;
				case 5:	if(tipo == 5) return comparaStrEStrFlush(jogadaAtual, jogadaAnt);
					else return 0;
					break;
				default:		
					return 0;
					break;
			}
		}
	}
	else if(!(cartasDiferentes(jogadaAtual))) return 0;	
	else if(passar >=3) return 1;
	else if(nroAnt == 0) return 1;
	else if(nroAnt != nroAg) return 0;
	else if(comparaMaos(jogadaAnt, jogadaAtual)) return 1;
	else return 0;
}
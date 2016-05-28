/**
tipo mao definido para representar long long int
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





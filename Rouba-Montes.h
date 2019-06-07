/**@<Rouba-Montes.h>::**/

/* 
** Cabeçalhos das funções implementadas no módulo Rouba-Montes.c
*/

#ifndef ROUBA_MONTES_H
#define ROUBA_MONTES_H

#define TAM 52
#define MAXCARTASMAO 4
#define MAXCARTASMESA 8
#define FIMVETOR -110
#define TAMSTRING 2

typedef struct carta {					// REGISTRO CONTENDO OS DADOS DAS CARTAS //
	int valor;
	int naipe;
} regBaralho;

typedef struct conjunto {				// REGISTRO CONTENDO OS DADOS DOS MONTES //
	int elemento;
	struct conjunto *prox;
} regMonte;

typedef struct pilha {					// REGISTRO CONTENDO OS DADOS DAS PILHAS DOS JOGADORES //
	int tamMonte;
	regMonte *pilhaJogador;
} regPilhaJogador;

typedef struct player {					// REGISTRO CONTENDO OS DADOS DOS JOGADORES //
	int vetor[4];
	int cartasNaMao;
	regPilhaJogador monte;
	int ativo;
} regJogador;

// ENUMERAÇÃO USADA PARA CRIAR OS BARALHOS
enum Naipes {Paus = 1, Copas, Espadas, Ouros};
enum Valores {As = 1, Dois, Tres, Quatro, Cinco, Seis, Sete, Oito, Nove, Dez, Valete, Dama, Rei};

// NOME DOS VALORES E NAIPES DAS CARTAS
char *naipesCartas[] = {"Paus", "Copas", "Espadas", "Ouros"};
char *valorCartas[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

// PROTÓTIPOS DAS FUNÇÕES //

void inicializar(int *jogadorAtual, int *jogadores, int *numBaralhos, int *jogadoresAtivos, regJogador **jogador,
                 regBaralho baralho[], regMonte **pilhaPrincipal, regMonte **listaMesa, int *cartasPilha);
                 
void criarBaralhos(regBaralho baralho[], int numBaralhos, regMonte **pilhaPrincipal, int *cartasPilha);

void distribuirCartas(regJogador **jogador, int jogadores, regMonte **pilhaPrincipal, regMonte **listaMesa, int *cartasPilha);

int checarFimDeJogo(regMonte *pilhaPrincipal, int jogadoresAtivos);

void jogar(int jogadorAtual, int jogadores, int *jogadoresAtivos, regJogador **jogador, regMonte **pilhaPrincipal,
           regMonte **listaMesa, regBaralho baralho[], int *cartasPilha);
           
void checarCartas(int jogadorAtual, int *jogadoresAtivos, regJogador **jogador, regMonte **pilhaPrincipal, int *cartasPilha);

int descartarCarta(int jogadorAtual, regJogador **jogador, regMonte **listaMesa, regBaralho baralho[]);

int roubarMonte(int jogadorAtual, int jogadores, regJogador **jogador, regMonte **listaMesa, regBaralho baralho[]);

void contarMontes(regJogador **jogador, int jogadores, int *maiorMonte);

void testaVencedor(regJogador **jogador, int jogadores, int maiorMonte);

void imprimirCarta(int carta, regBaralho baralho[]);

void imprimirCartasJogador(regJogador **jogador, int jogadorAtual, regBaralho baralho[]);

int selecionarCartaMao(int jogadorAtual, regJogador **jogador, char opcao[], regBaralho baralho[]);

int selecionarCartaMesa(regMonte **listaMesa, char opcao[], regBaralho baralho[]);

int rand_int(int n);

void shuffle(int *array, int n);

void insereLista(regMonte **listaMesa, int x);

void insereMesa(regMonte **listaMesa, regMonte **pilhaPrincipal, int *cartasPilha);

int removeLista(regMonte **listaMesa, int pos);

int buscarLista(regMonte *l, int busca);

void imprimirListaMesa(regMonte *l, regBaralho baralho[]);

void push(regMonte **p, int x);

int pop(regMonte **p);

int topo(regMonte *p);

void imprimirPilha(regMonte *p, regBaralho baralho[]);

void ordenarVetor(int jogadorAtual, regJogador **jogador, int tam);

void removerVetor(int jogadorAtual, regJogador **jogador, int cont);

#endif

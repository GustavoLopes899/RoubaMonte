/**@<Rouba-Montes.c>::**/

/*
	DESENVOLVIDO POR:	Gustavo Lopes Pereira da Silva
						GustavoLopes899@gmail.com
	TODOS OS FLUXOGRAMAS DESTE JOGO ESTÃO ANEXADOS A ESTE ARQUIVO.
*/

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <conio2.h>
#include <string.h>
#include <time.h>
#include "Rouba-Montes.h"

// FLUXOGRAMA MAIN //
int main() {
	int jogadorAtual;						// jogador atual para ser jogar
	int jogadores;							// contador auxiliar para os jogadores
	int numBaralhos;						// numero de baralhos em jogo
	int jogadoresAtivos;					// numero de jogadores ainda ativos no jogo
	int maiorMonte;							// tamanho do maior monte do final do jogo
	int cartasPilha = 0;					// cartas restantes na pilha principal
	regBaralho baralho[TAM];				// vetor que contém as cartas ordenadas por valor/naipe
	regJogador *jogador;					// variável responsável por alocar um vetor de jogadores do tipo regJogador
	regMonte *pilhaPrincipal, *listaMesa;	// variável da pilha principal e da lista da mesa

	// INICIALIZAÇÃO DOS DADOS //
	inicializar(&jogadorAtual, &jogadores, &numBaralhos, &jogadoresAtivos, &jogador, baralho, &pilhaPrincipal, &listaMesa, &cartasPilha);

	// LAÇO DO JOGO //
	while (jogadorAtual < jogadores && jogadoresAtivos != 0) {
		jogar(jogadorAtual, jogadores, &jogadoresAtivos, &jogador, &pilhaPrincipal, &listaMesa, baralho, &cartasPilha);
		jogadorAtual++;
		if (jogadorAtual > jogadores-1) {
			jogadorAtual = 0;
		}
	}
	// FIM DE JOGO //
	system("cls");
	printf("==========================================\n");
	printf("\t\tFim de jogo!!\n");
	printf("==========================================\n\n");
	contarMontes(&jogador, jogadores, &maiorMonte);
	testaVencedor(&jogador, jogadores, maiorMonte);
	free(jogador);
	free(pilhaPrincipal);
	free(listaMesa);
	getch();
}
// FIM FLUXOGRAMA MAIN //

// FLUXOGRAMA INICIALIZAR //
void inicializar(int *jogadorAtual, int *jogadores, int *numBaralhos, int *jogadoresAtivos, regJogador **jogador,
                 regBaralho baralho[], regMonte **pilhaPrincipal, regMonte **listaMesa, int *cartasPilha) {
	setlocale(LC_ALL, "portuguese");
	*jogadorAtual = 0;
	*pilhaPrincipal = NULL;
	do {
		//system("cls");
		//printf("======================================================================\n");
		//printf(">>>>>>>>>>>>>>>>>>>>>>>>>>> ROUBA-MONTES <<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
		//printf("======================================================================\n\n");
		//printf("\t>> INICIALIZAÇÃO DO JOGO <<\n");
		//printf("\n>> Entre com o número de jogadores: ");
		//scanf("%d", jogadores);
		*jogadores = 3;
	} while (*jogadores < 2 || *jogadores > 8);
	if (*jogadores <= 4) {
		*numBaralhos = 1;
	} else {
		*numBaralhos = 2;
	}
	*jogadoresAtivos = *jogadores;
	*jogador = (regJogador *) malloc(*jogadores*sizeof(regJogador));
	for (int i = 0; i < *jogadores; i++) {
		(*jogador)[i].monte.tamMonte = 0;
	}
	criarBaralhos(baralho, *numBaralhos, pilhaPrincipal, cartasPilha);
	distribuirCartas(&(*jogador), *jogadores, pilhaPrincipal, listaMesa, cartasPilha);
}
// FIM FLUXOGRAMA INICIALIZAR //

// FLUXOGRAMA CRIAR.BARALHOS //
void criarBaralhos(regBaralho baralho[], int numBaralhos, regMonte **pilhaPrincipal, int *cartasPilha) {
	regBaralho baralhoAux[TAM];
	regMonte *novo;
	*pilhaPrincipal = NULL;
	int k = 0, vetorAleatorio[TAM];
	for (int i = 1; i <= Rei; i++) {
		for (int j = 1; j <= Ouros; j++) {
			vetorAleatorio[k] = k;
			baralhoAux[k].valor = i;
			baralhoAux[k].naipe = j;
			baralho[k].valor = i;
			baralho[k].naipe = j;
			k++;
		}
	}
	for (int i = 1; i <= numBaralhos; i++) {
		shuffle(vetorAleatorio, 52);
		for (int j = 0; j < 52; j++) {
			novo = (regMonte *) malloc(sizeof(int));
			novo -> elemento = vetorAleatorio[j];
			novo -> prox = *pilhaPrincipal;
			*pilhaPrincipal = novo;
			(*cartasPilha)++;
		}
	}
}
// FIM FLUXOGRAMA CRIAR.BARALHOS //

// FLUXOGRAMA DISTRIBUIR.BARALHOS //
void distribuirCartas(regJogador **jogador, int jogadores, regMonte **pilhaPrincipal, regMonte **listaMesa, int *cartasPilha) {
	*listaMesa = NULL;
	for (int i = 0; i < jogadores; i++) {
		(*jogador)[i].monte.pilhaJogador = NULL;
		(*jogador)[i].ativo = 1;
		for (int j = 0; j < MAXCARTASMAO; j++) {
			(*jogador)[i].vetor[j] = (*pilhaPrincipal) -> elemento;
			pop(pilhaPrincipal);
			(*cartasPilha)--;
		}
		(*jogador)[i].cartasNaMao = 4;
		ordenarVetor(i, &(*jogador), (*jogador)[i].cartasNaMao);
	}
	for (int j = 0; j < MAXCARTASMESA; j++) {
		insereMesa(&(*listaMesa), &(*pilhaPrincipal), cartasPilha);
	}
}
// FIM FLUXOGRAMA DISTRIBUIR.BARALHOS //

// FLUXOGRAMA CHECAR.FIM.DE.JOGO //
int checarFimDeJogo(regMonte *pilhaPrincipal, int jogadoresAtivos) {
	if ((pilhaPrincipal == NULL) && (jogadoresAtivos == 0)) {
		return 1;
	} else {
		return 0;
	}
}
// FIM FLUXOGRAMA CHECAR.FIM.DE.JOGO //

// FLUXOGRAMA JOGAR //
void jogar(int jogadorAtual, int jogadores, int *jogadoresAtivos, regJogador **jogador, regMonte **pilhaPrincipal,
           regMonte **listaMesa, regBaralho baralho[], int *cartasPilha) {
	int opcao, resultado;
	checarCartas(jogadorAtual, &(*jogadoresAtivos), &(*jogador), &(*pilhaPrincipal), cartasPilha);
	if ((*jogador)[jogadorAtual].ativo == 1) {
		do {
			do {
				system("cls");
				printf("======================================================================\n");
				printf(">>>>>>>>>>>>>>>>>>>>>>>>>>> ROUBA-MONTES <<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
				printf("======================================================================\n\n");
				imprimirListaMesa(*listaMesa, baralho);
				printf("\n\n");
				for (int i = 0; i < jogadores; i++) {
					printf("Tam do monte: %d ", (*jogador)[i].monte.tamMonte);
					printf("Topo do monte do jogador %d: ", i+1);
					imprimirPilha(&(*(*jogador)[i].monte.pilhaJogador), baralho);
				}
				printf("\n\t>> Cartas restantes na pilha: %d <<\n", *cartasPilha);
				printf("\n\t\t>> Jogador %d <<\n\n", jogadorAtual+1);
				imprimirCartasJogador(jogador, jogadorAtual, baralho);
				printf("\n\n>> Escolha a opção desejada:\n");
				printf("\n1 -> Descartar carta;");
				printf("\n2 -> Roubar monte;\n");
				printf("\n>> Entre com a opção: ");
				scanf("%d", &opcao);
			} while (opcao != 1 &&  opcao != 2);
			switch(opcao) {
				case 1: {
					resultado = descartarCarta(jogadorAtual, &(*jogador), &(*listaMesa), baralho);
					break;
				}
				case 2: {
					resultado = roubarMonte(jogadorAtual, jogadores, &(*jogador), listaMesa, baralho);
					break;
				}
			}
		} while (!resultado);
	}
}
// FIM FLUXOGRAMA JOGAR //

// FLUXOGRAMA CHECAR.CARTAS //
void checarCartas(int jogadorAtual, int *jogadoresAtivos, regJogador **jogador, regMonte **pilhaPrincipal, int *cartasPilha) {
	if ((*jogador)[jogadorAtual].cartasNaMao == 0) {
		if (*pilhaPrincipal == NULL) {
			(*jogador)[jogadorAtual].ativo = 0;
			(*jogadoresAtivos)--;
		} else {
			for (int j = 0; j < MAXCARTASMAO; j++) {
				(*jogador)[jogadorAtual].vetor[j] = topo(*pilhaPrincipal);
				pop(pilhaPrincipal);
				(*cartasPilha)--;
				(*jogador)[jogadorAtual].cartasNaMao++;
				if (*pilhaPrincipal == NULL) {
					break;
				}
			}
			ordenarVetor(jogadorAtual, &(*jogador), (*jogador)[jogadorAtual].cartasNaMao);
		}
	}
}
// FIM FLUXOGRAMA CHECAR.CARTAS //

// FLUXOGRAMA DESCARTAR.CARTA //
int descartarCarta(int jogadorAtual, regJogador **jogador, regMonte **listaMesa, regBaralho baralho[]) {
	int cont;
	char opcao[TAMSTRING];
	do {
		printf("\nSelecione uma carta pelo valor:  ");
		fflush(stdin);
		gets(opcao);
		cont = selecionarCartaMao(jogadorAtual, &(*jogador), opcao, baralho);
	} while (cont == -1);
	insereLista(&*listaMesa, (*jogador)[jogadorAtual].vetor[cont]);
	((*jogador)[jogadorAtual].cartasNaMao)--;
	removerVetor(jogadorAtual, &(*jogador), cont);
	printf("\n>> Carta adicionada a mesa!");
	getch();
	return 1;
}
// FLUXOGRAMA DESCARTAR.CARTA //

// FLUXOGRAMA ROUBAR.MONTE //
int roubarMonte(int jogadorAtual, int jogadores, regJogador **jogador, regMonte **listaMesa, regBaralho baralho[]) {
	char cartaMao[TAMSTRING], cartaMesa[TAMSTRING];
	int opcao, jogada, cartaTopo, cont1, cont2, roubarJogador;
	regMonte *pilhaAux = NULL;
	do {
		printf("\n>> Deseja roubar:\n");
		printf("\n1 - Carta da mesa;");
		printf("\n2 - Monte de outro jogador.\n");
		printf("\n>> Entre com a opção: ");
		scanf("%d", &opcao);
	} while (opcao < 1 || opcao > 2);
	switch(opcao) {
		case 1: {					// case 1 -> roubar carta da mesa
			do {
				printf("\n>> Selecione uma carta da mão pelo valor:  ");
				fflush(stdin);
				gets(cartaMao);
				cont1 = selecionarCartaMao(jogadorAtual, &(*jogador), cartaMao, baralho);
			} while (cont1 == -1);
			do {
				printf("\n>> Selecione uma carta da mesa pelo valor: ");
				fflush(stdin);
				gets(cartaMesa);
				cont2 = selecionarCartaMesa(&*listaMesa, cartaMesa, baralho);
			} while (cont2 == -1);
			jogada = (*jogador)[jogadorAtual].vetor[cont1];
			cartaTopo = buscarLista(*listaMesa, cont2);
			if (baralho[jogada].valor == baralho[cartaTopo].valor) {
				removeLista(&*listaMesa, cont2);
				push(&(*jogador)[jogadorAtual].monte.pilhaJogador, cartaTopo);
				push(&(*jogador)[jogadorAtual].monte.pilhaJogador, jogada);
				removerVetor(jogadorAtual, &(*jogador), cont1);
				((*jogador)[jogadorAtual].cartasNaMao)--;
				((*jogador)[jogadorAtual].monte.tamMonte) += 2;
				printf("\n>> Jogada efetuada com sucesso! <<");
				getch();
				return 1;
			} else {
				printf("\n>> Não é possivel fazer esta jogada! <<");
				getch();
				return 0;
			}
			break;
		}
		case 2: {					// case 2 -> roubar monte de outro jogador
			do {
				printf("\nSelecione uma carta da mão pelo valor:  ");
				fflush(stdin);
				gets(cartaMao);
				cont1 = selecionarCartaMao(jogadorAtual, &(*jogador), cartaMao, baralho);
			} while (cont1 == -1);
			do {
				jogada = (*jogador)[jogadorAtual].vetor[cont1];
				printf("\n");
				for (int i = 0; i < jogadores; i++) {
					if (i != jogadorAtual) {
						printf("> Jogador %d: Topo -> ", i+1);
						imprimirPilha(&(*(*jogador)[i].monte.pilhaJogador), baralho);
					}
				}
				printf("\n>> Selecione um jogador para roubar o monte da mesa: ");
				scanf("%d", &roubarJogador);
			} while ((roubarJogador-1) < 0 || (roubarJogador-1) > jogadores || (roubarJogador-1) == jogadorAtual);
			cartaTopo = topo(&(*(*jogador)[roubarJogador-1].monte.pilhaJogador));
			if (baralho[jogada].valor == baralho[cartaTopo].valor) {
				((*jogador)[jogadorAtual].monte.tamMonte) += ((*jogador)[roubarJogador-1].monte.tamMonte);
				while ((*jogador)[roubarJogador-1].monte.pilhaJogador != NULL) {
					push(&pilhaAux, topo(&(*(*jogador)[roubarJogador-1].monte.pilhaJogador)));
					pop(&(*jogador)[roubarJogador-1].monte.pilhaJogador);
				}
				(*jogador)[roubarJogador-1].monte.tamMonte = 0;
				while (pilhaAux != NULL) {
					push(&(*jogador)[roubarJogador-1].monte.pilhaJogador, topo(pilhaAux));
					pop(&pilhaAux);
				}
				push(&(*jogador)[jogadorAtual].monte.pilhaJogador, jogada);
				removerVetor(jogadorAtual, &(*jogador), cont1);
				((*jogador)[jogadorAtual].cartasNaMao)--;
				((*jogador)[jogadorAtual].monte.tamMonte)++;
				(*jogador)[roubarJogador-1].monte.pilhaJogador = NULL;
				printf("\n>> Jogada efetuada com sucesso! <<");
				getch();
				return 1;
			} else {
				printf("\n>> Não é possivel fazer esta jogada! <<");
				getch();
				return 0;
			}
			break;
		}
	}
}
// FLUXOGRAMA ROUBAR.MONTE //

// FLUXOGRAMA CONTAR.MONTES //
void contarMontes(regJogador **jogador, int jogadores, int *maiorMonte) {
	*maiorMonte = 0;
	for (int j = 0; j < jogadores; j++) {
		if ((*jogador)[j].monte.tamMonte > *maiorMonte) {
			*maiorMonte = (*jogador)[j].monte.tamMonte;
		}
	}
}
// FIM FLUXOGRAMA CONTAR.MONTES //

// FLUXOGRAMA TESTA.VENCEDOR//
void testaVencedor(regJogador **jogador, int jogadores, int maiorMonte) {
	for (int j = 0; j < jogadores; j++) {
		if ((*jogador)[j].monte.tamMonte == maiorMonte) {
			printf("O jogador %d ganhou, com um monte de tamanho %d.\n", j+1, maiorMonte);
		}
	}
}
// FIM FLUXOGRAMA TESTA.VENCEDOR //

/*------------------------------------------------------------------------------------------------*/
/*						  		FUNÇÕES DE MANIPULAÇÃO DAS CARTAS								  */
/*------------------------------------------------------------------------------------------------*/

// FUNÇÃO QUE IMPRIME UMA CARTA //
void imprimirCarta(int carta, regBaralho baralho[]) {
	int naipe;
	naipe = baralho[carta].naipe % 4;
	switch (naipe) {
		case 0: {
			textbackground(RED);
			printf("%s ", valorCartas[baralho[carta].valor-1]);
			printf("Ouros");
			break;
		}
		case 1: {
			textcolor(BLACK);
			textbackground(WHITE);
			printf("%s ", valorCartas[baralho[carta].valor-1]);
			printf("Paus");
			break;
		}
		case 2: {
			textbackground(RED);
			printf("%s ", valorCartas[baralho[carta].valor-1]);
			printf("Copas");
			break;
		}
		case 3: {
			textcolor(BLACK);
			textbackground(WHITE);
			printf("%s ", valorCartas[(baralho[carta].valor)-1]);
			printf("Espadas");
			break;
		}
	}
	textbackground(BLACK);
	textcolor(WHITE);
}

// FUNÇÃO QUE IMPRIME AS CARTAS DA MÃO DO JOGADOR //
void imprimirCartasJogador(regJogador **jogador, int jogadorAtual, regBaralho baralho[]) {
	for (int i = 0; i < MAXCARTASMAO; i++) {
		if ((*jogador)[jogadorAtual].vetor[i] == FIMVETOR) {
			break;
		} else {
			imprimirCarta((*jogador)[jogadorAtual].vetor[i], baralho);
			printf("   ");
		}
	}
}

int selecionarCartaMao(int jogadorAtual, regJogador **jogador, char opcao[], regBaralho baralho[]) {
	int c = 0;
	if (!strcmp(opcao,"a")) {
		c = 1;
	} else {
		if (!strcmp(opcao,"j")) {
			c = 11;
		} else {
			if (!strcmp(opcao,"q")) {
				c = 12;
			} else {
				if (!strcmp(opcao,"k")) {
					c = 13;
				}
			}
		}	
	}
	if (c == 0) {
		c = atoi(opcao);
	}
	
	for (int i = 0; i < MAXCARTASMAO; i++) {
		if (baralho[(*jogador)[jogadorAtual].vetor[i]].valor == c) {
			return i;
		}
	}
	return -1;
}

int selecionarCartaMesa(regMonte **listaMesa, char opcao[], regBaralho baralho[]) {
	int c = 0, i = 0;
	regMonte *l = *listaMesa;
	if (!strcmp(opcao,"a")) {
		c = 1;
	} else {
		if (!strcmp(opcao,"j")) {
			c = 11;
		} else {
			if (!strcmp(opcao,"q")) {
				c = 12;
			} else {
				if (!strcmp(opcao,"k")) {
					c = 13;
				}
			}
		}	
	}
	if (c == 0) {
		c = atoi(opcao);
	}
	while (l != NULL) {
		if (baralho[l -> elemento].valor == c) {
			return i;
		} else {
			i++;
			l = l -> prox;
		}
	}
	return -1;
}

/*------------------------------------------------------------------------------------------------*/
/*						  		FUNÇÕES DE NÚMEROS ALEÁTORIOS									  */
/*------------------------------------------------------------------------------------------------*/

// O shuffle de Fisher-Yates é um algoritmo para gerar uma permutação aleatória de um conjunto finito de números //
// https://en.wikipedia.org/wiki/Fisher–Yates_shuffle													 //

int rand_int(int n) {
	int limit = RAND_MAX - RAND_MAX % n;
	int rnd;
	do {
		rnd = rand();
	} while (rnd >= limit);
	return rnd % n;
}

void shuffle(int *array, int n) {
	int i, j, tmp;
	srand(time(NULL));
	for (i = n - 1; i > 0; i--) {
		j = rand_int(i + 1);
		tmp = array[j];
		array[j] = array[i];
		array[i] = tmp;
	}
}

/*------------------------------------------------------------------------------------------------*/
/*								FUNÇÕES DE MANIPULAÇÃO DE LISTA									  */
/*------------------------------------------------------------------------------------------------*/

void insereLista(regMonte **listaMesa, int x) {
	regMonte *atual, *novo, *anterior;
    novo = (regMonte *) malloc(sizeof(int));
    atual = *listaMesa;
    anterior = NULL;   
    novo->elemento = x;
    
    if(atual == NULL) {
        novo -> prox = NULL;
        *listaMesa = novo;
    } else {
        while(atual != NULL && atual->elemento < x){
            anterior = atual;
            atual = atual -> prox;
        }
        novo -> prox = atual;
        if(anterior == NULL){
            *listaMesa = novo;
        } else {
            anterior->prox = novo;
        }
    }
}

void insereMesa(regMonte **listaMesa, regMonte **pilhaPrincipal, int *cartasPilha) {
	regMonte *p = *listaMesa;
	regMonte *novo;
	regMonte *anterior = NULL;
	novo = (regMonte *) malloc(sizeof(int));
	novo -> elemento = (*pilhaPrincipal) -> elemento;
	pop(&*pilhaPrincipal);
	(*cartasPilha)--;
	if (*listaMesa == NULL) {
		novo -> prox = NULL;
		*listaMesa = novo;
	} else {
		while(p != NULL && p -> elemento < novo -> elemento) {
			anterior = p;
			p = p -> prox;
		}
		novo -> prox = p;
		if (anterior == NULL){
    		*listaMesa = novo;
    	} else {
    		anterior -> prox = novo;
    	}
	}
}

int removeLista(regMonte **listaMesa, int pos) {
	regMonte *no, *aux, *anterior;
	int contador = 0, achou = 0;
	no = *listaMesa;
	if (*listaMesa != NULL) {
		while (contador < pos) {
			anterior = no;
			no = no -> prox;
			contador++;
		}
		if (pos == 0) {
			(*listaMesa) = (*listaMesa) -> prox;
		} else {
			aux = no -> prox;
			anterior -> prox = aux;
		}
	}
}

int buscarLista(regMonte *l, int busca) {
	int i = 0;
	if (l == NULL) {
		return -1;
	} else {
		while (l != NULL) {
			if (busca == i) {
				return l -> elemento;
			}
			i++;
			l = l -> prox;
		}
	}
}

void imprimirListaMesa(regMonte *l, regBaralho baralho[]) {
	printf("\t>> Cartas na Mesa:\n\n");
	if (l == NULL) {
		printf("vazia.");
	} else {
		while (l != NULL) {
			imprimirCarta(l -> elemento, baralho);
			l = l -> prox;
			if (l != NULL) {
				printf("   ");
			}
		}
	}
}

/*------------------------------------------------------------------------------------------------*/
/*								FUNÇÕES DE MANIPULAÇÃO DE PILHA									  */
/*------------------------------------------------------------------------------------------------*/

void push(regMonte **p, int x) {
	regMonte *novo;
	novo = (regMonte *) malloc(sizeof(int));
	novo -> elemento = x;
	novo -> prox = *p;
	*p = novo;
}

int pop(regMonte **p) {
	if (p == NULL) {
		return 0;
	} else {
		*p = (*p) -> prox;
		return 1;
	}
}

int topo(regMonte *p) {
	if (p == NULL) {
		return -1;
	} else {
		return p -> elemento;
	}
}

void imprimirPilha(regMonte *p, regBaralho baralho[]) {
	if (p == NULL) {
		printf("vazio.");
	} else {
		imprimirCarta(p -> elemento, baralho);
	}
	printf("\n");
}

/*------------------------------------------------------------------------------------------------*/
/*								FUNÇÕES DE MANIPULAÇÃO DE VETOR									  */
/*------------------------------------------------------------------------------------------------*/

// Ordenação do vetor usando o método de ordenação Selection Sort //

void ordenarVetor(int jogadorAtual, regJogador **jogador, int tam) {
	int i, j, min, swap; 
	for (int i = 0; i < tam - 1; i++) { 
    	min = i; 
    	for (int j = (i+1); j < tam; j++) { 
			if((*jogador)[jogadorAtual].vetor[j] < (*jogador)[jogadorAtual].vetor[min]) { 
        		min = j; 
			} 
    	} 
    	if (i != min) { 
			swap = (*jogador)[jogadorAtual].vetor[i]; 
			(*jogador)[jogadorAtual].vetor[i] = (*jogador)[jogadorAtual].vetor[min]; 
			(*jogador)[jogadorAtual].vetor[min] = swap; 
		} 
	}
}

void removerVetor(int jogadorAtual, regJogador **jogador, int cont) {
	int i = 0;
	while (i < (MAXCARTASMAO - 1)) {
		if (cont == MAXCARTASMAO - 1) {
			(*jogador)[jogadorAtual].vetor[cont] = FIMVETOR;
			break;
		}
		if (i == cont) {
			(*jogador)[jogadorAtual].vetor[i] = (*jogador)[jogadorAtual].vetor[i+1];
			(*jogador)[jogadorAtual].vetor[i+1] = FIMVETOR;
		}
		if ((*jogador)[jogadorAtual].vetor[i] == FIMVETOR) {
			(*jogador)[jogadorAtual].vetor[i] = (*jogador)[jogadorAtual].vetor[i+1];
			(*jogador)[jogadorAtual].vetor[i+1] = FIMVETOR;
		}
		i++;
	}
}

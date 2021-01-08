#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>#include <pthread.h>
#include "timer.h"

void *Bubble(void *arg) {
	const int tamanho = 100000;
	int vetor[tamanho];
	int cont, aux;
	bool troca;
	srand(1);

	//populando o vetor com números aleatorios
	for(cont=0; cont <= tamanho; cont++) {
		vetor[cont]=(rand() % tamanho);
	}

	//ordenando-o por bubblesort:
	do {
		troca = false;
		for(cont=0; cont<= tamanho; cont++) {
			if(vetor[cont] > vetor[cont + 1]) {
				aux = vetor[cont + 1];
				vetor[cont + 1] = vetor[cont];
				vetor[cont] = aux;
				troca = true;
			}
		}
	}
	while(troca == true);

	//liberacao dos recursos que a thread esta a consumir.
	pthread_exit(NULL);
}

int main() {
	int numero = 4;
	double Tinicial, Tfinal, Texec;
	pthread_t th[numero];
	int arg, r;
	
	//registra tempo inicial
	GET_TIME(Tinicial);

	for (r=0; r<numero; r++) {
		//threads sao criadas
		pthread_create(&th[r],NULL,*Bubble,(void *) &arg);
	}
	for (r=0; r<numero; r++) {
		//faz a thread que chamou(main) esperar ate a thread especificada terminar (nesse caso, como se trata de for, espera-se que todas terminem)
		pthread_join(th[r],NULL);
	}
	
	//regista tempo final
	GET_TIME(Tfinal);
	Texec = Tfinal - Tinicial;
	
	printf("Tempo gasto: %.15lf \n", Texec);
	
	return 0;
}

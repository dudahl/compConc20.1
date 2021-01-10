/* Para executar em modo concorrente, deve ser usado 'c' (sem as aspas) como entrada.
 Para executar em modo sequencial, digite qualquer outro caracter ('s', por exemplo)
*/
 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "timer.h"

void BubbleS(void) {
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
	
	return;
}


void *BubbleC(void *arg) {
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
	int arg, r, s;
	char exec;
	

	scanf("%c", &exec);

	if(exec=='c') {
		//registra tempo inicial
		GET_TIME(Tinicial);

		//threads sao criadas
		for (r=0; r<numero; r++) {
			pthread_create(&th[r],NULL,*BubbleC,(void *) &arg);
		}

		//faz a thread que chamou(main) esperar ate a thread especificada terminar (nesse caso, como se trata de for, espera-se que todas terminem)
		for (r=0; r<numero; r++) {
			pthread_join(th[r],NULL);
		}
	
		//regista tempo final
		GET_TIME(Tfinal);
		Texec = Tfinal - Tinicial;
		
		printf("Tempo gasto (concorrente): %.15lf \n", Texec);

	} else {
		//registra tempo inicial
		GET_TIME(Tinicial);
		
		//chamada da funcao de ordenação no modo sequencial 
		for (s=0; s<numero; s++) {
			BubbleS();
		}
	
		//regista tempo final
		GET_TIME(Tfinal);
		Texec = Tfinal - Tinicial;
	
		printf("Tempo gasto (sequencial): %.15lf \n", Texec);
	}
	return 0;

}

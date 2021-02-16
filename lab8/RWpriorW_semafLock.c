/* [semaforo com lock] Codigo baseado no lab5 ("Prioridade para escritores: quando um escritor desejar escrever, nenhum leitor
pode iniciar uma leitura enquanto o escritor nao for atendido") */ 

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define L 2
#define E 2

int leit=0;
int escr=0;
pthread_mutex_t em_e, em_l;
sem_t cond_escr, cond_leit;

void *leitor(void * arg) {
  int *id = (int *) arg;
  while(1) {
    printf("#L%d quer ler\n", *id);

    sem_wait(&cond_leit);
    pthread_mutex_lock(&em_l); 
    leit++;
    if (leit == 1) {
      sem_wait(&cond_escr);
    } pthread_mutex_unlock(&em_l);
    sem_post(&cond_leit);

    //inicio da leitura
    printf("#L%d lendo\n", *id);
    sleep(1);

    pthread_mutex_lock(&em_l); 
    leit--;
    if (leit == 0) {
      sem_post(&cond_escr);
    } pthread_mutex_unlock(&em_l);

    printf("#L%d leu\n", *id);
  }

}

void *escritor(void * arg) {
  int *id = (int *) arg;
  while(1) {
    printf("#E%d quer escrever\n", *id);

    pthread_mutex_lock(&em_e); 
    escr++;
    if (escr == 1) {
      sem_wait(&cond_leit);
    } pthread_mutex_unlock(&em_e);
    sem_wait(&cond_escr);

    //inicio da escrita
    printf("#E%d escrevendo\n", *id);
    sleep(2);

    sem_post(&cond_escr);
    pthread_mutex_lock(&em_e); 
    escr--;
    if (escr == 0) {
      sem_post(&cond_leit);
    } pthread_mutex_unlock(&em_e);

    printf("#E%d escreveu\n", *id);
  }

}

int main(void) {
    //identificadores das threads
    pthread_t tid[L+E];
    int id[L+E];

    //cria as threads leitoras
    for(int i=0; i<L; i++) {
        id[i] = i+1;
        if(pthread_create(&tid[i], NULL, leitor, (void *) &id[i])) exit(-1);
    }

    //cria as threads escritoras
    for(int i=0; i<E; i++) {
        id[i+L] = i+1;
        if(pthread_create(&tid[i+L], NULL, escritor, (void *) &id[i+L])) exit(-1);
    }

    //inicia os semaforos
    sem_init(&cond_escr, 0, 1); //condição
    sem_init(&cond_leit, 0, 1); //condição

    pthread_exit(NULL);
    return 0;
}

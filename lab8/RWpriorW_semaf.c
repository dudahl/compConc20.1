#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define L 2
#define E 2

sem_t em_e, em_l, cond_escr, cond_leit;
int qntdLeit=0, qntdEscr=0;

void * leitor (void * arg) {
    int *id = (int *) arg;
    while(1) {
        printf("Leitora %d quer ler\n", *id);

        sem_wait(&cond_leit);
        sem_wait(&em_l); qntdLeit++;
        if (qntdLeit == 1) sem_wait(&cond_escr);
        sem_post(&em_l);
        sem_post(&cond_leit);

        //inicio da leitura
        printf("Leitora %d esta lendo\n", *id);
        sleep(1);

        sem_wait(&em_l); qntdLeit--;
        if (qntdLeit == 0) sem_post(&cond_escr);
        sem_post(&em_l);

        printf("Leitora %d leu\n", *id);
    }

}

//thread escritora
void * escritor (void * arg) {
  int *id = (int *) arg;
  while(1) {
      printf("E[%d] quer escrever\n", *id);

      sem_wait(&em_e); qntdEscr++;
      //condicional de escrita na fila para colocar leitura em espera
      if (qntdEscr == 1) sem_wait(&cond_leit);
      sem_post(&em_e);
      sem_wait(&cond_escr);

      //inicio da escrita
      printf("Escritora %d esta escrevendo\n", *id);
      sleep(2);

      sem_post(&cond_escr);
      sem_wait(&em_e); qntdEscr--;
      //condicional de fila vazia para liberar leitura
      if (qntdEscr == 0) sem_post(&cond_leit);
      sem_post(&em_e);

      printf("Escritora %d escreveu\n", *id);
  }



}

//funcao principal
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

  //inicializa os semaforos
  //mutex
  sem_init(&em_e, 0, 1);
  sem_init(&em_l, 0, 1);
  //condicionais
  sem_init(&cond_escr, 0, 1);
  sem_init(&cond_leit, 0, 1);

  pthread_exit(NULL);
  return 0;


}

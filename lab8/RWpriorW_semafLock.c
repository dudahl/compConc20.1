#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define L 2
#define E 1

//variaveis do problema
int leit=0; //contador de threads lendo
int escr=0; //contador de threads escrevendo
int qntdEscr=0; //escritores que ainda vao ser atendidos

//variaveis para sincronizacao
pthread_mutex_t mutex;
pthread_cond_t cond_escr, cond_leit;

void * leitor (void * arg) {
  int *id = (int *) arg;
  while(1){
      pthread_mutex_lock(&mutex);
      printf("#L%d quer ler\n", *id);
      while (escr > 0 || qntdEscr > 0) {
        printf("#L%d bloqueou: %d na fila e %d escritor(es)\n", *id, qntdEscr, escr);
        pthread_cond_wait(&cond_leit, &mutex);
        printf("#L%d desbloqueou: %d na fila e %d escritor(es)\n", *id, qntdEscr, escr);
      }

      leit++;
      pthread_mutex_unlock(&mutex);

      //inicio da leitura
      printf("#L%d está lendo\n", *id);
      sleep(1);

      pthread_mutex_lock(&mutex); leit--;
      if (leit == 0) {
          pthread_cond_signal(&cond_escr);
      }
      pthread_mutex_unlock(&mutex);

      printf("#L%d terminou de ler\n", *id);
  }

}

void * escritor (void * arg) {
    int *id = (int *) arg;
    while(1) {
      printf("#E%d quer escrever\n", *id);

      pthread_mutex_lock(&mutex);
      qntdEscr++; //acrescenta escritor para ser atendido
      while (leit > 0 || escr > 0) {
        printf("#E%d bloqueou: %d escritores e %d leitores\n", *id, escr, leit);
        pthread_cond_wait(&cond_escr, &mutex);
        printf("#E%d desbloqueou: %d escritores e %d leitores\n", *id, escr, leit);
      }

      escr++;
      pthread_mutex_unlock(&mutex);

      // Inicio da escrita
      printf("#E%d escrevendo\n", *id);
      sleep(2);

      pthread_mutex_lock(&mutex);
      qntdEscr--; //escritor atendido
      escr--;
      if (qntdEscr == 0) pthread_cond_broadcast(&cond_leit); //prossegue se todos escritores ja foram atendidos ou ainda nao ha nenhum para ser atendido
      pthread_mutex_unlock(&mutex);

      printf("#E%d terminou de escrever\n", *id);
    }



}

int main(void) {
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

    //inicializa as variaveis de sincronizacao
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_escr, NULL);

    //inicia os semaforos
//    sem_init(&cond_escrita, 0, 1); //condição
//    sem_init(&cond_leitura, 0, 1); //condição

    pthread_exit(NULL);
    return 0;


}

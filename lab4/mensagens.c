#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NTHREAD  4

int AUX= 0;
char mensagem1[15] = "tudo bem?";
char mensagem2[15] = "bom dia?";
char mensagem3[15] = "ate mais!";
char mensagem4[15] = "boa tarde!";

pthread_mutex_t mutex;
pthread_cond_t cond;

void *primarias(void *args) {
    int * thID = (int *) args;

    switch(*thID+1) {
    case 1: printf("(thread%d) %s\n", *thID+1, mensagem1);
      break;
    case 2: printf("(thread%d) %s\n", *thID+1, mensagem2);
      break;
    }

    pthread_mutex_lock(&mutex);
    AUX++;

    if(AUX == 2) {
        //printf("a thread%d sinaliza a condicao\n", *thID+1);
        pthread_cond_broadcast(&cond);
    }

    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void *finais(void *args) {
    int * thID = (int *) args;

    //printf("(thread%d) inicia\n", *thID+1);
    pthread_mutex_lock(&mutex);

    if(AUX<2){
        //printf("(thread%d) se bloqueia\n", *thID+1);
        pthread_cond_wait(&cond, &mutex);
        //printf("a thread%d recebe o signal e realoca mutex\n", *thID+1);
    }

    switch(*thID+1) {
      case 3: printf("(thread%d) %s\n", *thID+1, mensagem3);
        break;
      case 4: printf("(thread%d) %s\n", *thID+1, mensagem4);
        break;
    }

    AUX++;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int IDlocal[NTHREAD] = {0, 1, 2, 3};
    void *exec;
    pthread_t IDsist[NTHREAD];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    //criando as threads
    for (int i=0; i<NTHREAD; i++){
        if(i<2) {
          exec=primarias;
        } else {
          exec=finais;
        } pthread_create(&IDsist[i], NULL, exec, &IDlocal[i]);
    }

    //espera as threads finalizarem
    for (int j=0; j<NTHREAD; j++) {
        pthread_join(IDsist[j], NULL);
    }

    printf("\nFim.");

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}

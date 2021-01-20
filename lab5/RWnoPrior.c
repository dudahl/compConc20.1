/* Sem prioridades (definicao basica do problema): sempre que um leitor quiser ler
e nao houver escritor escrevendo (pode haver escritor esperando), ele tem acesso
a estrutura de dados. Nesta solucao, um escritor pode ter de esperar por muito
tempo (inanicao), caso novos leitores cheguem com frequencia.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define L 4 //numero de threads leitoras
#define E 2 //numero de threads escritoras
#define tamanho 6

//variaveis do problema
int *vetor;
//int tamanho = 6;
int leit=0; //contador de threads lendo
int escr=0; //contador de threads escrevendo

//variaveis para sincronizacao
pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_escr;

//entrada leitura
void InicLeit (int id) {
  pthread_mutex_lock(&mutex);
  printf("L[%d] quer ler\n", id);
  while(escr > 0) {
    printf("L[%d] bloqueou\n", id);
    pthread_cond_wait(&cond_leit, &mutex);
    printf("L[%d] desbloqueou\n", id);
  }
  leit++;
  pthread_mutex_unlock(&mutex);
}

//saida leitura
void FimLeit(int id) {
  pthread_mutex_lock(&mutex);
  printf("L[%d] terminou de ler\n", id);
  leit--;
  if(leit==0) pthread_cond_signal(&cond_escr);
  pthread_mutex_unlock(&mutex);
}

//entrada escrita
void InicEscr (int id) {
  pthread_mutex_lock(&mutex);
  printf("E[%d] quer escrever\n", id);
  while((leit>0) || (escr>0)) {
    printf("E[%d] bloqueou\n", id);
    pthread_cond_wait(&cond_escr, &mutex);
    printf("E[%d] desbloqueou\n", id);
  }
  escr++;
  pthread_mutex_unlock(&mutex);
}

//saida escrita
void FimEscr (int id) {
  pthread_mutex_lock(&mutex);
  printf("E[%d] terminou de escrever\n", id);
  escr--;
  pthread_cond_signal(&cond_escr);
  pthread_cond_broadcast(&cond_leit);
  pthread_mutex_unlock(&mutex);
}

//thread leitora
void * leitor (void * arg) {
  int *id = (int *) arg;
  while(1) {
    int i;
    double cont = 0;
    char buf[20 * (tamanho + 2)] = "";
    char *str = buf;

    InicLeit(*id);
    printf("Leitora %d esta lendo\n", *id);

    //percorre o vetor de inteiros
    sprintf(str, "#L[%d] vetor lido: ", *id); //obs.: a vantagem do sprintf aqui se reduz a poder formatar qq dado dentro de uma string
    for (i = 0; i < tamanho; i++) {
      sprintf(str, " %s%d", buf, vetor[i]);
      cont = cont + vetor[i];
    }
    //e imprime na tela seu conteudo e a media dos valores encontrados
    printf("%s | média: %.2f\n", buf, (cont/tamanho));

    FimLeit(*id);
    sleep(1);
  }
  free(arg);
  pthread_exit(NULL);
}

//thread leitora
void * escritor (void * arg) {
  int *id = (int *) arg;
  while(1) {
    InicEscr(*id);
    printf("Escritora %d esta escrevendo\n", *id);

    //modifica o conteudo do vetor escrevendo o valor do seu identificador  ́
    //de thread na aplicacao na primeira e ultima posicao do vetor
    vetor[0] = *id;
    vetor[tamanho - 1] = *id;
    //e o dobro desse valor nas demais posicoes
    for (int i = 1; i < (tamanho-1); i++) {
      vetor[i] = (*id)*2;
    }
    FimEscr(*id);
    sleep(1);
  }
  free(arg);
  pthread_exit(NULL);
}

void inicializaca_aleatorio() {
  int j;
  time_t tp;

  srand((unsigned) time(&tp));
  vetor = malloc(tamanho * sizeof(int));
  for (j = 0; j < tamanho; j++) {
    vetor[j] = rand() % 100;
  }
}

//funcao principal
int main(void) {
  //identificadores das threads
  pthread_t tid[L+E];
  int id[L+E];

  //inicializa as variaveis de sincronizacao
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond_leit, NULL);
  pthread_cond_init(&cond_escr, NULL);
  inicializaca_aleatorio();

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

  pthread_exit(NULL);
  return 0;
}

class V {
  private int tamanho;
  private int[] vetor;

  //cria o vetor, que primeiramente fica vazio
  public V(int tamanho) {
    this.tamanho = tamanho;
    this.vetor = new int[tamanho];

    for (int i = 0; i < tamanho; i++) {
      vetor[i] = 0;
    }
  }

  //inicializa o vetor com valores aleatorios
  public void inicializaVetor() {
    for (int j = 0; j < tamanho; j++) {
      vetor[j] = (int)Math.round(Math.random() * 10);
    }
  }

  // Impressão do vetor
  public void imprimeVetor() {
    for (int k = 0; k < tamanho; k++) {
      System.out.print("| " + vetor[k] + " ");
    }
    System.out.print("| \n");
  }

  public int[] v() {
    return this.vetor;
  }

  public int tam() {
    return this.vetor.length;
  }

}

//classe que estende Thread e implementa a tarefa de cada thread do programa
class Soma extends Thread {

  //identificador da thread
  private int id;

  //objetos compartilhados com outras threads
  V A;
  V B;
  V C;
  int N;

  //construtor
  public Soma(int tid, V A, V B, V C, int N){
    this.id = tid;
    this.A = A;
    this.B = B;
    this.C = C;
    this.N = N;
  }

  //metodo main da thread
  public void run(){
    for (int p = this.id; p < A.tam(); p+=N) {
       C.v()[p] = A.v()[p] + B.v()[p];
    }
  }

}

//classe da aplicacao
public class somaAB{

  public static void main(String[] args){
    int N;
    int tamanho;

    //parametros que o usuario devera informar como entrada ao executar
    N = Integer.parseInt(args[0]); //quantidade de threads
    tamanho =  Integer.parseInt(args[1]); //tamanho de cada vetor


    //reserva espaço para um vetor de threads
    Thread[] threads = new Thread[N];

    //--PASSO 1: cria uma instancia do recurso compartilhado entre as threads
    V A = new V(tamanho);
    V B = new V(tamanho);
    V C = new V(tamanho);

    A.inicializaVetor();
    System.out.println("vetor A:");
    A.imprimeVetor();

    B.inicializaVetor();
    System.out.println("vetor B:");
    B.imprimeVetor();

    //--PASSO 2: cria as threads da aplicacao
    for (int q = 0; q < threads.length; q++) {
       threads[q] = new Soma(q, A, B, C, N);
    }

    //--PASSO 3: iniciar as threads
    for (int r = 0; r < threads.length; r++) {
       threads[r].start();
    }

    //--PASSO 4: esperar pelo termino de todas as threads
    for (int s = 0; s < threads.length; s++) {
       try { threads[s].join(); }
       catch (InterruptedException e) { return; }
    }

    System.out.println("Soma A+B = vetor C:");
    C.imprimeVetor();

  }
}

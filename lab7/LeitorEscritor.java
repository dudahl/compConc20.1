//--------------------------------------------------------
// Leitor
class Leitor extends Thread {
	int id; //identificador da thread
	int delay; //atraso bobo
	Buffer buffer;

	// Construtor
	public Leitor(int id, int delayTime, Buffer b) {
		this.id = id;
		this.delay = delayTime;
		this.buffer = b;
	}

	// Método executado pela thread
	public void run() {
		while ( true ) {
			this.buffer.EntraLeitor(this.id);
      try {
			    sleep(this.delay);
			} catch(InterruptedException e) {return; }
		}
	}
}

//--------------------------------------------------------
// Escritor
class Escritor extends Thread {
	int id; //identificador da thread
	int delay; //atraso bobo...
	Buffer buffer;

	// Construtor
	public Escritor(int id, int delayTime, Buffer b) {
		this.id = id;
		this.delay = delayTime;
		this.buffer = b;
	}

	// Método executado pela thread
	public void run() {
		while (true) {
				this.buffer.EntraEscritor(this.id);
				try {
				    sleep(this.delay); //atraso bobo...
				} catch(InterruptedException e) {return; }
		}
	}
}

//--------------------------------------------------------
// Classe principal
class LeitorEscritor {
	static final int L = 4;
	static final int E = 3;
	static final int s = 10; // Parametro de tamanho do Buffer

	public static void main (String[] args) {
		int i;
		Buffer buffer = new Buffer(s);         // Buffer (objeto compartilhado entre leitores e escritores)
		Leitor[] l = new Leitor[L];  					// Threads leitores (consumidores)
		Escritor[] e = new Escritor[E];      // Threads escritores (produtores)

		for (i=0; i<L; i++) {
			l[i] = new Leitor(i+1, (i+1)*500, buffer);
			l[i].start();
		}
		for (i=0; i<E; i++) {
			e[i] = new Escritor(i+1, (i+1)*500, buffer);
			e[i].start();
		}
	}
}

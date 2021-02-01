class Buffer {
	private int[] buffer;
	private int inicio;
	private int fim;

	public Buffer(int tamanho) {
		this.buffer = new int[tamanho];
		this.inicio = -1;
		this.fim = -1;
	}

	private boolean bufferCheio() {
		return this.inicio == this.fim && this.inicio != -1;
	}

	private boolean bufferVazio() {
		return this.inicio == this.fim && this.inicio == -1;
	}

	public synchronized void EntraEscritor(int aux) {
		try {
			System.out.println("Produtor #" + aux + " quer entrar p/ escrever");

			//Se o buffer estiver cheio, a thread que tenta inserir um novo elemento deve ser	bloqueada.
			while (this.bufferCheio()) {
				System.out.println("Produtor #" +aux+ " -> bloqueado");
				this.wait();
				System.out.println("Produtor #" +aux+ " -> notificado");
			}

			if (this.inicio == -1) {
				this.inicio = 0;
				this.fim = 0;
			}

			this.buffer[this.fim] = aux;
			System.out.println("Produtor #" + aux + " escreveu o valor " + aux + " na posição " + this.fim);
			this.fim = (this.fim + 1) % this.tamanho();

		} catch (InterruptedException e) {
			System.out.println(e);

		} finally {
			this.notifyAll();
		}
	}

	public synchronized int EntraLeitor(int id) {
		int aux = 0;
		try {
			System.out.println("Consumidor #" + id + " quer entrar p/ remover");

			// Se o buffer estiver vazio, a thread que tenta remover um elemento deve ser bloqueada.
			while (this.bufferVazio()) {
				System.out.println("Consumidor #" + id + " -> bloqueado");
				this.wait();
				System.out.println("Consumidor #" + id + " notificado");
			}

			aux = this.buffer[this.inicio];
			System.out.println("Consumidor #" + id + " removeu o valor " + aux + " na posição " + this.inicio);
			this.inicio = (this.inicio+1) % this.tamanho();

			if (this.inicio == this.fim) {
				this.inicio = this.fim = -1;
			}

		} catch (InterruptedException e) {
			System.out.println(e);

		} finally {
			this.notifyAll();
		}

		return aux;
	}

	public int tamanho() {
		return this.buffer.length;
	}

	public synchronized void imprimeBuffer() {
		System.out.print("<< buffer >> ");
		for (int i = 0; i < this.buffer.length; i++) {
			if (this.bufferVazio()) {
				System.out.print(" [/] ");
			} else if (this.inicio <= i || this.fim > i) {
				System.out.print( this.buffer[i] + " ");
			} else {
				System.out.print(" ");
			}
		}
		System.out.println();
	}
}

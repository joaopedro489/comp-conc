//classe que estende Thread e implementa a tarefa de cada thread do programa
class Thr extends Thread {
	//identificador da thread
	private int id;
	//objetos compartilhados com outras threads
	public int[] vetor;
	public int nthreads;

	//construtor
	public Thr(int tid, int[] vetor, int nthreads) {
		this.id = tid;
		this.vetor = vetor;
		this.nthreads = nthreads;
	}

	//metodo main da thread
	public void run() {
		System.out.println("Thread " + this.id + " iniciou!");
		for (int i=this.id; i<this.vetor.length; i+=nthreads) {
			this.vetor[i]++;
		}
		System.out.println("Thread " + this.id + " terminou!");
	}
}

//classe da aplicacao
class Principal {
	public static void main (String[] args) {
		if(args.length == 0){
			System.out.println("java Principal <nº de threads> <tamanho do vetor>");
			System.exit(1);
		}
		int tam = Integer.parseInt(args[1]);
		//numero de threads
		int nthreads = Integer.parseInt(args[0]) > tam ? tam : Integer.parseInt(args[0]);
		//reserva espaço para um vetor de threads
		Thread[] threads = new Thread[nthreads];

		//cria o vetor compartilhado entre as threads
		int[] vetor = new int[tam];
		for (int i = 0; i< tam; i++ ) {
			vetor[i] = 0;
			System.out.printf("%d ",vetor[i]);
		}
		System.out.printf("\n");
		//cria as threads da aplicacao
		for (int i=0; i<threads.length; i++) {
		   threads[i] = new Thr(i, vetor, nthreads);
		}

		//inicia as threads
		for (int i=0; i<threads.length; i++) {
		   threads[i].start();
		}

		//espera pelo termino de todas as threads
		for (int i=0; i<threads.length; i++) {
		   try { threads[i].join(); } catch (InterruptedException e) { return; }
		}
		for (int i = 0; i< tam; i++) {
			System.out.printf("%d ",vetor[i]);
		}
		System.out.printf("\n");
	}
}

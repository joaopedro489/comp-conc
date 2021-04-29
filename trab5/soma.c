#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
int *array;
int nthreads;
int bloqueadas = 0;

//Funcao barreira
void barreira(int iter) {
	//inicio secao critica
	pthread_mutex_lock(&mutex);
	//ultima thread a chegar na barreira
	if (bloqueadas == nthreads - iter - 1) {
		bloqueadas = 0;
		pthread_cond_broadcast(&cond);
	}
	else {
		bloqueadas++;
		pthread_cond_wait(&cond, &mutex);
	}
	//fim secao critica
	pthread_mutex_unlock(&mutex);
}

//Funcao das threads
void * prefixo(void * arg) {
	int id = *(int *)arg;
	int aux;
	for(int i = 1; i <= id; i *= 2) {
		aux = array[id - i];
		//espera leitura dos elementos
		barreira(i);
		array[id] += aux;
		//espera soma dos elementos
		barreira(i);
	}
	pthread_exit(NULL);
}

//Funcao principal
int main(int argc, char * argv[]) {

	pthread_t *tid;
	int *id_thread;

	if (argc < 2) {
		printf("%s <tamanho do vetor em pot de 2>\n", argv[0]);
		return 1;
	}

	nthreads = pow(2, atoi(argv[1])) > 300 ? 300 : pow(2, atoi(argv[1]));

	//Aloca a memória
	id_thread = (int *) malloc(sizeof(int) * nthreads);
	if(id_thread == NULL) {
		printf("ERRO - malloc\n");
		return -1;
	}
	array = (int *) malloc(sizeof(int) * nthreads);
	if(array == NULL) {
    	printf("ERRO - malloc\n");
    	return -1;
	}

	tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
	if(tid == NULL) {
    	printf("ERRO - malloc\n");
    	return -1;
	}

	//Inicializa um array de 1
	for(int i = 0; i < nthreads; i++) {
		array[i] = 1;
	}

	//Inicializa o mutex e a variavel de condicao
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	//Cria as threads
	for (int i = 0; i < nthreads; i++) {
		id_thread[i] = i;
		pthread_create(tid+i, NULL, prefixo, (void *)&id_thread[i]);
	}

	//Espera todas as threads completarem
	for (int i = 0; i < nthreads; i++) {
		pthread_join(*(tid+i), NULL);
	}

	printf("Soma Sequencial:  ");
	for(int i = 1; i <= nthreads; i++) {
		printf("%d ", i);
	}
	printf("\n");

	printf("Soma Concorrente: ");
	for(int i = 0; i < nthreads; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");

	//final da execução
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	free(id_thread);
	free(array);
	return 0;
}

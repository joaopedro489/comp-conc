#include <stdio.h>
#include <pthread.h>
#define ARRAY_SIZE 10000

int vetor[ARRAY_SIZE];

void* aumentaValor(void * arg){
	int index =  *(int *) arg;

	for (int i = index; i < ARRAY_SIZE; i+=2) {
		vetor[i]++;
	}
	pthread_exit(NULL);
}


int main(void) {
	for (int i = 0; i < ARRAY_SIZE; i++) {
		vetor[i] = i;
	}
	int input[2];
	input[0] = 0;
	input[1] = 1;
	pthread_t tid[2];

	for (int i = 0; i < 2; i++) {
		if(	pthread_create(&tid[i], NULL, aumentaValor, (void *) &input[i])){
			printf("Erro ao criar mais um Thread\n");
			return -1;
		}
	}
	for (int i = 0; i < 2; i++) {
		if(pthread_join(tid[i], NULL)){
			printf("Erro ao executar a Thread\n");
			return -1;
		}
	}
	for (int i = 0; i < ARRAY_SIZE; i++) {
		if((vetor[i]) != (i+1) ){
			printf("Não aumentou em 1\n");
			return -1;
		}
	}
	printf("Todos os valores aumentam em 1\n");
	return 0;
}

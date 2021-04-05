#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float * firstMat;
float * secondMat;
float * result;
int nthreads;
int dim;

//função da multiplicação de matriz que as threads executarão
void * multiplyMatrices(void * arg){
	int id = *(int *) arg;
	for (int i = id; i < dim; i+=nthreads) {
		for (int j = 0; j < dim; j++) {
			for (int k = 0; k < dim; k++) {
				result[i*dim + j] += firstMat[i*dim + k] * secondMat[k*dim + j];
			}
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
	pthread_t *t_id;
	double inicio, fim, delta;
	//Inicialização do programa
	GET_TIME(inicio);
	if(argc<3) {
      printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
      return 1;
  	}

	dim = atoi(argv[1]);
	nthreads = atoi(argv[2]) > dim ? dim : atoi(argv[2]);

	//alocacao de memoria para as estruturas de dados
	firstMat = (float *) malloc(sizeof(float)*dim*dim);
	secondMat = (float *) malloc(sizeof(float)*dim*dim);
	result = (float *) malloc(sizeof(float)*dim*dim);

	//inicializacao das estruturas de dados de entrada e saida
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			firstMat[i*dim + j] = 1;
			secondMat[i*dim + j] = 1;
			result[i*dim + j] = 0;
		}
	}
	int arrayId[nthreads];
	GET_TIME(fim);
   	delta = fim - inicio;
   	printf("Tempo inicializacao:%lf\n", delta);

	//Multiplicação em diferentes threads, parte concorrente do programa
	GET_TIME(inicio);
	t_id = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
	//criacao das threads
	for (int i = 0; i < nthreads; i++){
		arrayId[i] = i;
		pthread_create(t_id+i,NULL,multiplyMatrices,(void *) &arrayId[i]);
	}

	//espera pelo termino da threads
	for (int i = 0; i < nthreads; i++)
		pthread_join(*(t_id+i), NULL);

	GET_TIME(fim);
   	delta = fim - inicio;
   	printf("Tempo multiplicacao:%lf\n", delta);

	//finalizacao do programa
	GET_TIME(inicio);
	int check = 0;
	//checagem do resultado da matriz
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			if(result[i*dim + j] != dim){
				check = 1;
				break;
			}
		}
	}
	//liberação de memoria
	free(firstMat);
	free(secondMat);
	free(result);
	free(t_id);
	GET_TIME(fim);
   	delta = fim - inicio;
   	printf("Tempo finalizacao:%lf\n", delta);
	printf("O resultado da matriz é %s.\n", (check) ? "errado" : "correto");
	return 0;
}

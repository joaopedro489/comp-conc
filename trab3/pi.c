#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

long long int n;
int nthreads;

void * somaDeValores(void * arg) {
	long long int id = (long long int) arg;
	double *somaLocal;
	somaLocal = (double*) malloc(sizeof(double));
	if(somaLocal == NULL) {
		printf("Sem memoria para alocação\n");
		exit(1);
	}
	long long int tamBloco = n/nthreads;
	long long int ini = id * tamBloco;
	long long int fim = (id == nthreads - 1) ?  n : ini + tamBloco;
	for(long long int i = ini; i < fim; i++)
		*somaLocal += (i % 2 == 0 )? 1/((double)i*2 + 1) : -1/((double)i*2 + 1);
	//retorna o resultado da soma local
	pthread_exit((void *) somaLocal);
}

int main(int argc, char *argv[]) {
	double somaSeq = 0;
	double somaConc = 0;
	pthread_t *tid;
	double ini, fim;

	double *retorno;
	if(argc < 3) {
		fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
		return 1;
	}
	n = atoi(argv[1]);
	nthreads = atoi(argv[2]);

	//soma sequencial dos elementos
	GET_TIME(ini);
	for(long long int i=0; i<n; i++)
		somaSeq += (i % 2 == 0 )? 1/((double)i*2 + 1) : -1/((double)i*2 + 1);
	GET_TIME(fim);
	printf("Tempo sequencial:  %lf\n", fim-ini);

	//soma concorrente dos elementos
	GET_TIME(ini);
	tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
	if(tid==NULL) {
		printf("Sem memoria para alocação\n");
		return 1;
	}
	//criar as threads
	for(long long int i=0; i<nthreads; i++) {
		if(pthread_create(tid+i, NULL, somaDeValores, (void*) i)){
			printf("Erro pthread_create\n");
			return 1;
		}
	}
	//aguardar o termino das threads
	for(long long int i=0; i<nthreads; i++) {
		if(pthread_join(*(tid+i), (void**) &retorno)){
			printf("Erro pthread_join\n");
			return 1;
		}
		//soma global
		somaConc += *retorno;
	}
	GET_TIME(fim);
	printf("Tempo concorrente:  %lf\n", fim-ini);

	//exibir os resultados
	printf("Soma seq:  %.15lf\n", 4 * somaSeq);
	printf("Soma conc: %.15lf\n", 4 * somaConc);
	printf("Valor de PI: %.15lf\n", M_PI);
	//libera as areas de memoria alocadas
	free(tid);

	return 0;
}

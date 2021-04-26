#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 4
/* Variaveis globais */
int x = 0;
pthread_mutex_t mutex;
pthread_cond_t cond1, cond2;

//Threads a serem executadas
void* mensagem1(void* arg){
	pthread_mutex_lock(&mutex);
	if(x < 3){
		pthread_cond_wait(&cond2, &mutex);
	}
	printf("Volte sempre!\n");
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}
void* mensagem2(void* arg){
	pthread_mutex_lock(&mutex);
	if(x == 0){
		pthread_cond_wait(&cond1, &mutex);
	}
	printf("Fique a vontade.\n");
	x++;
	if(x == 3){
		pthread_cond_signal(&cond2);
	}
	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);
}
void* mensagem3(void* arg){
	pthread_mutex_lock(&mutex);
	if(x == 0){
		pthread_cond_wait(&cond1, &mutex);
	}
	printf("Sente-se por favor.\n");
	x++;
	if(x == 3){
		pthread_cond_signal(&cond2);
	}
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}
void* mensagem4(void* arg){
	pthread_mutex_lock(&mutex);
	x++;
	printf("Seja bem-vindo!\n");
	pthread_cond_broadcast(&cond1);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
    pthread_t threads[NTHREADS];

    /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init (&cond1, NULL);
	pthread_cond_init (&cond2, NULL);

    /* Cria as threads */
    pthread_create(&threads[0], NULL, mensagem1, NULL);
    pthread_create(&threads[1], NULL, mensagem2, NULL);
    pthread_create(&threads[2], NULL, mensagem3, NULL);
	pthread_create(&threads[3], NULL, mensagem4, NULL);

	/* Espera todas as threads completarem */
    for (int i = 0; i < NTHREADS; i++) {
      pthread_join(threads[i], NULL);
    }
    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond1);
    pthread_cond_destroy(&cond2);
	return 0;
}

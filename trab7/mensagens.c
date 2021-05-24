//Arquivo para execução em mac OS
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h> //import para a flag O_CREAT

#define NTHREADS 4 //numero de threads

//variáveis globais
sem_t* cond1; //condição das mensagens 2 e 3
sem_t* cond2; //condição da mensagem 4
sem_t* cond3; //condição da mensagem 4

//Tarefas das threads
void* mensagem1(void* arg) {
	printf("Seja bem-vindo!\n");
	sem_post(cond1);
	pthread_exit(NULL);
}

void* mensagem2(void* arg) {
	sem_wait(cond1);
	printf("Fique a vontade.\n");
	sem_post(cond1);
	sem_post(cond2);
	pthread_exit(NULL);
}

void* mensagem3(void* arg) {
	sem_wait(cond1);
	printf("Sente-se por favor.\n");
	sem_post(cond1);
	sem_post(cond3);
	pthread_exit(NULL);
}

void* mensagem4(void* arg) {
	sem_wait(cond2);
	sem_wait(cond3);
	printf("Volte sempre!\n");
	pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
    pthread_t threads[NTHREADS];

	//desvincular qualquer nome de semaforos usado
	sem_unlink("cond1");
	sem_unlink("cond2");
	sem_unlink("cond3");
	//Inicializa os semáforos
	// Foi utilizado o método sem_open ao inves do sem_init por conta das configurações do sistema operacional
	// o mac os não suporta semaforos nao nomeados, com isso é necessario usar sem_open para nomear os semaforos
	cond1 = sem_open("cond1", O_CREAT,0644, 0);
	cond2 = sem_open("cond2", O_CREAT, 0644, 0);
	cond3 = sem_open("cond3", O_CREAT,0644, 0);


	/* Cria as threads */
	pthread_create(&threads[0], NULL, mensagem4, NULL);
	pthread_create(&threads[1], NULL, mensagem3, NULL);
	pthread_create(&threads[2], NULL, mensagem2, NULL);
	pthread_create(&threads[3], NULL, mensagem1, NULL);

	//Espera todas as threads completarem
    for (int i = 0; i < NTHREADS; i++) {
		if (pthread_join(threads[i], NULL)) {
			printf("ERRO: pthread_join(%d)\n", i);
			exit(-1);
		}
  	}
	sem_close(cond1);
	sem_close(cond2);
	sem_close(cond3);

	return 0;
}

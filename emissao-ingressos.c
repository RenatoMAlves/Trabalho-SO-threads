#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

char ingressos[30];
pthread_mutex_t ExcMutua=PTHREAD_MUTEX_INITIALIZER;

struct Comprad{
	char num;
	char nome[10];
};

void *emitir(void *ingresso){
	for(int i=0;i<30;i++){
		pthread_mutex_lock(&ExcMutua);
		ingressos[i]='e';
		pthread_mutex_unlock(&ExcMutua);
		sleep(3);
		puts("\nIngresso emitido");
	}
	pthread_exit(0);
}

void *comprador(void *comp){
	char numero_comprador = ((struct Comprad*)comp)->num;
	bool flag = false;
	for(;;){
		for(int i=0;i<30;i++){
			printf("\n%s", ingressos[i]);
			pthread_mutex_lock(&ExcMutua);
			if(ingressos[i]=='e'){
				puts("\nteste");
				sprintf(ingressos[i], "%c", numero_comprador);
				printf("\n Comprador: %c - Poltrona: %d", numero_comprador, i);
				flag = true;
			}
			pthread_mutex_unlock(&ExcMutua);
		}
		if(flag != true){
			printf("\n%s Ainda não foram emitidos novos ingressos, por favor aguarde", ((struct Comprad*)comp)->nome);
		}
		sleep(1);
	}
	if(strcmp(ingressos[29],'n') != 0 && strcmp(ingressos[29],'e') != 0){
		puts("teste");
		pthread_exit(0);
	}
}


int main(){
	pthread_t comprador1, comprador2, emissor;

	struct Comprad *comp1 = (struct Comprad *)malloc(sizeof(struct Comprad));
	strcpy(comp1->nome, "Comprador1");
	comp1->num = '1';

	struct Comprad *comp2 = (struct Comprad *)malloc(sizeof(struct Comprad));
	strcpy(comp2->nome, "Comprador2");
	comp2->num = '2';


	for(int i=0;i<30;i++){
		ingressos[i] = 'n';
	}


	pthread_create(&emissor, 0, emitir, "");
	pthread_create(&comprador1, 0, comprador, (void *)comp1);
	pthread_create(&comprador2, 0, comprador, (void *)comp2);

	pthread_join(emissor, 0);
	pthread_join(comprador1, 0);
	pthread_join(comprador2, 0);

	printf("\n Os ingressos estão esgotados");
	puts("Relação Poltrona / Comprador");
	for(int i=0;i<30;i++){
		printf("\n Poltrona: %d - Comprador: %d", i, ingressos[i]);	
	}
}

#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
char ingressos[30];
pthread_mutex_t ExcMutua=PTHREAD_MUTEX_INITIALIZER;

struct Comprad{
	int num;
	char nome[10];
};

void *emitir(void *ingresso){
	for(int i=0;i<30;i++){
		pthread_mutex_lock(&ExcMutua);
		ingressos[i]="e";
		sleep(3);
		pthread_mutex_unlock(&ExcMutua);
	}
	pthread_exit(0);
}

void *comprador(void *comp){
	int numero_comprador = ((struct Comprad*)comp)->num;
	bool flag = false;
	for(int i=0;i<30;i++){
		pthread_mutex_lock(&ExcMutua);
		if(ingressos[i]=="e"){
			ingressos[i] = numero_comprador;
			printf("\n Comprador: %d - Poltrona: %d", numero_comprador, i);
			flag = true;
		}
		pthread_mutex_unlock(&ExcMutua);
	}
	if(flag != true){
		printf("\n%s Ainda não foram emitidos novos ingressos, por favor aguarde", ((struct Comprad*)comp)->nome);
	}

	if(ingressos[29] != "n"){
		pthread_exit(0);
	}
}


int main(){
	pthread_t comprador1, comprador2, emissor;
	/*
	struct Comprad{
		int num;
		char nome[10];
	};
	
	struct Comprador comp1;
	struct Comprador comp2;

	comp1.num = 1;
	comp2.num = 2;

	strcpy(comp1.nome, "Comprador 1");
	strcpy(comp2.nome, "Comprador 2");
	*/

	struct Comprad *comp1 = (struct Comprad *)malloc(sizeof(struct Comprad));
	strcpy(comp1->nome, "Comprador1");
	comp1->num = 1;

	struct Comprad *comp2 = (struct Comprad *)malloc(sizeof(struct Comprad));
	strcpy(comp2->nome, "Comprador2");
	comp2->num = 2;


	for(int i=0;i<30;i++){
		ingressos[i] = "n";
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

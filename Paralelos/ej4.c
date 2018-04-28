#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int tamanio=100;
int cantThreads=2;
int *arreglo;
int *resultados;

void * llenarArreglo (){
	time_t t;
	int i;
	srand((unsigned) time (&t));
	for (i=0;i<tamanio;i++){
		arreglo[i]=rand() % 100;

		printf(" %i ", arreglo[i]);
	}
	printf("\n");
}
void * hilo (void * ptr){
	int id=*((int *)ptr);
	int i;
	int desde=id*(tamanio/cantThreads);
	int hasta=desde+(tamanio/cantThreads);

	for (i=desde;i<hasta;i++){
		
	}
	pthread_exit(0);
}
int main(int argc,char*argv[]){
	if ((atoi(argv[1])==2) | (atoi(argv[1])==4)){
		cantThreads=atoi(argv[1]);
	}
	if (atoi(argv[2])>0){
		tamanio=atoi(argv[2]);
	}

	printf("Threads: %i\n", cantThreads);
	printf("Tamaño: %i\n", tamanio);

	int i, ids[cantThreads];
	pthread_attr_t attr;
	pthread_t threads[cantThreads];
	pthread_attr_init(&attr);
	
	arreglo=(int*)malloc(sizeof(int)*tamanio);
	resultados=(int*)malloc(sizeof(int)*cantThreads);

	llenarArreglo();

	/* Crea los hilos */
	for (i=0;i<cantThreads;i++){
		ids[i]=i;
		pthread_create(&threads[i], NULL, hilo, &ids[i]);
	}
	
	/* Espera q terminen los hilos */
	for (i=0;i<cantThreads;i++){
		pthread_join(threads[i],NULL);
	}

	/* Suma todo */
	int total=0;
	for (i=0;i<cantThreads;i++){
		total=total+resultados[i];
	}
	//Imprimir arreglo
	for (i=0;i<tamanio;i++){
		printf(" %i ", arreglo[i]);
	}
	return 0;
}

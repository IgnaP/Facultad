#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int tamanio;
int cantThreads;
int *arreglo;
int *minimos, *maximos;
int elemento;

void * llenarArreglo (){
	time_t t;
	int i;
	srand((unsigned) time (&t));
	for (i=0;i<tamanio;i++){
		arreglo[i]=rand() % 10;

		printf(" %i ", arreglo[i]);
	}
	printf("\n");
}
void * hilo (void * ptr){
	int id=*((int *)ptr);
	int min=10;
	int max=-1;
	int i;
	int desde=id*(tamanio/cantThreads);
	int hasta=desde+(tamanio/cantThreads);

	for (i=desde;i<hasta;i++){
		if (arreglo[i]<min){
			min=arreglo[i];
		}
		if (arreglo[i]>max){
			max=arreglo[i];
		}
	}
	minimos[id]=min;
	maximos[id]=max;
	pthread_exit(0);
}
int main(int argc,char*argv[]){
	cantThreads=atoi(argv[1]);
	tamanio=atoi(argv[2]);

	printf("Threads: %i\n", cantThreads);
	printf("Tamaño: %i\n", tamanio);

	int i, ids[cantThreads];
	pthread_attr_t attr;
	pthread_t threads[cantThreads];
	pthread_attr_init(&attr);
	
	arreglo=(int*)malloc(sizeof(int)*tamanio);
	minimos=(int*)malloc(sizeof(int)*cantThreads);
	maximos=(int*)malloc(sizeof(int)*cantThreads);

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
	int minimo=minimos[0];
	int maximo=maximos[0];
	for (i=1;i<cantThreads;i++){
		if (minimos[i]<minimo){
			minimo=minimos[i];
		}
	}
	for (i=1;i<cantThreads;i++){
		if (maximos[i]>maximo){
			maximo=maximos[i];
		}
	}
	printf("Minimo: %i\n", minimo);
	printf("Maximo: %i\n", maximo);
	return 0;
}

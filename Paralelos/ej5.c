#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int tamanio;
int cantThreads;
int *arreglo;
double *promedios;

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
	int total=0;
	int i;
	int cantElems=tamanio/cantThreads;
	int desde=id*cantElems;
	int hasta=desde+cantElems;

	for (i=desde;i<hasta;i++){
		total=total+arreglo[i];
	}
	promedios[id]=(total/(double)cantElems);
	printf("Prom%i: %f\n", id, promedios[id]);
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
	promedios=(double*)malloc(sizeof(double)*cantThreads);

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
	double total=0;
	for (i=0;i<cantThreads;i++){
		total=total+promedios[i];
	}
	printf("Promedio: %f\n", (total/cantThreads));
	return 0;
}

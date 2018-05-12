#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

int tamanio;
int cantThreads;
int *arreglo;
int *resultados;

double dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}
void * llenarArreglo (){
	int i;
	for (i=0;i<tamanio;i++){
		arreglo[i]=2;

	//	printf(" %i ", arreglo[i]);
	}
	//printf("\n");
}
void * hilo (void * ptr){
	int id=*((int *)ptr);
	int pares=0;
	int i;
	int desde=id*(tamanio/cantThreads);
	int hasta=desde+(tamanio/cantThreads);

	for (i=desde;i<hasta;i++){
		if ((arreglo[i] % 2)==0){
			pares++;
		}
	}
	resultados[id]=pares;
	pthread_exit(0);
}
int main(int argc,char*argv[]){
	cantThreads=atoi(argv[1]);
	tamanio=atoll(argv[2]);
	//tamanio=pow(2,29);

	printf("Threads: %i\n", cantThreads);
	printf("Tamaño: %i\n", tamanio);

	int ids[cantThreads];
	int i;
	pthread_t threads[cantThreads];
	
	arreglo=(int*)malloc(sizeof(int)*tamanio);
	resultados=(int*)malloc(sizeof(int)*cantThreads);

	llenarArreglo();
	double timetick= dwalltime();

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
	int pares=0;
	for (i=0;i<cantThreads;i++){
		pares=pares+resultados[i];
	}
	printf("Pares: %i\n", pares);
	printf("Tiempo en segundos %f \n", dwalltime() - timetick);
	return 0;
}

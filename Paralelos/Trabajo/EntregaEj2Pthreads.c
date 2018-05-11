#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

unsigned long long int tamanio;
int cantThreads;
unsigned long long int *arreglo;
unsigned long long int *resultados;

double dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}
void * llenarArreglo (){
	unsigned long long int i;
	for (i=0;i<tamanio;i++){
		arreglo[i]=2;

	//	printf(" %i ", arreglo[i]);
	}
	//printf("\n");
}
void * hilo (void * ptr){
	int id=*((int *)ptr);
	unsigned long long int pares=0;
	unsigned long long int i;
	unsigned long long int desde=id*(tamanio/cantThreads);
	unsigned long long int hasta=desde+(tamanio/cantThreads);

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
	tamanio=atoi(argv[2]);

	printf("Threads: %i\n", cantThreads);
	printf("Tamaño: %lld\n", tamanio);

	int ids[cantThreads];
	unsigned long long int i;
	pthread_attr_t attr;
	pthread_t threads[cantThreads];
	pthread_attr_init(&attr);
	
	arreglo=(unsigned long long int*)malloc(sizeof(unsigned long long int)*tamanio);
	resultados=(unsigned long long int*)malloc(sizeof(unsigned long long int)*cantThreads);

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
	unsigned long long int pares=0;
	for (i=0;i<cantThreads;i++){
		pares=pares+resultados[i];
	}
	printf("Pares: %lld\n", pares);
	printf("Tiempo en segundos %f \n", dwalltime() - timetick);
	return 0;
}

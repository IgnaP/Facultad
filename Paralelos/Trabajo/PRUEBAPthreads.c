#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

//gcc -pthread -o PRUEBAPthreads PRUEBAPthreads.c -lm

int elevado=27;
int tamanio;
int cantThreads=2;
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
	}
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
	double tiempoTotal;
	int i,j,k;
	int ids[cantThreads];
	pthread_attr_t attr;
	pthread_t threads[cantThreads];
	pthread_attr_init(&attr);
	printf("Threads: %i\n", cantThreads);
	resultados=(int*)malloc(sizeof(int)*cantThreads);
  for(k=0;k<3;k++){
    tiempoTotal=0;
    tamanio= pow(2,elevado);
    printf("Tamaño del arreglo: 2 a la %i (%i)\n", elevado,tamanio);
    arreglo=(int*)malloc(sizeof(int)*tamanio);
    llenarArreglo();

    for (j=0;j<5;j++){
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
	tiempoTotal=tiempoTotal+(dwalltime() - timetick);
    }

    printf("Tiempo promedio %f \n", tiempoTotal/5);
    elevado++;
  }
  return 0;
}

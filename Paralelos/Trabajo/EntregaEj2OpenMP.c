#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

int tamanio;
int cantThreads;
int *arreglo;

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

		printf(" %i ", arreglo[i]);
	}
	printf("\n");
}
int main(int argc,char*argv[]){
	cantThreads=atoi(argv[1]);
	tamanio=atoi(argv[2]);
	double timetick;

	printf("Threads: %i\n", cantThreads);
	printf("Tamaño del arreglo: %i\n", tamanio);

	int i;
	arreglo=(int*)malloc(sizeof(int)*tamanio);
	llenarArreglo();

	omp_set_num_threads(cantThreads);
	timetick = dwalltime();

	int pares=0;
	#pragma omp parallel for shared(arreglo) private(i)
	for(i=0;i<tamanio;i++){
		if ((arreglo[i] % 2)==0){
			#pragma omp critical
			pares++;
		}   
	}

	printf("Pares: %i\n", pares);
	printf("Tiempo en segundos %f \n", dwalltime() - timetick);
	return 0;
}

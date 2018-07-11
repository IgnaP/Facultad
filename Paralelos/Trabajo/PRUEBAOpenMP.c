#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <math.h>

int elevado=27;
int tamanio;
int cantThreads=4;
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
	}
}
int main(int argc,char*argv[]){
	double timetick,tiempoTotal;
	int i,j,k,pares;
	cantThreads=atoi(argv[1]);
	omp_set_num_threads(cantThreads);
	printf("Threads: %i\n", cantThreads);
  for(k=0;k<3;k++){
	tiempoTotal=0;
	tamanio= pow(2,elevado);
	printf("Tamaño del arreglo: 2 a la %i (%i)\n", elevado,tamanio);
	arreglo=(int*)malloc(sizeof(int)*tamanio);
	llenarArreglo();
     for (j=0;j<5;j++){
	timetick = dwalltime();
	pares=0;
	#pragma omp parallel for shared(arreglo) private(i) reduction(+:pares)
	for(i=0;i<tamanio;i++){
		if ((arreglo[i] % 2)==0){
			pares++;
		}   
	}
	tiempoTotal=tiempoTotal+(dwalltime() - timetick);
     }
     printf("Tiempo promedio %f \n", tiempoTotal/5);
     elevado++;
  }
	return 0;
}


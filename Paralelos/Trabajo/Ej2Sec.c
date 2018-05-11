#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

int tamanio;
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

	//	printf(" %i ", arreglo[i]);
	}
	//printf("\n");
}
int main(int argc,char*argv[]){
	tamanio=atoi(argv[1]);
	double timetick;

	printf("Tamaño del arreglo: %i\n", tamanio);

	int i;
	arreglo=(int*)malloc(sizeof(int)*tamanio);
	llenarArreglo();

	timetick = dwalltime();

	int pares=0;
	for(i=0;i<tamanio;i++){
		if ((arreglo[i] % 2)==0){
			pares++;
		}   
	}

	printf("Pares: %i\n", pares);
	printf("Tiempo en segundos %f \n", dwalltime() - timetick);
	return 0;
}

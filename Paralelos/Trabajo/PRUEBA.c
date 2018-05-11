#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

//gcc -o PRUEBA PRUEBA.c -lm

int elevado=27;
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
	}
}
int main(int argc,char*argv[]){
 double timetick;
 double tiempoTotal;
 int i,j,k;
 for (k=0;k<3;k++){
	tiempoTotal=0;
	tamanio= pow(2,elevado);
	printf("Tamaño del arreglo: 2 a la %i (%i)\n", elevado,tamanio);
	arreglo=(int*)malloc(sizeof(int)*tamanio);
	llenarArreglo();
	for (j=0;j<5;j++){
		timetick = dwalltime();
		int pares=0;
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

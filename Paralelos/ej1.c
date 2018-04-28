#include<stdio.h>
#include<stdlib.h>

//Dimension por defecto de las matrices
int N=100;
int cantThreads;
double *A,*B,*C;

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

void * hilo (void * ptr){
	int id=*((int *)ptr);
	int i,j,k;
	int desde=id*(N/cantThreads);
	int hasta=desde+(N/cantThreads);

	for(i=desde;i<hasta;i++){
		for(j=0;j<N;j++){
		    C[i*N+j]=0;
		    for(k=0;k<N;k++){
			 C[i*N+j]=(C[i*N+j] + A[i*N+k] * B[k*N+j]);
		    }
		}
	}
	pthread_exit(0);
}

int main(int argc,char*argv[]){
 int i,j;
 int check=1;
 double timetick,tardo;

 //Controla los argumentos al programa
 if ((argc != 3) || ((N = atoi(argv[1])) <= 0) )
  {
    printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n", argv[0]);
    exit(1);
  }

 //Aloca memoria para las matrices
  A=(double*)malloc(sizeof(double)*N*N);
  B=(double*)malloc(sizeof(double)*N*N);
  C=(double*)malloc(sizeof(double)*N*N);

 //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	A[i*N+j]=1;
	B[i*N+j]=1;
   }
  }   

	cantThreads=atoi(argv[2]);

	printf("Threads: %i\n", cantThreads);
	printf("Tamaño: %i\n", N);

	int ids[cantThreads];
	pthread_attr_t attr;
	pthread_t threads[cantThreads];
	pthread_attr_init(&attr);

timetick = dwalltime();

	/* Crea los hilos */
	for (i=0;i<cantThreads;i++){
		ids[i]=i;
		pthread_create(&threads[i], NULL, hilo, &ids[i]);
	}
	
	/* Espera q terminen los hilos */
	for (i=0;i<cantThreads;i++){
		pthread_join(threads[i],NULL);
	}

tardo=dwalltime() - timetick;

 //Verifica el resultado
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	check=check&&(C[i*N+j]==N);
   }
  }   

  if(check){
   printf("Multiplicacion de matrices: resultado correcto\n");
  }else{
   printf("Multiplicacion de matrices: resultado erroneo\n");
  }

  printf("Tiempo: %f segundos\n", tardo);

 free(A);
 free(B);
 free(C);
 return(0);
}

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

double dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}

int main(int argc,char*argv[]){
 double *A,*B,*C,*D,*L,*AB,*LB,*ABC,*LBD;
 int i,j,k,N;
 int check=1;
 double timetick;

 //Controla los argumentos al programa
  if (argc < 3){
   printf("\n Faltan argumentos:: N dimension de la matriz, T cantidad de threads \n");
   return 0;
  }
  N=atoi(argv[1]);
  int numThreads=atoi(argv[2]);
  omp_set_num_threads(numThreads);
 
   
 //Aloca memoria para las matrices
  A=(double*)malloc(sizeof(double)*N*N);
  B=(double*)malloc(sizeof(double)*N*N);
  C=(double*)malloc(sizeof(double)*N*N);
  D=(double*)malloc(sizeof(double)*N*N);
  L=(double*)malloc(sizeof(double)*N*N);
  AB=(double*)malloc(sizeof(double)*N*N);
  LB=(double*)malloc(sizeof(double)*N*N);
  ABC=(double*)malloc(sizeof(double)*N*N);
  LBD=(double*)malloc(sizeof(double)*N*N);

 //Inicializa las matrices
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	A[i*N+j]=1;
	B[i+j*N]=1;
	C[i*N+j]=1;
	D[i+j*N]=1;
	L[i*N+j]=1;
	AB[i*N+j]=0;
	LB[i*N+j]=0;
	ABC[i*N+j]=0;
	LBD[i*N+j]=0;
   }
  }   

  timetick = dwalltime();
 //Realiza la multiplicacion
 #pragma omp parallel for shared(A,B,C,L,AB,LB) private(i,j,k)
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
    for(k=0;k<N;k++){
	AB[i*N+j]= AB[i*N+j] + A[i*N+k]*B[k+j*N];
	LB[i*N+j]= LB[i*N+j] + L[i*N+k]*B[k+j*N];
    }
   }
  }
 #pragma omp parallel for shared(AB,LB,C,D,ABC,LBD) private(i,j,k)
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
    for(k=0;k<N;k++){
	ABC[i*N+j]= ABC[i*N+j] + AB[i*N+k]*C[k+j*N];
	LBD[i*N+j]= LBD[i*N+j] + LB[i*N+k]*D[k+j*N];
    }
   }
  }

  printf("Tiempo en segundos %f \n", dwalltime() - timetick);
 //Verifica el resultado
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	check=check&&(AB[i*N+j]==N);
   }
  }   
  if(check){
   printf("Multiplicacion de matrices resultado correcto\n");
  }else{
   printf("Multiplicacion de matrices resultado erroneo\n");
  }
 free(A);
 free(B);
 free(C);
 free(D);
 free(L);
 free(AB);
 free(LB);
 free(ABC);
 free(LBD);
 return(0);
}

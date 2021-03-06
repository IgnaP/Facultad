#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

int N;

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

int main(int argc,char* argv[]){

 double *A,*B,*C,*D,*LT,*M,*AB,*ABC,*LB,*LBD;
 double sumab, sumal, promL, promB;
 int i,j,k;
 int check = 1;
 double timetick, tiempoFinal;
 int cantTriangular;

 //Controla los argumentos al programa
 if ((argc != 3) || ((N = atoi(argv[1])) <= 0) ) {
    printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n", argv[0]);
    exit(1);
  }
 int cantThreads=atoi(argv[2]);
 omp_set_num_threads(cantThreads);

 //Aloca memoria para las matrices
 A=(double*)malloc(sizeof(double)*N*N);
 B=(double*)malloc(sizeof(double)*N*N);
 C=(double*)malloc(sizeof(double)*N*N);
 D=(double*)malloc(sizeof(double)*N*N);
 LT=(double*)malloc(sizeof(double)*N*N); //triangular
 M=(double*)malloc(sizeof(double)*N*N);
 AB=(double*)malloc(sizeof(double)*N*N);
 ABC=(double*)malloc(sizeof(double)*N*N);
 LB=(double*)malloc(sizeof(double)*N*N);
 LBD=(double*)malloc(sizeof(double)*N*N);

 //Inicializa las matrices
 for(i=0;i<N;i++){
  for(j=0;j<N;j++){
    A[i*N+j]=1; 
    B[i+N*j]=1;
    AB[i*N+j]=0;
    C[i+N*j]=1;
    ABC[i*N+j]=0;
    LB[i+N*j]=0;
    D[i*N+j]=1;
    LBD[i*N+j]=0;

    if(i<=j){
     LT[i+N*j]=1; //Carga triangular inferior
    }else{
     LT[i+N*j]=0; 
    }
    
    //printf("matriz triangular, valor en %d : %f\n", i+N*j, LT[i+N*j]); 
  }
 }
//printf("\n matriz triangular:\n"); 

 //Realiza la multiplicacion 
 timetick = dwalltime();
#pragma omp parallel for shared(AB,A,B) private(i,j,k)
for(i=0;i<N;i++){
  for(j=0;j<N;j++){
   for(k=0;k<N;k++){
    AB[i*N+j]=AB[i*N+j] + A[i*N+k]*B[k+j*N];
   }
  }
}

//multiplicación matriz triangular

printf("\n");
#pragma omp parallel for shared(LB,LT,B) private(i,j,k)
for (i = 0; i < N; i++){
  for (j = 0; j < N; j++){
    for (k = 0; k < N; k++){
      LB[i*N+j]=LB[i*N+j] + LT[i*N+k]*B[k+j*N];
      //printf("matriz: %d\n", k+j*N);
    }
  }
}
#pragma omp parallel for shared(ABC,LBD,AB,C,LB,D) private(i,j,k)
for(i=0;i<N;i++){
  for(j=0;j<N;j++){
   for(k=0;k<N;k++){
    ABC[i*N+j]=ABC[i*N+j] + AB[i*N+k]*C[k+j*N];
    LBD[i*N+j]=LBD[i*N+j] + LB[i*N+k]*D[k+j*N];
   }
  }
}

//promedios
sumal=0;
sumab=0;
#pragma omp parallel for shared(LT,B) private(i,j)
for (i=0; i<N; i++){
  for (j = 0; j < N; j++){
	#pragma omp critical
	sumal = sumal + LT[i*N+j];
	sumab = sumab + B[i*N+j];
  }
}
promL= sumal/(N*N);
promB= sumab/(N*N);

//Multiplico escalar por cada matriz y sumo
#pragma omp parallel for shared(M,ABC,LBD) private(i,j)
for (i=0; i<N; i++){
  for (j = 0; j < N; j++){
    M[i*N+j] = promL*ABC[i*N+j] + promB*LBD[i*N+j];
  }
}

tiempoFinal = dwalltime() - timetick;


//Imprimo matriz
for (int i = 0; i < N; i++){
  for (int j = 0; j < N; j++){
    printf("valor matriz m en %d : %f\n", i*N+j, M[i*N+j]);
  }
}

//printf("promedio matriz B: %f\n", promB);

printf("Tiempo en segundos %f\n", tiempoFinal);

 
 free(A);
 free(B);
 free(C);
 free(D);
 free(LT);
 free(M);
 free(AB);
 free(ABC);
 free(LB);
 free(LBD);
 return(0);

}

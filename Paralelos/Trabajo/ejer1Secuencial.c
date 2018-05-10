#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}
int N;
int main(int argc,char* argv[]){

 double *A,*B,*C,*D,*LT,*M,*AB,*ABC,*LB,*LBD;
 double sumab, sumal, promL, promB;
 int i,j,k;
 int check = 1;
 double timetick;

 //Controla los argumentos al programa
 if ((argc != 2) || ((N = atoi(argv[1])) <= 0) ) {
    printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n", argv[0]);
    exit(1);
  }
 
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
 //LT matriz triangular inferior
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
     LT[i+N*j]=1; //Cargar triangular inferior
    }else{
     LT[i+N*j]=0; 
    }
    //printf("matriz triangular, valor en %d : %f\n", i+N*j, LT[i+N*j]); 
  }
 }

 //Realiza la multiplicacion 
 timetick = dwalltime();

for(i=0;i<N;i++){
  for(j=0;j<N;j++){
   for(k=0;k<N;k++){
    AB[i*N+j]=AB[i*N+j] + A[i*N+k]*B[k+j*N];
    LB[i*N+j]=LB[i*N+j] + LT[i*N+k]*B[k+j*N];
   }
  }
}

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
for (i=0; i<N; i++){
  for (j = 0; j < N; j++){
      sumal = sumal + LT[i*N+j];
      sumab = sumab + B[i*N+j];
  }
}
promL= sumal/(N*N);
promB= sumab/(N*N);

//Multiplico escalar por cada matriz y sumo
for (i=0; i<N; i++){
  for (j = 0; j < N; j++){
    //M[i*N+j] = promL*ABC[i*N+j] + promB*LBD[i*N+j];
    printf("valor matriz m en %d : %f\n", i*N+j, M[i*N+j]);
  }
}

printf("promedio matriz B: %f\n", promB);

printf("Tiempo en segundos %f\n", dwalltime() - timetick);

 //Verifica el resultado
/* for(i=0;i<N;i++){
  for(j=0;j<N;j++){
   check = check && (C[i*N+j]==N);
  }
 }

 if(check){
  printf("Multiplicacion de matrices correcta\n");
 }else{
  printf("Multiplicacion de matrices erroneo\n");
 }*/
 
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


int N;

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

void imprimirMatriz(double * matriz){
  int i,j;
  for (i = 0; i < N; ++i){
    for (j = 0; j < N; ++j){
        printf("%f |", matriz[i*N+j]);
    }
    printf("\n");
  }
}
void imprimirMatrizColumna(double * matriz){
  int i,j;
  for (i = 0; i < N; ++i){
    for (j = 0; j < N; ++j){
        printf("%f |", matriz[i+N*j]);
    }
    printf("\n");
  }
}


int main(int argc,char* argv[]){

 double *A,*B,*C,*D,*LT,*M,*AB,*ABC,*LB,*LBD;
 double sumab, sumal, promL, promB, tiempo;
 int i,j,k, indiceTriangular;
 int check = 1;
 double timetick;
 int cantTriangular;

 //Controla los argumentos al programa
 if ((argc != 2) || ((N = atoi(argv[1])) <= 0) ) {
    printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n", argv[0]);
    exit(1);
  }
 
 cantTriangular = N*(N+1)/2;

 //Aloca memoria para las matrices
 A=(double*)malloc(sizeof(double)*N*N);
 B=(double*)malloc(sizeof(double)*N*N);
 C=(double*)malloc(sizeof(double)*N*N);
 D=(double*)malloc(sizeof(double)*N*N);
 LT=(double*)malloc(sizeof(double)*cantTriangular); //triangular
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
    LB[i*N+j]=0;
    D[i+j*N]=1;
    LBD[i*N+j]=0;
  }
 }

 //Inicializo triangular todo en 1
 for (i = 0; i < cantTriangular; i++){
 	LT[i] = 1;
 }

//printf("\n matriz triangular:\n"); 
//imprimirMatriz(LT);

 //Realiza la multiplicacion 
 timetick = dwalltime();

for(i=0;i<N;i++){
  for(j=0;j<N;j++){
   for(k=0;k<N;k++){
    AB[i*N+j]=AB[i*N+j] + A[i*N+k]*B[k+j*N];
   }
  }
}

// ---- multiplicación matriz triangular por B ----

for (i = 0; i < N; i++){
  for (j = 0; j <= i; j++){
  	indiceTriangular = j+(i*(i+1)/2); //indice del vector que se multiplicará con valor de B
  	//printf("zona triangular: %d\n", indiceTriangular);
    for (k = 0; k < N; k++){
    	LB[i*N+k] = LB[i*N+k] + LT[indiceTriangular]*B[k+j*N];
      //printf("Zona indice de triangular: %d con zona matriz B: %d\n y guardando en matriz LB zona: %d\n", indiceTriangular, k+j*N, i*N+k);
    }
  }
}

//printf("\n matriz LB:\n");
//imprimirMatriz(LB);

for(i=0;i<N;i++){
  for(j=0;j<N;j++){
   for(k=0;k<N;k++){
    ABC[i*N+j]=ABC[i*N+j] + AB[i*N+k]*C[k+j*N];
    LBD[i*N+j]=LBD[i*N+j] + LB[i*N+k]*D[k+j*N];
   }
  }
}

//printf("\n matriz LBD:\n");
//imprimirMatriz(LBD);

//promedios
sumal=0;
sumab=0;
for (i=0; i<N; i++){
  for (j = 0; j < N; j++){
      sumab = sumab + B[i*N+j];
  }
}

for (i = 0; i < cantTriangular; i++){
	sumal = sumal + LT[i]; //sumando valores triangular
}

promL= sumal/(N*N);
promB= sumab/(N*N);
//printf("Promedio de L: %f\n", promL);
//Multiplico escalar por cada matriz y sumo
for (i=0; i<N; i++){
  for (j = 0; j < N; j++){
    M[i*N+j] = promL*ABC[i*N+j] + promB*LBD[i*N+j];
    //printf("valor matriz m en %d : %f\n", i*N+j, M[i*N+j]);
  }
}

//printf("promedio matriz B: %f\n", promB);
tiempo = dwalltime() - timetick;

 //Verifica el resultado
 for(i=0;i<N;i++){
  for(j=0;j<N;j++){
   check = check && (M[i*N+j] == cantTriangular + (i+1)*N);
  }
 }

 if(check){
  printf("\nMultiplicación de matrices correcta\n");
 }else{
  printf("Multiplicación de matrices erroneo\n");
 }

 printf("Tiempo en segundos %f\n\n", tiempo);
 
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
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

int N, THREADS, totalL, totalB, PASOS;
double *A,*B,*C,*D,*LT,*M,*AB,*ABC,*LB,*LBD, *RESULTL, *RESULTB, promL, promB;

void * multiplicarAByLB (void * ptr);
void * multiplicarABCyLBD (void * ptr);
void * promedio (void * ptr);
void * multEscalarYSuma (void * ptr);

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

int main(int argc,char* argv[]){
 //double sumab, sumal;
 int i,j,k, ids[THREADS];
 int check = 1;
 double timetick;
 pthread_t threads[THREADS];

 //Controla los argumentos al programa
 if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || (THREADS = atoi(argv[2])) <= 0) {
    printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n", argv[0]);
    printf("Arg2: numero de threads\n");
    exit(1);
 }

 printf("N %d\n", N);
 printf("threads: %d\n", THREADS );
 

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
 RESULTL=(double*)malloc(sizeof(double)*THREADS);
 RESULTB=(double*)malloc(sizeof(double)*THREADS);

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
    printf("matriz triangular, valor en %d : %f\n", i+N*j, LT[i+N*j]); 
  }
 }
 printf("antes de pasos\n");
 PASOS = N / THREADS;
 printf("despues de pasos %d\n", PASOS);

 timetick = dwalltime();

for (i = 0; i < THREADS; i++){
  ids[i] = i;
  pthread_create(&threads[i], NULL, &multiplicarAByLB, &ids[i]);
}

for(j=0; i < THREADS; j++){
  pthread_join(threads[j], NULL);
}

for (i = 0; i < THREADS; i++){
  ids[i] = i;
  pthread_create(&threads[i], NULL, &multiplicarABCyLBD, &ids[i]);
}

for(j=0; i < THREADS; i++){
  pthread_join(threads[i], NULL);
}

for (i = 0; i < THREADS; i++){
  ids[i] = i;
  pthread_create(&threads[i], NULL, &promedio, &ids[i]);
}

for(i=0; i < THREADS; i++){
  pthread_join(threads[i], NULL);
}

//sumo el total para promedio
totalL =0;
totalB=0;
for (i = 0; i < THREADS; i++){
  totalL += RESULTL[i];
  totalB += RESULTB[i];
}
promL = totalL/(N*N);
promB = totalB/(N*N);

for (i = 0; i < THREADS; i++){
  ids[i] = i;
  pthread_create(&threads[i], NULL, &multEscalarYSuma, &ids[i]);
}

for(i=0; i < THREADS; i++){
  pthread_join(threads[i], NULL);
}

/*for(i=0;i<N;i++){
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
}*/

//promedios
/*sumal=0;
sumab=0;
for (i=0; i<N; i++){
  for (j = 0; j < N; j++){
      sumal = sumal + LT[i*N+j];
      sumab = sumab + B[i*N+j];
  }
}
promL= sumal/(N*N); //Esto lo tiene que hacer el hilo master
promB= sumab/(N*N);/*

//Multiplico escalar por cada matriz y sumo
/*for (i=0; i<N; i++){
  for (j = 0; j < N; j++){
    M[i*N+j] = promL*ABC[i*N+j] + promB*LBD[i*N+j];
    printf("valor matriz m en %d : %f\n", i*N+j, M[i*N+j]);
  }
}*/

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

void * multiplicarAByLB (void * ptr){
  int * p, id;
  int desde, hasta;
  p = (int *) ptr; //lo casteo a entero
  id = *p; //*p se accede al valor apuntado
  printf("estoy en multiplicar");
  desde = PASOS*id;
  hasta = (id+1)*PASOS;
  printf("Estoy con el hilo %d. Desde %d, hasta %d", id, desde, hasta);
  for (int i=desde; i < hasta; i++){
    for (int j = 0; j < N; i++){
      for (int k = 0; k < N; k++){
        AB[i*N+j]=AB[i*N+j] + A[i*N+k]*B[k+j*N];
        LB[i*N+j]=LB[i*N+j] + LT[i*N+k]*B[k+j*N];
      }
    }
  }
  pthread_exit(0);
}

void * multiplicarABCyLBD (void * ptr){
  int * p, id;
  int desde, hasta;
  p = (int *) ptr; //lo casteo a entero
  id = *p; //*p se accede al valor apuntado
  desde = PASOS*id;
  hasta = (id+1)*PASOS;
  printf("Estoy con el hilo %d. Desde %d, hasta %d\n", id, desde, hasta);
  for (int i=desde; i < hasta; i++){
    for (int j = 0; j < N; i++){
      for (int k = 0; k < N; k++){
        ABC[i*N+j]=ABC[i*N+j] + AB[i*N+k]*C[k+j*N];
        LBD[i*N+j]=LBD[i*N+j] + LB[i*N+k]*D[k+j*N];
      }
    }
  }
  pthread_exit(0);
}

void * promedio (void * ptr){
  int * p, id;
  int desde, hasta, sumal, sumab;
  p = (int *) ptr; //lo casteo a entero
  id = *p; //*p se accede al valor apuntado
  printf("Estoy con el hilo %d\n calculando promedio \n", id);
  sumal=0;
  sumab=0;
  desde = PASOS*id;
  hasta = (id+1)*PASOS;
  for (int i=desde; i< hasta; i++){
    for (int j = 0; j < N; j++){
      sumal = sumal + LT[i*N+j];
      sumab = sumab + B[i*N+j];
    }
  }
  RESULTL[id] = sumal;
  RESULTB[id] = sumab;
  pthread_exit(0);
}

void * multEscalarYSuma (void * ptr){
  int * p, id;
  int desde, hasta;
  p = (int *) ptr; //lo casteo a entero
  id = *p; //*p se accede al valor apuntado
  desde = PASOS*id;
  hasta = (id+1)*PASOS;
  printf("Estoy con el hilo %d\n multiplicando escalar \n", id);
  for (int i=desde; i<hasta; i++){
    for (int j = 0; j < N; j++){
      M[i*N+j] = promL*ABC[i*N+j] + promB*LBD[i*N+j];
    }
  }
}
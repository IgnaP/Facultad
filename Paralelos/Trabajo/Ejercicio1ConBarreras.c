#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

int N, THREADS, PASOS, cantThreads;
double *A,*B,*C,*D,*LT,*M,*AB,*ABC,*LB,*LBD, *RESULTL, *RESULTB, promL, promB, totalL, totalB;
pthread_barrier_t BARRERA;
pthread_mutex_t mutex;

void * calculosMatrices (void * ptr);

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}


void multiplicarAB (int desde, int hasta, int id){
  int i, j, k;
  printf("estoy en multiplicar AB\n");
  printf("Estoy con el hilo %d. Desde %d, hasta %d\n", id, desde, hasta);
  for (i=desde; i < hasta; i++){
    for (j = 0; j < N; j++){
      for (k = 0; k < N; k++){
        AB[i*N+j]=AB[i*N+j] + A[i*N+k]*B[k+j*N];
        LB[i*N+j]=LB[i*N+j] + LT[i*N+k]*B[k+j*N];
      }
    }
  }
  printf("sali de multiplicar AB\n");
}

void multiplicarConTriangular (int desde, int hasta, int id){
  int indiceTriangular;
  printf("estoy en multiplicar triangular con B\n");
  printf("Estoy con el hilo %d. Desde %d, hasta %d\n", id, desde, hasta);
  for (int i=desde; i < hasta; i++){
    for (int j = 0; j <= i; j++){
      indiceTriangular = j+(i*(i+1)/2);
      for (int k = 0; k < hasta; k++){
        LB[i*N+k] = LB[i*N+k] + LT[indiceTriangular]*B[k+j*N];
      }
    }
  }
  //printf("sali de multiplicar AB\n");
}

void multiplicarABCyLBD (int desde, int hasta, int id){
  printf("Estoy con el hilo %d en multiplicar ABC. Desde %d, hasta %d\n", id, desde, hasta);
  for (int i=desde; i < hasta; i++){
    for (int j = 0; j < N; j++){
      for (int k = 0; k < N; k++){
        ABC[i*N+j]=ABC[i*N+j] + AB[i*N+k]*C[k+j*N];
        LBD[i*N+j]=LBD[i*N+j] + LB[i*N+k]*D[k+j*N];
      }
    }
  }
}

void promedio (int desde, int hasta, int id){
  printf("Estoy con el hilo %d\n calculando promedio \n", id);
  int sumal=0;
  int sumab=0;
  for (int i=desde; i< hasta; i++){
    for (int j = 0; j < N; j++){
      sumal = sumal + LT[i*N+j];
      sumab = sumab + B[i*N+j];
    }
  }
  printf("Sali de sumar");
  pthread_mutex_lock(&mutex);
  printf("estoy con el mutex");
  totalL += sumal;
  totalB += sumab;
  cantThreads++;
  if (cantThreads == THREADS){ //El último hilo que llegue va a realizar el promedio final
    printf("Soy el último hilo con id %d \n", id);
    promL = totalL/(N*N);
    promB = totalB/(N*N);
  }
  pthread_mutex_unlock(&mutex);
}

void multEscalarYSuma (int desde, int hasta, int id){
  printf("Estoy con el hilo %d\n multiplicando escalar \n", id);
  for (int i=desde; i<hasta; i++){
    for (int j = 0; j < N; j++){
      M[i*N+j] = promL*ABC[i*N+j] + promB*LBD[i*N+j];
    }
  }
}

int main(int argc,char* argv[]){
 //double sumab, sumal;
 int i,j,k, ids[THREADS];
 int check = 1;
 double timetick, tiempo;
 pthread_t threads[THREADS];

 //Controla los argumentos al programa
 if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || (THREADS = atoi(argv[2])) <= 0) {
    printf("\nUsar arg1: %s N h\nN: Dimension de la matriz (nxn X nxn)\n", argv[0]);
    printf("h: numero de threads\n");
    exit(1);
 }

 printf("N %d\n", N);
 printf("threads: %d\n", THREADS );
 
 int cantTriangular = N*(N+1)/2;

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
  }
 }

//Inicializo triangular todo en 1
for (i = 0; i < cantTriangular; i++){
  LT[i] = 1;
}

PASOS = N/THREADS;

totalL = 0.0;
totalB = 0.0;
cantThreads = 0;

//Inicializo mutex
pthread_mutex_init(&mutex, NULL);

//Inicializo barrera
pthread_barrier_init(&BARRERA, NULL, THREADS);

//Comienza tiempo
timetick = dwalltime();

for (i = 0; i < THREADS; i++){
  ids[i] = i;
  pthread_create(&threads[i], NULL, &calculosMatrices, &ids[i]);
}

/*sumo el total para promedio

for (i = 0; i < THREADS; i++){
  totalL += RESULTL[i];
  totalB += RESULTB[i];
}
promL = totalL/(N*N);
promB = totalB/(N*N);*/

for(i=0; i < THREADS; i++){
  pthread_join(threads[i], NULL);
}

tiempo = dwalltime() - timetick;

pthread_barrier_destroy(&BARRERA);

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
free(RESULTL);
free(RESULTB);

return(0);

}

void * calculosMatrices(void * ptr){
  int * p, id;
  int desde, hasta;
  p = (int *) ptr; //lo casteo a entero
  id = *p; //*p se accede al valor apuntado
  desde = PASOS*id;
  hasta = (id+1)*PASOS;
  multiplicarAB(desde, hasta, id);
  pthread_barrier_wait(&BARRERA);
  multiplicarConTriangular(desde, hasta, id);
  pthread_barrier_wait(&BARRERA);
  multiplicarABCyLBD(desde, hasta, id);
  pthread_barrier_wait(&BARRERA);
  promedio(desde, hasta, id);
  pthread_barrier_wait(&BARRERA);
  multEscalarYSuma(desde, hasta, id);
  pthread_exit(NULL);
}
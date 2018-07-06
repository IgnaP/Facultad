/*
<------  N-Queens Solutions  -----> <---- time ---->
 N:           Total          Unique days hh:mm:ss.--			Nuestro tiempo
 5:              10               2             0.00
 6:               4               1             0.00
 7:              40               6             0.00
 8:              92              12             0.00
 9:             352              46             0.00
10:             724              92             0.00
11:            2680             341             0.00
12:           14200            1787             0.00			0.33305 segundos
13:           73712            9233             0.02
14:          365596           45752             0.05
15:         2279184          285053             0.22
16:        14772512         1846955             1.47			571.474 segundos
17:        95815104        11977939             9.42
18:       666090624        83263591          1:11.21
19:      4968057848       621012754          8:32.54
mpicc -o nQueens2MPI nQueens2MPI.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <mpi.h>
#define MAX_SOLUTIONS sizeof(solutions)/sizeof(int)

#ifndef _NQUEENS_H_
#define _NQUEENS_H_
#endif /* _NQUEENS_H_ */

int resultadosCorrectos[18]={1,0,0,2,10,4,40,92,352,724,2680,14200,73712,365596,2279184,14772512,95815104,666090624};

double dwalltime(){
	double sec;
	struct timeval tv;
	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}

int ok(int queen_number, int row_position, int* position) {
	int i;
	// Check each queen before this one
	for(i = 0; i < queen_number; i++) {
		// Get another queen's row_position
		int other_row_pos = position[i];

		// Now check if they're in the same row or diagonals
		if (other_row_pos == row_position || // Same row
			other_row_pos == row_position - (queen_number - i) || // Same diagonal
			other_row_pos == row_position + (queen_number - i))   // Same diagonal
			return 0;
	}
	return 1;
}

int put_queen(int size, int queen_number, int* position) {
	int i;
	for(i=position[queen_number]+1; i<size; i++) {
		if(ok(queen_number, i, position)) {
			position[queen_number] = i;
			return 1;
		}
	}
	position[queen_number] = -1;
	return 0;
}

int nqueens(int size, int i) {
	int count=0;
	int* position;
	count = 0;
	//for(i=0; i<size; i++) {
		int j;
		position = (int *) malloc(size * sizeof(int));
		position[0] = i;

		for(j = 1; j < size; j++)
			position[j] = -1;

		int queen_number = 1;
		while(queen_number > 0) {
			if(put_queen(size, queen_number, position)) {
				queen_number++;
				if(queen_number == size) {
					count += 1;
					position[queen_number-1] = -1;
					queen_number -= 2;
				}
			} else {
				queen_number--;
			}
		}
	//}
	return count;
}
int master(int slaves,int size, MPI_Status estado){
	int resp,x;
	int soluciones=0;
	int i=0;
	int flag=0;
	//Mientras haya tareas
	while(i<size){ //estas van a ser las tareas, el indice i es lo que le voy a pasar al hilo worker 
		//Pedidos
		MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &estado);
		if(flag){	//flag = true si tengo pedidos
			MPI_Recv(&resp, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
			if(resp != -1){
				soluciones +=resp;
			}
			MPI_Send(&i, 1, MPI_INT, estado.MPI_SOURCE, 0, MPI_COMM_WORLD);
			i++;
		}else{	//Trabaja master
			if(i<size){
				soluciones +=nqueens(size, i);
				i++;
			}	
		}
	}
	//Cancelar resto de pedidos
	int terminar= -99;
	for (x=0; x<slaves; x++) {
		MPI_Recv(&resp, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
		if(resp != -1){ //Si es -1 es porque nunca pudo trabajar, porque son muchos hilos o porque les ganó el master
			soluciones +=resp;
		}
		MPI_Send(&terminar, 1, MPI_INT, estado.MPI_SOURCE, 0, MPI_COMM_WORLD);	
	}
	return soluciones;
}
void slave(int id, MPI_Status estado, int size){
	int soluciones,tarea;
	double start_time = dwalltime();
	//Pedir primera tarea
	tarea = -1;
	MPI_Send(&tarea,1,MPI_INT,0,0,MPI_COMM_WORLD);
	MPI_Recv(&tarea,1,MPI_INT,0,0,MPI_COMM_WORLD,&estado);
	while(tarea>=0){
		//Trabajar
		soluciones = nqueens(size, tarea);
		//Enviar resultados
		MPI_Send(&soluciones,1,MPI_INT,0,0,MPI_COMM_WORLD);
		//Pedir siguiente tarea
		MPI_Recv(&tarea,1,MPI_INT,0,0,MPI_COMM_WORLD,&estado);
	}
	printf("Tiempo: %g segundos de hilo worker %i\n", dwalltime() - start_time, id);
}
int main(int argc, char** argv) {
	int size,soluciones;
	if(argc > 1)	// 1<=Size<=18
		size = atoi(argv[1]);

	int cantidad, identificador;
	MPI_Init(&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &cantidad);
	MPI_Comm_rank (MPI_COMM_WORLD, &identificador);
	MPI_Status estado;

	double start_time = dwalltime();
	if (identificador == 0) {
		soluciones=master(cantidad-1,size,estado);
		printf("Tiempo: %g segundos\n", dwalltime() - start_time);
		//Checkear resultado
		if (resultadosCorrectos[size-1]==soluciones) {
			printf("Soluciones: %d\nOK\n", soluciones);
		} else {
			printf("Soluciones: %d\nERROR\n", soluciones);
		}
	} else {
		slave(identificador,estado, size);
	}
	MPI_Finalize();
	return 0;
}

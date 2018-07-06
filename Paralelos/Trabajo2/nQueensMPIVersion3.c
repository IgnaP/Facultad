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
/*
int find_queens(int size,int pos) {
	int total_count=0;
	nqueens(size, &total_count);
	return total_count;
}
*/
int master(int slaves,int size, MPI_Status estado){
	int resp,x;
	int soluciones=0;
	int i=0;
	int sinTrabajo = 0; 
	int trabajando = 0;
	int flag=0;
	//Asignar primera tarea
	printf("empecé, master\n");
	/*for (x=0; x<slaves-1; ++x) {
		MPI_Recv(&resp, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &estado);
		MPI_Send(&i, 1, MPI_INT, estado.MPI_SOURCE, 1, MPI_COMM_WORLD);
		if (i<size && i>=0){
			i++;
			printf("i: %d\n",i);
		}else{
			i=-999;
			sinTrabajo++;
		}
	}*/
	//Mientras haya tareas
	while(i<size){ //estas van a ser las tareas, el indice i es lo que le voy a pasar al hilo worker 
		//Pedidos
		//while(flag && i<size){	//flag = true si tengo pedidos
			MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &estado);
			if(flag){
				MPI_Recv(&resp, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
				if(resp != -1){
					soluciones +=resp;
					//trabajando--;
				}
				MPI_Send(&i, 1, MPI_INT, estado.MPI_SOURCE, 0, MPI_COMM_WORLD);
				//trabajando++;
				i++;
			}else{
				//Trabaja master
				if(i<size){
					printf("Trabaja master\n");
					soluciones +=nqueens(size, i);
					i++;
				}	
			}
		//}
	}
	//Cancelar resto de pedidos
	int terminar= -99;
	printf("No hay mas pedidos.\n i:%i sinTrabajo: %i\n",i,sinTrabajo);
	for (x=0; x<slaves; x++) {
		printf("hago recive\n");
		MPI_Recv(&resp, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
		printf("x: %i\n", x);
		if(resp != -1){ //Si es -1 es porque nunca pudo trabajar, porque son muchos hilos o porque les ganó el master
			soluciones +=resp;
		}

		printf("hago el send\n");
		MPI_Send(&terminar, 1, MPI_INT, estado.MPI_SOURCE, 0, MPI_COMM_WORLD);	
	}
	printf("Terminé master \n");
	printf("slave: %d \n",slaves);
	return soluciones;
}
void slave(int id, MPI_Status estado, int size){
	int soluciones,tarea;
	double start_time = dwalltime();
	//Pedir primera tarea
	tarea = -1;
	MPI_Send(&tarea,1,MPI_INT,0,0,MPI_COMM_WORLD);
	MPI_Recv(&tarea,1,MPI_INT,0,0,MPI_COMM_WORLD,&estado);
	printf("empezando a trabajar slave\n");
	while(tarea>0){
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
	if(argc > 1)
		size = atoi(argv[1]);

	int cantidad, identificador;
	MPI_Init(&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &cantidad);
	MPI_Comm_rank (MPI_COMM_WORLD, &identificador);
	MPI_Status estado;

	double start_time = dwalltime();
	if (identificador == 0) {
		printf("soy master\n");
		soluciones=master(cantidad-1,size,estado);
		printf("Tiempo: %g segundos\n", dwalltime() - start_time);
		printf("Soluciones: %d\n", soluciones);
	} else {
		printf("soy slave\n");
		slave(identificador,estado, size);
	}
	MPI_Finalize();

	return 0;
}